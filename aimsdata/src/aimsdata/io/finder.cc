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

// activate deprecation warning
#ifdef AIMSDATA_CLASS_NO_DEPREC_WARNING
#undef AIMSDATA_CLASS_NO_DEPREC_WARNING
#endif

/*
 *  Finder class
 */
#include <aims/io/finder.h>
#include <aims/io/finderFormats.h>
#include <aims/data/header.h>
#include <aims/io/spmattribconverter.h>
#include <cartobase/plugin/plugin.h>
#include <cartobase/exception/ioexcept.h>
#include <cartobase/stream/fileutil.h>
#include <cartobase/type/typetraits.h>
#include <cartobase/thread/mutex.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/filedatasource.h>
#include <cartodata/io/carto2aimsheadertranslator.h>
#include <map>
#include <set>
#include <iostream>
//#define AIMS_DEBUG_IO
using namespace aims;
using namespace carto;
using namespace std;


namespace aims
{
  struct Finder_Private
  {
    ~Finder_Private();

    map<string, FinderFormat *>	formats;
    map<string, list<string> >	extensions;
  };

  namespace internal
  {
    //defined in reader_d.cc
    void keep_exception_priority( std::exception & e, int & prio, int & type,
				  std::string & message );
  }
}


Finder_Private::~Finder_Private()
{
  map<string, FinderFormat *>::iterator	im, em = formats.end();
  for( im=formats.begin(); im!=em; ++im )
    delete im->second;
}


unique_ptr<Finder_Private>	Finder::pd;


namespace
{
  Mutex _finder_mutex( Mutex::Recursive );
}


void Finder::initPrivate()
{
  _finder_mutex.lock();
  if( !pd.get() )
    {
      pd.reset( new Finder_Private );
      vector<string>	ext;
      ext.push_back( "img" );
      ext.push_back( "hdr" );
      registerFormat( "SPM", new FinderSpmFormat, ext );
#ifdef VIDA_LIB
      ext.clear();
      ext.push_back( "vimg" );
      ext.push_back( "vinfo" );
      ext.push_back( "vhdr" );
      registerFormat( "VIDA", new FinderVidaFormat, ext );
#endif
#ifdef ECAT_LIB
      ext.clear();
      ext.push_back( "v" );
      ext.push_back( "i" );
      ext.push_back( "p" );
      registerFormat( "ECAT", new FinderEcatFormat, ext );
#endif
#ifdef AIMS_JPEG
      ext.clear();
      ext.push_back( "jpg" );
      registerFormat( "JPEG", new FinderJpegFormat, ext );
#endif
      ext.clear();
      ext.push_back( "mesh" );
      registerFormat( "MESH", new FinderMeshFormat, ext );
      ext.clear();
      ext.push_back( "tri" );
      registerFormat( "TRI", new FinderTriFormat, ext );
      ext.clear();
      ext.push_back( "obj" );
      registerFormat( "MNI_OBJ", new FinderMniObjFormat, ext );
      ext.clear();
      ext.push_back( "obj" );
      ext.push_back( "OBJ" );
      registerFormat( "WAVEFRONT", new FinderWavefrontFormat, ext );
      ext.clear();
      ext.push_back( "tex" );
      registerFormat( "TEX", new FinderTexFormat, ext );
      ext.clear();
      ext.push_back( "bck" );
      registerFormat( "BCK", new FinderBckFormat, ext );
      ext.clear();
      ext.push_back( "arg" );
      registerFormat( "ARG", new FinderGraphFormat, ext );
      ext.clear();
      ext.push_back( "bundles" );
      ext.push_back( "trk" );
      ext.push_back( "tck" );
      registerFormat( "BUNDLES", new FinderGraphFormat, ext );
      ext.clear();
      ext.push_back( "trm" );
      registerFormat( "TRM", new FinderTrmFormat, ext );
      ext.clear();
      ext.push_back( "hie" );
      registerFormat( "HIE", new FinderHieFormat, ext );
      // at end: formats without an extension
      ext.clear();
      ext.push_back( "" );
      registerFormat( "GENESIS", new FinderGenesisFormat, ext );
#ifdef DICOM_LIB
      ext.clear();
      ext.push_back( "" );
      registerFormat( "DICOM", new FinderDicomFormat, ext );
#endif
      ext.clear();
      ext.push_back( "imas" );
      registerFormat( "IMASPARSE", new FinderImasFormat, ext );
    }
  _finder_mutex.unlock();
}


