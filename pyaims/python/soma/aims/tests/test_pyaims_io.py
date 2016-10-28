# -*- coding: utf-8 -*-
from __future__ import print_function

import tempfile
import os
import sys
import unittest
import shutil
from soma import aims
import numpy as np

class TestPyaimsIO(unittest.TestCase):

    verbose = False
    debug = False


    def setUp(self):
        self.work_dir = tempfile.mkdtemp(prefix='test_pyaims')
        if self.verbose or self.debug:
            print('work directory:', self.work_dir)


    def compare_images(self, vol, vol2, vol1_name, vol2_name, thresh=1e-6,
                       rel_thresh = False):
        #print('comp vol, sizes:', vol.getSize(), vol2.getSize())
        #print('    vsizes:', vol.getVoxelSize(), vol2.getVoxelSize())
        msg = 'comparing %s and %s' % (vol1_name, vol2_name)
        self.assertEqual(vol.getSize(), vol2.getSize(),
                         msg + ': %s != %s'
                         % (str(vol.getSize()), str(vol2.getSize())))
        self.assertTrue(
            np.max(np.abs(np.asarray(vol.getVoxelSize())
                          - vol2.getVoxelSize())) < 1e-6,
            msg + ': voxels size differ: %s != %s'
            % (str(vol.getVoxelSize()), str(vol2.getVoxelSize())))
        if rel_thresh:
            val_range = float(np.max(np.asarray(vol))) \
                - np.min(np.asarray(vol))
            thresh = thresh * val_range
        self.assertTrue(np.max(np.abs(np.asarray(vol) - np.asarray(vol2)))
                        < thresh,
                        msg + ', max diff: %f, max allowed: %f'
                        % (np.max(np.abs(np.asarray(vol) - np.asarray(vol2))),
                           thresh))

    def check_open_files(self, fnames):
        proc_dir = '/proc/self/fd'
        failing = []
        if os.path.exists(proc_dir):
            for fd in os.listdir(proc_dir):
                fdpath = os.path.join(proc_dir, fd)
                if os.path.islink(fdpath):
                    fpath = os.readlink(fdpath)
                    if fpath in fnames:
                        failing.append((fd, fpath))
        if self.verbose and failing:
            print('    ! remaining open files:', failing)
        return failing


    def use_type(self, dtype):
        formats = [('.nii', {'max_dims': 7}),
                   ('.nii.gz', {'max_dims': 7}),
                   '.ima',
                   ('.tiff', {'max_dims': 4}),
                   ('.mnc', {'max_dims': 3}), # minc 4D is OK except voxel size
                   ('.v', {'max_dims': 3})] # ecat >= 4D unsupported

        # create test volume for 8D IO
        vol = aims.Volume((9, 8, 7, 6, 5, 4, 3, 3), dtype=dtype)
        vol.header()['voxel_size'] = [0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.1, 1.2]
        view = ((2, 3, 2, 1, 1, 1, 1, 1), (7, 5, 2, 2, 3, 2, 2, 2),
                (1, 2, 3, 3, 2, 0, 0, 0), (7, 5, 2, 2, 3, 2, 2, 2))
        np.asarray(vol)[:] \
            = np.ogrid[0:np.asarray(vol).ravel().shape[0]].reshape(
                np.asarray(vol).shape)
        failing_files = set()
        for format in formats:
            options = None
            if isinstance(format, tuple):
                format, options = format
            failing_files.update(self.use_format(vol, format, view, options))
        return failing_files


    def use_format(self, vol, format, view, options=None):
        suffixes = {'.dcm': '1', '.tiff': '_t0000_s0'}
        partial_read = ['.nii', '.nii.gz', '.ima', '.dcm']
        partial_write = ['.nii', '.ima']
        default_epsilon = 1e-6
        # ecat scaling is far from exact...
        epsilon = {'.v': {'S16': 1e-6, 'FLOAT': 5e-5, 'rel': True}}
        dtype = aims.typeCode(np.asarray(vol).dtype)

        ndim = len(vol.getSize())
        if options:
            if 'max_dims' in options:
                ndim = options['max_dims']
                vol = aims.VolumeView(vol, [0] * ndim, vol.getSize()[:ndim])
                view = [x[:ndim] for x in view]
                for i in range(ndim, len(vol.header()['voxel_size'])):
                    vol.header()['voxel_size'][i] = 1.

        if self.verbose:
            print('testing type: %s, format: %s, dims: %d'
                  % (dtype, format, ndim))
        # write volume
        fname = os.path.join(
            self.work_dir, 'vol_%s%s' % (dtype, format))
        aims.write(vol, fname)
        vol1_name = os.path.basename(fname) + ' (written)'

        # re-read it
        fname = os.path.join(
            self.work_dir, 'vol_%s%s%s'
            % (dtype, suffixes.get(format, ''), format))
        vol2_name = os.path.basename(fname) + ' (re-read)'
        vol2 = aims.read(fname)
        thresh = epsilon.get(format, default_epsilon)
        rel_thresh = False
        if isinstance(thresh, dict):
            rel_thresh = thresh.get('rel', False)
            thresh = thresh.get(dtype, default_epsilon)

        # ensure we get the same
        self.compare_images(vol, vol2, vol1_name, vol2_name, thresh,
                            rel_thresh)
        del vol2

        # test native file without minf
        minf_fname = fname + '.minf'
        minf = aims.read(minf_fname)
        if 'filenames' not in minf:
            os.unlink(minf_fname)
            vol3_name = os.path.basename(fname) + ' (re-read without .minf)'
            vol3 = aims.read(fname)
            self.compare_images(vol, vol3, vol1_name, vol3_name, thresh,
                                rel_thresh)

        view_pos1, view_size1, view_pos2, view_size2 = view
        if format in partial_read:
            if self.verbose:
                print('    testing partial reading:', format)
            url_ext = '&&'.join(['ox%d=%d' % (i + 1, n)
                                 for i, n in enumerate(view_pos1)])
            url_ext += '&&' + '&&'.join(['sx%d=%d' % (i + 1, n)
                                         for i, n in enumerate(view_size1)])
            vol3 = aims.read(fname + '?%s' % url_ext)
            self.assertEqual(vol3.getSize(), view_size1)
            vol4 = aims.VolumeView(vol, view_pos1, view_size1)
            self.compare_images(vol4, vol3, 'sub-volume', 'patially read',
                                thresh, rel_thresh)

        if format in partial_write:
            if self.verbose:
                print('    testing partial writing:', format)
            vol2 = aims.VolumeView(vol, view_pos2, view_size2)
            url_ext = '&&'.join(['ox%d=%d' % (i + 1, n)
                                 for i, n in enumerate(view_pos2)])
            aims.write(vol2, fname + '?partial_writing=1&&%s' % url_ext)
            vol3 = aims.read(fname)
            self.assertEqual(vol3.getSize(), vol.getSize())
            vol4 = aims.VolumeView(vol3, view_pos2, view_size2)
            # compare the written view
            self.compare_images(vol4, vol2, 'sub-volume %s (write, format %s)'
                                % (aims.typeCode(vol), format),
                                'patially written', thresh, rel_thresh)
            # compare a part of the original volume
            vol2 = aims.VolumeView(vol, (0, 0, 0, 0), (10, 10, 5, 1))
            vol4 = aims.VolumeView(vol3, (0, 0, 0, 0), (10, 10, 5, 1))
            self.compare_images(vol4, vol2, 'sub-volume %s (write, format %s)'
                                % (aims.typeCode(vol), format),
                                'original part', thresh, rel_thresh)

        # check if files remain open
        failing_files = self.check_open_files([fname, minf_fname])
        return failing_files


    def test_pyaims_io(self):
        types = ['S16', 'FLOAT']
        failing_files = set()
        for dtype in types:
            failing_files.update(self.use_type(dtype))
        if failing_files:
            raise RuntimeError('There are still open files: %s'
                % repr(failing_files) )


    def tearDown(self):
        if self.debug:
            print('leaving files in', self.work_dir)
        else:
            shutil.rmtree(self.work_dir)


def test_suite():
    if '-v' in sys.argv[1:] or '--verbose' in sys.argv[1:]:
        TestPyaimsIO.verbose = True
    return unittest.TestLoader().loadTestsFromTestCase(TestPyaimsIO)


if __name__ == '__main__':
    if '-d' in sys.argv[1:] or '--debug' in sys.argv[1:]:
        TestPyaimsIO.debug = True
        try:
            i = sys.argv.index('-d')
        except:
            i = sys.argv.index('--debug')
        del sys.argv[i]
    unittest.main(defaultTest='test_suite')

