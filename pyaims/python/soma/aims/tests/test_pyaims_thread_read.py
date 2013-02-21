#!/usr/bin/env python

import threading
from soma import aims
import os, sys
from optparse import OptionParser
import threading


def aims_test_thread_read( filenames ):
  class Loadfile( object ):
      def __init__( self, filename, lock, objlist ):
          self._filename = filename
          self.lock = lock
          self.objlist = objlist
      def __call__( self ):
          print 'reading %s...' % self._filename
          obj = aims.read( self._filename )
          print 'read %s: %s' % ( self._filename, str(type(obj)) )
          self.lock.acquire()
          self.objlist.append( obj )
          self.lock.release()

  aims.carto.PluginLoader.load() # do this once in main thread

  threads = []
  lock = threading.RLock()
  objlist = []

  for fname in filenames:
    thread = threading.Thread( target=Loadfile( fname, lock, objlist ) )
    thread.start()
    threads.append( thread )

  for thread in threads:
    thread.join()

  print 'finished. Read %d / %d objects.' % \
    ( len( objlist ), len( filenames ) )
  nmissing = len( filenames ) - len( objlist )
  if nmissing != 0:
    print 'Not all objects were loaded, %d missing.' % nmissing
    raise RuntimeError( 'Not all objects were loaded, %d missing.' % nmissing )


if __name__ == '__main__':

  parser = OptionParser( description='Perform tests of threaded concurrent loading of aims objects in pyaims' )
  parser.add_option( '-i', '--input', dest='infiles',
    help='files to be read concurrently', action='append', default=[] )
  parser.add_option( '-n', '--number', dest='number',
    help='number of times each file should be read at the same time. Default: 30 if one input filename, 1 otherwise', default=0 )

  options, args = parser.parse_args()

  filenames = options.infiles + args
  if len( filenames ) == 0:
    print 'no input files.'
    parser.parse_args( [ '-h' ] )
  if options.number == 0:
    if len( filenames ) == 1:
      num = 30
    else:
      num = 1
  filenames = filenames * num


  #import libxml2
  #libxml2.newTextReaderFilename( '/tmp/ra_head.gii.minf' )
  #import xml.parsers.expat
  #open( '/tmp/xml.xml', 'w' ).write( '<?xml version="1.0" encoding="utf-8" ?><grop></grop>' )
  #p = xml.parsers.expat.ParserCreate()
  #p.ParseFile( open( '/tmp/xml.xml' ) )

  aims_test_thread_read( filenames )

