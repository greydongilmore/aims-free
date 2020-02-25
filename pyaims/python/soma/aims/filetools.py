#  This software and supporting documentation are distributed by
#      Institut Federatif de Recherche 49
#      CEA/NeuroSpin, Batiment 145,
#      91191 Gif-sur-Yvette cedex
#      France
#
# This software is governed by the CeCILL-B license under
# French law and abiding by the rules of distribution of free software.
# You can  use, modify and/or redistribute the software under the
# terms of the CeCILL-B license as circulated by CEA, CNRS
# and INRIA at the following URL "http://www.cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-B license and that you accept its terms.

'''File functions'''
from __future__ import print_function

from __future__ import absolute_import
import os
import sys
import numpy as np
import gzip
import hashlib
import tempfile
import filecmp
from soma import aims


def compare_gzip_files(file1, file2):
    same = False
    # file1
    with gzip.open(file1, 'rb') as f:
        f1 = f.read()
    tmp_file1 = tempfile.mkstemp()
    os.close(tmp_file1[0])
    tf1 = open(tmp_file1[1], 'wb')
    tf1.write(f1)
    trf.close()
    # file2
    with gzip.open(file2, 'rb') as f:
        f2 = f.read()
    tmp_file2 = tempfile.mkstemp()
    os.close(tmp_file2[0])
    tf2 = open(tmp_file2[1], 'wb')
    tf2.write(f2)
    tf2.close()
    
    same = filecmp.cmp(tmp_ref_file[1], tmp_test_file[1])
    
    os.unlink(tmp_file1[1])
    os.unlink(tmp_file2[1])
    
    return same


def compare_text_files(file1, file2, thresh=1e-6):
    '''
    Compare text files (.txt, .csv,..) which may contain nan.
    '''
    arr1 = np.genfromtxt(file1)
    arr2 = np.genfromtxt(file2)
    #if not np.any(np.isnan(arr1)) and not np.any(np.isnan(arr2)):
        #return filecmp.cmp(file1, file2)
    if np.any(np.isnan(arr1)) and np.any(np.isnan(arr2)):
        if np.all(np.isnan(arr1)==np.isnan(arr2)):
            arr1[np.isnan(arr1)]=0
            arr2[np.isnan(arr2)]=0
            return np.max(np.abs(arr2 - arr1)) <= thresh
    elif not np.any(np.isnan(arr1)) and not np.any(np.isnan(arr2)):
        return np.max(np.abs(arr2 - arr1)) <= thresh
    
    return False
    

def compare_nii_files(file1, file2, thresh=50, out_stream=sys.stdout):
    '''
    Compare nifti files (.nii, .nii.gz)
    '''

    # Get md5 for each file
    if os.path.splitext(file1)[-1] == '.gz':
        md5_file1 = hashlib.md5(gzip.open(file1, 'rb').read()).hexdigest()
    else:
        md5_file1 = hashlib.md5(open(file1, 'rb').read()).hexdigest()

    if os.path.splitext(file2)[-1] == '.gz':
        md5_file2 = hashlib.md5(gzip.open(file2, 'rb').read()).hexdigest()
    else:
        md5_file2 = hashlib.md5(open(file2, 'rb').read()).hexdigest()

    if md5_file1 == md5_file2:
        return True
    
    # md5 are differents
    # Check the voxels
    a_1 = aims.read(file1)
    a_2 = aims.read(file2)
    if a_1.arraydata().shape == a_2.arraydata().shape:
        d = a_1.arraydata() - a_2.arraydata()
        if abs(np.max(d) - np.min(d)) < thresh:
            print('WARNING, use aims, absolute difference value:'
                  ' %s' % abs(np.max(d) - np.min(d)), file=out_stream)
            return True
    return False
