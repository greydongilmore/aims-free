/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#ifndef AIMS_IO_READER_D_H
#define AIMS_IO_READER_D_H

#include <aims/io/reader.h>
#include <aims/io/fileFormat.h>
#include <aims/io/finder.h>
#include <cartobase/type/string_conversion.h>
#include <cartobase/exception/ioexcept.h>
#include <cartobase/stream/fileutil.h>
#include <set>

#ifdef USE_SOMA_IO
  #include <soma-io/io/reader_d.h>
  #include <soma-io/io/formatdictionary_d.h>
  #define AIMS_INSTANTIATE_READER( T ) \
    namespace aims { \
      template class aims::Reader< T >; \
    } \
    namespace soma { \
      template class soma::FormatDictionary< T >; \
      template class soma::Reader< T >; \
    }
#else
  #define AIMS_INSTANTIATE_READER( T ) \
    namespace aims { \
      template class aims::Reader< T >; \
    }
#endif

namespace aims
{

  template<class T> Reader<T>::Reader()
    : _mode( carto::AllocatorStrategy::Memory )
  {
  }

  template<class T> Reader<T>::Reader( const std::string& filename )
    : _filename( filename ), _mode( carto::AllocatorStrategy::Memory )
  {
  }


  template<class T>
  void Reader<T>::setMode( carto::AllocatorStrategy::MappingMode mode )
  {
    _mode = mode;
    // compatibility with new AllocatorContext
    switch( mode )
      {
      case carto::AllocatorStrategy::Memory:
      case carto::AllocatorStrategy::MAP:
      case carto::AllocatorStrategy::CopyMap:
        _alloccontext.setAccessMode( carto::AllocatorStrategy::InternalModif );
        break;
      case carto::AllocatorStrategy::ReadOnlyMap:
        _alloccontext.setAccessMode( carto::AllocatorStrategy::ReadOnly );
        break;
      case carto::AllocatorStrategy::ReadWriteMap:
        _alloccontext.setAccessMode( carto::AllocatorStrategy::ReadWrite );
        break;
      case carto::AllocatorStrategy::Unallocated:
        _alloccontext.setAccessMode( carto::AllocatorStrategy::NotOwner );
        break;
      default:
        _alloccontext.setAccessMode( carto::AllocatorStrategy::InternalModif );
        break;
      }
  }

  template <typename T>
  void Reader<T>::setAllocatorContext( const carto::AllocatorContext & ac )
  {
    _alloccontext = ac;
  }

  template <typename T>
  const carto::AllocatorContext & Reader<T>::allocatorContext() const
  {
    return _alloccontext;
  }

  template<class T>
  void Reader<T>::setFileName( const std::string &fileName )
  {
    _filename = fileName;
  }

  template<class T>
  void Reader<T>::setOptions( carto::Object options )
  {
    _options = options;
  }

  template<class T>
  carto::Object Reader<T>::options() const
  {
    return _options;
  }

  template<class T>
  carto::Object & Reader<T>::options()
  {
    return _options;
  }

  template<class T>
  bool Reader<T>::read( T & obj, int border, const std::string* format, 
			int frame )
  {
#ifdef USE_SOMA_IO
    // try first soma-io reader (since 2013)
    try{
      // building uri
      std::string uri = _filename;
      if( border != 0 || frame != -1 )
        uri += "?";
      if( border != 0 )
        uri += ( "border=" + carto::toString( border ) );
      if( border != 0 && frame != -1 )
        uri += "&";
      if ( frame != -1 )
        uri += ( "oz=" + carto::toString( frame ) + "&sz=1" );
      
      soma::Reader<T> reader( uri );
      reader.setOptions( _options );
      return reader.read( obj, carto::none() );
    } catch( ... ) {}
    // if it failed, continue with aims reader.
#endif
    
#ifdef AIMS_DEBUG_IO
    std::cout << "Reader<" << carto::DataTypeCode<T>::name() << ">\n";
#endif

    // take care of old-style options
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );
    if( frame >= 0 )
      {
        _options->setProperty( "frame", frame );
        // (for graphs)
        _options->setProperty( "subobjectsfilter", frame );
      }
    if( border > 0 )
      _options->setProperty( "border", border );

    std::set<std::string>		tried;
    std::set<FileFormat<T> *>		triedf;
    FileFormat<T>			*reader;
    std::set<std::string>::iterator	notyet = tried.end();
    typename std::set<FileFormat<T> *>::iterator	notyetf = triedf.end();
    int					excp = 0;
    int					exct = -1;
    std::string				excm;