Finder::Finder() : _state( Unchecked ), _errorcode( -1 )
{
  initPrivate();
}


Finder::~Finder()
{
}


void Finder::registerFormat( const string & fmtid, FinderFormat* format,
                             const vector<std::string> & extensions,
                             const string & before )
{
  initPrivate();

  FinderFormat	*oldr = finderFormat( fmtid );
  delete oldr;
  pd->formats[ fmtid ] = format;

  vector<string>::const_iterator	ie, ee = extensions.end();
  list<string>::iterator  ie2, ee2;
  for( ie=extensions.begin(); ie!=ee; ++ie )
  {
    list<string>  & ext = pd->extensions[ *ie ];
    if( before.empty() )
      ext.push_back( fmtid );
    else
    {
      for( ie2=ext.begin(), ee2=ext.end(); ie2!=ee2; ++ie2 )
        if( *ie2 == before )
          break;
      ext.insert( ie2, fmtid );
    }
  }
}


void Finder::unregisterFormat( const string & fmtid )
{
  if( !pd )
    return;

  map<string, FinderFormat *>::const_iterator	i
    = pd->formats.find( fmtid );
  if( i != pd->formats.end() )
  {
    FinderFormat *fmt = i->second;
    pd->formats.erase( i );
    // and actually delete it
    delete fmt;
  }
  map<string, list<string> >::iterator ie = pd->extensions.begin(),
    je, ee = pd->extensions.end();
  list<string>::iterator il, jl, el;

  while( ie!=ee )
  {
    il=ie->second.begin();
    el=ie->second.end();
    je = ie;
    ++ie;
    while( il!=el )
      if( *il == fmtid )
      {
        jl = il;
        ++il;
        je->second.erase( jl );
      }
      else
        ++il;
    if( je->second.empty() )
      pd->extensions.erase( je );
  }
}


FinderFormat* Finder::finderFormat( const string & format )
{
  initPrivate();

  map<string, FinderFormat *>::const_iterator	i
    = pd->formats.find( format );
  if( i == pd->formats.end() )
    return( 0 );
  return( (*i).second );
}


