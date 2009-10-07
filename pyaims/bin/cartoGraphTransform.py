#!/usr/bin/env python

# Copyright IFR 49 (1995-2009)
#
#  This software and supporting documentation were developed by
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
import glob, string, os, sys
from optparse import OptionParser
from soma import aims

def graphTransform2( g, motions, tmpg ):
  ks = motions.keys()
  x = tmpg.rfind( '.' )
  if x>= 0:
    tmpdat = tmpg[:x] + '.data'
  else:
    tmpdat = tmpg + '.data'
  vol = glob.glob( os.path.join( tmpdat, '*.ima' ) )[0]
  print 'label volume:', vol
  for v in g.vertices():
    l = v[ 'name' ].getString()
    i = v[ 'roi_label' ].getScalar()
    print 'node:', i, ':', l
    if l in ks:
      mot = motions[ l ]
      print 'transfo:', mot
      thr = '/tmp/toto.ima'
      c = [ 'AimsThreshold', '-i', vol, '-o', thr, '-m', 'eq', 
            '-t', str( i ), '-b' ]
      cs = '"' + string.join( c, '" "' ) + '"'
      print cs
      os.system( cs )
      c = [ 'AimsResample', '-i', thr, '-o', thr, '-m', mot, '-t', 'n' ]
      # ... unfinished ...


def graphTransform( g, motions ):
  ks = motions.keys()
  vs = map( lambda x: x.getScalar(), g[ 'voxel_size' ] )
  print 'voxel size:', vs
  for v in g.vertices():
    l = v[ 'name' ].getString()
    b = aims.BucketMap_VOID.fromObject( v[ 'aims_roi' ].get() )
    if l in ks:
      mot = motions[ l ]
      print 'transform', l
      b2 = aims.BucketMap_VOID()
      bi = b[0]
      bo = b2[0]
      for p in bi.keys():
        po = mot.transform( aims.Point3df( p.item(0) * vs[0], 
                                           p.item(1) * vs[1], 
                                           p.item(2) * vs[2] ) )
        poi = aims.Point3d( round( po.item(0) / vs[0] ), 
                            round( po.item(1) / vs[1] ), 
                            round( po.item(2) / vs[2] ) )
        #print p.item(0), p.item(1), p.item(2), '->', poi.item(0), poi.item(1), poi.item(2)
        bo[ poi ] = 0
      # store bucket
      b.get()[0] = bo


def parseOpts( argv ):
  parser = OptionParser( description = 'Apply transformations to ROI graph ' 
                         'nodes.\n' 
                         'Several transformations can apply to different ' 
                         'regions specified by their name.\n' 
                         'Currently only ROI bucket graphs are handled, and ' 
                         'no resampling is done (each voxel is transformed ' 
                         'with no more processing, which may result in holes ' 
                         'and/or unconnected regions if scaling factors are ' 
                         'superior to 1.)' )
  parser.add_option( '-i', '--input', dest='input', help='input graph' )
  parser.add_option( '-o', '--output', dest='output', 
                     help='output graph [default: <INPUT>]' )
  parser.add_option( '-t', "--transformation", dest="transfo", action='append', 
                     help="transformation file. Any -t option must correspond " 
                     "to a -n option" )
  parser.add_option( '-n', "--name", dest="name", action='append', 
                     help="ROI name to apply transformation to. Any -n option " 
                     "must correspond to a -t option" )
  (options, args) = parser.parse_args()
  if args and not options.input:
    options.input = args[0]
    del args[0]
  if args and not options.output:
    options.output = args[0]
    del args[0]
  if not options.output:
    options.output = options.input
  if not options.transfo:
    options.transfo = []
  if not options.name:
    options.name = []
  if len( args ) > 0 or len( options.transfo ) != len( options.name ) \
    or not options.input:
    parser.parse_args( [ '-h' ] )
  motions = {}
  for i in xrange( len( options.transfo ) ):
    motions[ options.name[i] ] = options.transfo[i]
  return ( options.input, options.output, motions )


if __name__ == '__main__':
  gname, outname, motnames = parseOpts( sys.argv )
  #gvol = '/tmp/roivol.arg'
  #c = [ 'AimsGraphConvert', '-i', gname, '-o', gvol, '--volume' ]
  #cs = '"' + string.join( c, '" "' ) + '"'
  #print cs
  #os.system( cs )

  print 'input:', gname
  print 'output:', outname
  print 'transfos:', motnames

  r = aims.Reader()
  g = r.read( gname )
  motions = {}
  for l, m in motnames.items():
    print 'read transform', m
    motions[ l ] = r.read( m )

  graphTransform( g, motions )
  g[ 'filename_base' ] = '*'
  w = aims.Writer()
  w.write( g, outname )