    if( format )	// priority to format hint
    {
      reader = FileFormatDictionary<T>::fileFormat( *format );
      if( reader )
      {
        try
        {
#ifdef AIMS_DEBUG_IO
          std::cout << "1. try reader " << *format << std::endl;
#endif
          if( reader->read( _filename, obj, _alloccontext, _options ) )
          {
#ifdef AIMS_DEBUG_IO
            std::cout << "1. " << *format << " OK\n";
#endif
            carto::Object h = carto::getObjectHeader( obj );
            if( h )
              Finder::postProcessHeader( h );
            return true;
          }
        }
        catch( std::exception & e )
        {
#ifdef AIMS_DEBUG_IO
          std::cout << "1. failed: " << e.what() << "\n";
#endif
          carto::io_error::keepExceptionPriority( e, excp, exct, excm,
                                                  5 );
        }
        tried.insert( *format );
        triedf.insert( reader );
      }
    }

    std::string                bname = carto::FileUtil::basename( _filename );
    std::string::size_type     pos = bname.find( '.' );
    std::string::size_type     dlen = _filename.length() - bname.length();
    std::string                ext;

    if( pos != std::string::npos )
      ext = _filename.substr( dlen+pos+1, _filename.length() - pos - 1 );

    const std::map<std::string, std::list<std::string> >	& extensions
      = FileFormatDictionary<T>::extensions();

    std::map<std::string, std::list<std::string> >::const_iterator iext
        = extensions.find( ext ),
    eext = extensions.end();
    std::list<std::string>::const_iterator ie, ee;

    while( iext == eext && (pos=bname.find( '.', pos+1 ))!=std::string::npos )
    {
      ext = _filename.substr( dlen+pos+1, _filename.length() - pos - 1 );
      iext = extensions.find( ext );
    }