bool Finder::check( const string& filename_uri )
{
#ifdef AIMS_DEBUG_IO
  cout << "FINDER:: check( " << filename_uri << " )\n";
#endif
  static bool plugs = false;
  if( !plugs )
    {
      plugs = true;
      PluginLoader::load();
    }

  initPrivate();
  _state = Unchecked;

  // try using DataSourceInfo first (new system 2005)
  #ifdef AIMS_DEBUG_IO
  cout << "FINDER:: trying check " << filename_uri
    << " through DataSourceInfoLoader... " << endl;
  #endif
  Object h;
  string somaio_ext;
  try
  {
    // try first 2 passes
    DataSourceInfoLoader dsil;
    rc_ptr<DataSource> ds( new FileDataSource( filename_uri ) );
    DataSourceInfo dsi = dsil.check( DataSourceInfo( ds ),
                                      carto::none(), 1, 2 );
    h = dsi.header();
    somaio_ext = dsi.identifiedFileExtension();
  }
  catch( ... )
  {
  }
  bool dsok = !h.isNone();
  if( dsok )
  {
    _state = Ok;

    Carto2AimsHeaderTranslator	t;
    t.translate( h );

    string	x;
    h->getProperty( "object_type", x );
    setObjectType( x );
    x.clear();
    h->getProperty( "data_type", x );
    setDataType( x );
    vector<string>	vt;
    vt.push_back( x );
    h->getProperty( "possible_data_types", vt );
    setPossibleDataTypes( vt );
    x.clear();
    h->getProperty( "file_type", x );
    setFormat( x );

    PythonHeader	*ph = new PythonHeader;
    ph->copyProperties( h );
    setHeader( ph );

    #ifdef AIMS_DEBUG_IO
    cout << "FINDER:: DataSourceInfo worked for " << filename_uri
      << " with extension: " << somaio_ext << endl;
    #endif

    /* Not returning yet, even if the file was identified by Soma-IO:
        Some formats are more specialized in Aims (typically: GIFTI is XML,
        XML is recognized by Soma-IO, but we actually want the GIFTI reader
        in, Aims.

        So we go on to check if a longer file extension is matching in the
        ones registered in Aims, which could be "longer" and (euristically)
        more appropriate.
    */
  }
  #ifdef AIMS_DEBUG_IO
  cout << "FINDER:: for " << filename_uri << ", trying through Finder... "
       << endl;
  #endif

  _errorcode = -1;
  _errormsg.clear();

  // remove query-string part or URI
  string filename = carto::FileUtil::uriFilename( filename_uri );

  //	find filename extension
  string                bname = FileUtil::basename( filename );
  string::size_type	pos = bname.find( '.' );
  string::size_type     dlen = filename.length() - bname.length();
  string		ext;
  int			excp = 0;

  if( pos != string::npos )
    ext = filename.substr( dlen+pos+1, filename.length() - pos - 1 );

#ifdef AIMS_DEBUG_IO
  cout << "FINDER:: ext : " << ext << endl;
#endif
  //	Check compatible formats
  set<string>			tried;
  FinderFormat			*reader;
  set<string>::iterator	notyet = tried.end();

  map<string, list<string> >::const_iterator iext
      = pd->extensions.find( ext ),
  eext = pd->extensions.end();

  // find longest matching ext
  while( iext == eext && (pos=bname.find( '.', pos+1 ))!=string::npos )
  {
    ext = filename.substr( dlen+pos+1, filename.length() - pos - 1 );
    iext = pd->extensions.find( ext );
  }
  if( iext != eext )
  {
#ifdef AIMS_DEBUG_IO
    cout << "best extension: " << iext->first << endl;
#endif
    ext = iext->first;
  }
  if( _state == Ok && ( iext == eext
      || iext->first.length() <= somaio_ext.length() ) )
  {
    // Soma-IO has recognized the format, and none in aims matches a longer
    // extension, so let's go with Soma-IO.
#ifdef AIMS_DEBUG_IO
    cout << "No better match in AIMS - taking Soma-IO format.\n";
#endif
    return true;
  }

  list<string>::const_iterator	ie, ee;
  if( iext != eext )
  {
    // try every matching format until one works
    for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
      if( tried.find( *ie ) == notyet )
        {
#ifdef AIMS_DEBUG_IO
          cout << "FINDER:: trying " << *ie << " for " << filename << "...\n";
#endif
          reader = finderFormat( *ie );
          if( reader )
            {
              try
                {
                  if( reader->check( filename, *this ) )
                    {
                      _state = Ok;
                      postProcessHeader( _header );
                      return( true );
                    }
                }
              catch( exception & e )
                {
                  io_error::keepExceptionPriority( e, excp, _errorcode,
                                                  _errormsg );
                }
              tried.insert( *ie );
            }
        }
  }

#ifdef AIMS_DEBUG_IO
  cout << "FINDER:: not found yet... pass2...\n";
#endif
  if( !ext.empty() )
    {
      // not found or none works: try readers with no extension
      iext = pd->extensions.find( "" );

      if( iext != eext )
        for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
          if( tried.find( *ie ) == notyet )
            {
#ifdef AIMS_DEBUG_IO
              cout << "FINDER:: pass2, trying " << *ie << "...\n";
#endif
              reader = finderFormat( *ie );
              if( reader )
                {
                  try
                    {
                      if( reader->check( filename, *this ) )
                        {
                          _state = Ok;
                          postProcessHeader( _header );
                          return( true );
                        }
                    }
                  catch( exception & e )
                    {
                      io_error::keepExceptionPriority( e, excp, _errorcode,
                                                      _errormsg );
                    }
                  tried.insert( *ie );
                }
            }
    }

#ifdef AIMS_DEBUG_IO
  cout << "FINDER:: not found yet... pass3...\n";
#endif
  // still not found ? well, try EVERY format this time...
  for( iext=pd->extensions.begin(); iext!=eext; ++iext )
    for( ie=iext->second.begin(), ee=iext->second.end(); ie!=ee; ++ie )
      if( tried.find( *ie ) == notyet )
      {
        reader = finderFormat( *ie );
        if( reader )
          {
#ifdef AIMS_DEBUG_IO
            cout << "FINDER:: pass3, trying " << *ie << "...\n";
#endif
            try
              {
                if( reader->check( filename, *this ) )
                  {
                    _state = Ok;
                    postProcessHeader( _header );
                    return( true );
                  }
              }
            catch( exception & e )
              {
                io_error::keepExceptionPriority( e, excp, _errorcode,
                                                _errormsg );
              }
            tried.insert( *ie );
          }
      }

  // try pass 3
  try {
    Object h;
    DataSourceInfoLoader dsil;
    rc_ptr<DataSource> ds( new FileDataSource( filename ) );
    DataSourceInfo dsi = dsil.check( DataSourceInfo( ds ),
                                     carto::none(), 3, 3 );
    h = dsi.header();
    bool dsok = !h.isNone();
    if( dsok )
    {
      _state = Ok;

      Carto2AimsHeaderTranslator  t;
      t.translate( h );

      string  x;
      h->getProperty( "object_type", x );
      setObjectType( x );
      x.clear();
      h->getProperty( "data_type", x );
      setDataType( x );
      vector<string>  vt;
      vt.push_back( x );
      h->getProperty( "possible_data_types", vt );
      setPossibleDataTypes( vt );
      x.clear();
      h->getProperty( "file_type", x );
      setFormat( x );

      PythonHeader  *ph = new PythonHeader;
      ph->copyProperties( h );
      setHeader( ph );

      #ifdef AIMS_DEBUG_IO
      cout << "FINDER:: DataSourceInfo worked" << endl;
      #endif
      if( filename.substr( filename.length() - 4, 4 ) != ".gii" )
        // bidouille to let the gifti reader work (it's both gifti and XML)
        return true;
    }
  } catch( ... ) {}

#ifdef AIMS_DEBUG_IO
  cout << "FINDER:: not found at all, giving up\n";
#endif
  // still not succeeded, it's hopeless...

  if( dsok )
    // well, the datasource was OK (exception for .gii and longer
    // formats extensions)
    return true;

  _state = Error;
  if( _errorcode < 0 )
    {
      _errorcode = 0;
      _errormsg = filename + " : could not identify format";
    }
  //launchException();
  return false;
}


