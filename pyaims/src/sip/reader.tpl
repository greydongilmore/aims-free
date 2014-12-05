
class Reader_%Template1typecode%
{
%TypeHeaderCode
#include <aims/io/reader.h>
%Template1typeinclude%
%Template1sipinclude%
typedef aims::Reader<%Template1% > Reader_%Template1typecode%;
%End

public:
  Reader_%Template1typecode%();
  Reader_%Template1typecode%( const std::string & );
  virtual ~Reader_%Template1typecode%();

%If (SIPTHROW_BUG)
  virtual %Template1PyType% *read( int = 0, const std::string * = 0, int = -1 )
    throw ( ) /Factory, ReleaseGIL/;
%End
%If (!SIPTHROW_BUG)
  virtual %Template1PyType% *read( int = 0, const std::string * = 0, int = -1 )
    throw ( carto::io_error, std::exception ) /Factory/; //, ReleaseGIL/;
%End

  void setFileName( std::string & );
  const std::string & fileName() const;
  void setAllocatorContext( const carto::AllocatorContext & ac );
  const carto::AllocatorContext & allocatorContext() const;
  void setOptions( carto::Object options );
  carto::Object options() const;
};


class Writer_%Template1typecode%
{
%TypeHeaderCode
#include <aims/io/writer.h>
%Template1typeinclude%
%Template1sipinclude%
typedef aims::Writer<%Template1% > Writer_%Template1typecode%;
%End

public:
  Writer_%Template1typecode%();
  Writer_%Template1typecode%( const std::string &,
    carto::Object = carto::none() );
  virtual ~Writer_%Template1typecode%();

%If (SIPTHROW_BUG)
  virtual bool write( const %Template1PyType% &, bool = false, 
                      const std::string * = 0 )
    throw ( ) /Factory, ReleaseGIL/;
%End
%If (!SIPTHROW_BUG)
  virtual bool write( const %Template1PyType% &, bool = false, 
                      const std::string * = 0 )
    throw ( carto::io_error, std::exception ) /Factory/; //, ReleaseGIL/;
%End

  void setFileName( std::string & );
  const std::string & fileName() const;

  virtual std::string writtenObjectType() const;
  virtual std::string writtenObjectDataType() const;
  virtual std::string writtenObjectFullType() const;
};


// --- SOMA IO ---

namespace carto
{

  class FormatDictionary_%Template1typecode%
  {
%TypeHeaderCode
#include <soma-io/io/formatdictionary.h>
%Template1typeinclude%
%Template1sipinclude%
namespace carto
{
  typedef carto::FormatDictionary<%Template1% > FormatDictionary_%Template1typecode%;
}
%End

%TypeCode
#include <cartobase/plugin/plugin.h>
%End

  public:
    static SIP_PYDICT readExtensions();
%MethodCode
  carto::PluginLoader::load();
  sipRes = PyDict_New();
  std::multimap<std::string, std::string>::const_iterator i, e \
    = soma::FormatDictionary<%Template1% >::readExtensions().end();
  std::map<std::string, PyObject *> pymap;
  std::map<std::string, PyObject *>::iterator ipm, epm = pymap.end();
  for( i=soma::FormatDictionary<%Template1% >::readExtensions().begin(); i!=e; ++i )
  {
    ipm = pymap.find(i->first);
    PyObject *p;
    if( ipm == epm )
    {
      p = PyList_New( 0 );
      pymap[i->first] = p;
      ipm = pymap.find(i->first);
      PyDict_SetItem( sipRes, PyString_FromString( i->first.c_str() ), p );
    }
    p = ipm->second;
    PyList_Append( p, PyString_FromString( i->second.c_str() ) );
  }
%End

    static SIP_PYDICT writeExtensions();
%MethodCode
  carto::PluginLoader::load();
  sipRes = PyDict_New();
  std::multimap<std::string, std::string>::const_iterator i, e \
    = soma::FormatDictionary<%Template1% >::writeExtensions().end();
  std::map<std::string, PyObject *> pymap;
  std::map<std::string, PyObject *>::iterator ipm, epm = pymap.end();
  for( i=soma::FormatDictionary<%Template1% >::writeExtensions().begin(); i!=e; ++i )
  {
    ipm = pymap.find(i->first);
    PyObject *p;
    if( ipm == epm )
    {
      p = PyList_New( 0 );
      pymap[i->first] = p;
      ipm = pymap.find(i->first);
      PyDict_SetItem( sipRes, PyString_FromString( i->first.c_str() ), p );
    }
    p = ipm->second;
    PyList_Append( p, PyString_FromString( i->second.c_str() ) );
  }
%End

    static set_STRING readFormats();
    static set_STRING writeFormats();
  };

};