    // try every matching format until one works
    if( iext != eext )
      for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
        if( tried.find( *ie ) == notyet )
        {
          reader = FileFormatDictionary<T>::fileFormat( *ie );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try
            {
#ifdef AIMS_DEBUG_IO
              std::cout << "2. try reader " << *ie << std::endl;
#endif
              if( reader->read( _filename, obj, _alloccontext, _options ) )
              {
#ifdef AIMS_DEBUG_IO
                std::cout << "2. " << *ie << " OK\n";
#endif
                carto::Object h = carto::getObjectHeader( obj );
                if( h )
                  Finder::postProcessHeader( h );
                return true;
              }
            }
            catch( std::exception & e )
            {
#ifdef AIMS_DEBUG_IO
              std::cout << "2. failed: " << e.what() << "\n";
#endif
              carto::io_error::keepExceptionPriority( e, excp, exct,
                                                      excm );
            }
#ifdef AIMS_DEBUG_IO
            std::cout << "2. unsuccessfully tried " << *ie << std::endl;
#endif
            tried.insert( *ie );
            triedf.insert( reader );
          }
        }

    if( !ext.empty() )
    {
      // not found or none works: try readers with no extension
      iext = extensions.find( "" );

      if( iext != eext )
        for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
          if( tried.find( *ie ) == notyet )
          {
            reader = FileFormatDictionary<T>::fileFormat( *ie );
            if( reader && triedf.find( reader ) == notyetf )
            {
              try
              {
#ifdef AIMS_DEBUG_IO
                std::cout << "3. try reader " << *ie << std::endl;
#endif
                if( reader->read( _filename, obj, _alloccontext, _options ) )
                {
#ifdef AIMS_DEBUG_IO
                  std::cout << "3. " << *ie << " OK\n";
#endif
                  carto::Object h = carto::getObjectHeader( obj );
                  if( h )
                    Finder::postProcessHeader( h );
                  return true;
                }
              }
              catch( std::exception & e )
              {
#ifdef AIMS_DEBUG_IO
                std::cout << "3. failed: " << e.what() << "\n";
#endif
                carto::io_error::keepExceptionPriority( e, excp, exct,
                                                        excm );
              }
              tried.insert( *ie );
              triedf.insert( reader );
            }
          }
      }

    // still not found ? well, try EVERY format this time...
    for( iext=extensions.begin(); iext!=eext; ++iext )
      for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
        if( tried.find( *ie ) == notyet )
        {
          reader = FileFormatDictionary<T>::fileFormat( *ie );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try
            {
#ifdef AIMS_DEBUG_IO
              std::cout << "4. try reader " << *ie << std::endl;
#endif
              if( reader->read( _filename, obj, _alloccontext, _options ) )
              {
#ifdef AIMS_DEBUG_IO
                std::cout << "4. " << *ie << " OK\n";
#endif
                carto::Object h = carto::getObjectHeader( obj );
                if( h )
                  Finder::postProcessHeader( h );
                return true;
              }
            }
            catch( std::exception & e )
            {
#ifdef AIMS_DEBUG_IO
              std::cout << "4. failed: " << e.what() << "\n";
#endif
              carto::io_error::keepExceptionPriority( e, excp, exct,
                                                      excm );
            }
            tried.insert( *ie );
            triedf.insert( reader );
          }
        }

    // still not succeeded, it's hopeless...
    carto::io_error::launchExcept( exct, excm, 
				   _filename + " : no matching format" );
    return( false );
  }


  template<class T>
  T* Reader<T>::read( int border, const std::string* format, int frame )
  {
#ifdef USE_SOMA_IO
    // try first soma-io reader (since 2013)
    try{
      // building uri
      std::string uri = _filename;
      if( border != 0 || frame != -1 )
        uri += "?";
      if( border != 0 )
        uri += ( "border=" + carto::toString( border ) );
      if( border != 0 && frame != -1 )
        uri += "&";
      if ( frame != -1 )
        uri += ( "oz=" + carto::toString( frame ) + "&sz=1" );
      
      soma::Reader<T> reader( uri );
      reader.setOptions( _options );
      return reader.read();
    } catch( ... ) {}
    // if it failed, continue with aims reader.
#endif
    
    // take care of old-style options
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );
    if( frame >= 0 )
      {
        _options->setProperty( "frame", frame );
        // (for graphs)
        _options->setProperty( "subobjectsfilter", frame );
      }
    if( border > 0 )
      _options->setProperty( "border", border );

    std::set<std::string>		tried;
    std::set<FileFormat<T> *>		triedf;
    FileFormat<T>			*reader;
    std::set<std::string>::iterator	notyet = tried.end();
    typename std::set<FileFormat<T> *>::iterator	notyetf = triedf.end();
    T					*obj;
    int					excp = 0;
    int					exct = -1;
    std::string				excm;

    if( format )	// priority to format hint
    {
      reader = FileFormatDictionary<T>::fileFormat( *format );
      if( reader )
      {
        try
        {
          obj = reader->read( _filename, _alloccontext, _options );
          if( obj )
          {
            carto::Object h = carto::getObjectHeader( obj );
            if( h )
              Finder::postProcessHeader( h );
            return( obj );
          }
        }
        catch( std::exception & e )
        {
          carto::io_error::keepExceptionPriority( e, excp, exct, excm,
                                                  5 );
        }
        tried.insert( *format );
        triedf.insert( reader );
      }
    }

    std::string	ext = FileFormatDictionary<T>::fileExtension( _filename );

    const std::map<std::string, std::list<std::string> >	& extensions
        = FileFormatDictionary<T>::extensions();

    std::map<std::string, std::list<std::string> >::const_iterator iext
        = extensions.find( ext ),
    eext = extensions.end();
    std::list<std::string>::const_iterator ie, ee;

    // try every matching format until one works
    if( iext != eext )
      for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
        if( tried.find( *ie ) == notyet )
        {
          reader = FileFormatDictionary<T>::fileFormat( *ie );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try
            {
              obj = reader->read( _filename, _alloccontext, _options );
              if( obj )
              {
                carto::Object h = carto::getObjectHeader( obj );
                if( h )
                  Finder::postProcessHeader( h );
                return( obj );
              }
            }
            catch( std::exception & e )
            {
              carto::io_error::keepExceptionPriority( e, excp, exct,
                                                      excm );
            }
            tried.insert( *ie );
            triedf.insert( reader );
          }
        }

    if( !ext.empty() )
    {
      // not found or none works: try readers with no extension
      iext = extensions.find( "" );

      if( iext != eext )
        for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
          if( tried.find( *ie ) == notyet )
          {
            reader = FileFormatDictionary<T>::fileFormat( *ie );
            if( reader && triedf.find( reader ) == notyetf )
            {
              try
              {
                obj = reader->read( _filename, _alloccontext, _options );
                if( obj )
                {
                  carto::Object h = carto::getObjectHeader( obj );
                  if( h )
                    Finder::postProcessHeader( h );
                  return( obj );
                }
              }
              catch( std::exception & e )
              {
                carto::io_error::keepExceptionPriority( e, excp, exct,
                                                        excm );
              }
              tried.insert( *ie );
              triedf.insert( reader );
            }
          }
    }

    // still not found ? well, try EVERY format this time...
    for( iext=extensions.begin(); iext!=eext; ++iext )
      for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
        if( tried.find( *ie ) == notyet )
        {
          reader = FileFormatDictionary<T>::fileFormat( *ie );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try
            {
              obj = reader->read( _filename, _alloccontext, _options );
              if( obj )
              {
                carto::Object h = carto::getObjectHeader( obj );
                if( h )
                  Finder::postProcessHeader( h );
                return( obj );
              }
            }
            catch( std::exception & e )
            {
              carto::io_error::keepExceptionPriority( e, excp, exct,
                                                      excm );
            }
            tried.insert( *ie );
            triedf.insert( reader );
          }
        }

    // still not succeeded, it's hopeless...
    carto::io_error::launchExcept( exct, excm, 
				   _filename + " : no matching format" );
    return( 0 );
  }

}


#endif