const Header* Finder::header() const
{
  return static_cast<const PythonHeader *>( _header.get() );
}


void Finder::setHeader( Header* hdr )
{
  if( !dynamic_cast<PythonHeader *>( hdr) )
    throw runtime_error( "Finder::setHeader: non-generic Header is not "
                         "supported anymore.");
  _header.reset( static_cast<PythonHeader *>( hdr ) );
}


void Finder::setHeader( Object hdr )
{
  if( !dynamic_cast<PythonHeader *>( hdr.get() ) )
  {
    Object phdr( new PythonHeader );
    phdr->copyProperties( hdr );
    hdr = phdr;
  }
  _header = hdr;
}


void Finder::launchException()
{
  /* cout << "Finder::launchException\n";
  cout << "code: " << _errorcode << ", state: " << (int) _state
  << ", msg: " << _errormsg << endl; */
  if( _state != Error || _errorcode < 0 )
    return;	// no error (?)
  string	defmsg;
  if( _errormsg.empty() )
    defmsg = "could not identify format";
  io_error::launchExcept( _errorcode, _errormsg, defmsg );
}


void Finder::postProcessHeader( carto::Object hdr )
{
  initPrivate();
  spmAttributesConverter( *hdr );
}


set<string> Finder::extensions( const string & format )
{
  initPrivate();
  set<string> exts;
  map<string, list<string> >::const_iterator ie, ee = pd->extensions.end();
  list<string>::const_iterator ifo, efo;
  for( ie=pd->extensions.begin(); ie!=ee; ++ie )
    for( ifo=ie->second.begin(), efo=ie->second.end(); ifo!=efo; ++ifo )
      if( *ifo == format )
      {
        exts.insert( ie->first );
        break;
      }
  return exts;
}

