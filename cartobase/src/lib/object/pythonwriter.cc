/* Copyright (c) 1995-2006 CEA
 *
 *  This software and supporting documentation were developed by
 *      CEA/DSV/SHFJ
 *      4 place du General Leclerc
 *      91401 Orsay cedex
 *      France
 *
 * This software is governed by the CeCILL license version 2 under 
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the 
 * terms of the CeCILL license version 2 as circulated by CEA, CNRS
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
 * knowledge of the CeCILL license version 2 and that you accept its terms.
 */

#include <cartobase/object/pythonwriter.h>
#include <cartobase/object/object_d.h>
#include <cartobase/object/property.h>
#include <cartobase/exception/file.h>
#include <cartobase/exception/ioexcept.h>
#include <cartobase/wip/io/asciidatasourcetraits.h>
#include <cartobase/datasource/filedatasource.h>
#include <cartobase/datasource/streamdatasource.h>
// debug
//#include <iostream>

using namespace std;
using namespace carto;


namespace {

template<typename T> 
void 
genericHelper( const GenericObject & obj, PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  // using GenericObject:: avoids a bug in gcc-2.96
  const T	& x = obj.GenericObject::value<T>();
  AsciiDataSourceTraits<T>::write( ds, x );
}


template<>
void genericHelper<char>( const GenericObject & obj, PythonWriter & w, int, 
                          bool )
{
  DataSource	& ds = *w.dataSource();
  // using GenericObject:: avoids a bug in gcc-2.96
  const char	& x = obj.GenericObject::value<char>();
  AsciiDataSourceTraits<int>::write( ds, x );
}


template<>
void genericHelper<unsigned char>( const GenericObject & obj, 
                                   PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  // using GenericObject:: avoids a bug in gcc-2.96
  const unsigned char	& x = obj.GenericObject::value<unsigned char>();
  AsciiDataSourceTraits<unsigned>::write( ds, x );
}


template<>
void genericHelper<string>( const GenericObject & obj, 
                            PythonWriter & w, int, bool )
{
  w.writeString( *w.dataSource(), obj.getString() );
}


template<>
void genericHelper<double>( const GenericObject & obj, 
                            PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<double>::write( ds, obj.getScalar() );
}


template<>
void genericHelper<float>( const GenericObject & obj,
                           PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<float>::write( ds, (float) obj.getScalar() );
}


template<>
void genericHelper<int16_t>( const GenericObject & obj,
                             PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<int16_t>::write( ds, (int16_t) obj.getScalar() );
}


template<>
void genericHelper<uint16_t>( const GenericObject & obj,
                              PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<uint16_t>::write( ds, (uint16_t) obj.getScalar() );
}


template<>
void genericHelper<int32_t>( const GenericObject & obj,
                             PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<int32_t>::write( ds, (int32_t) obj.getScalar() );
}


template<>
void genericHelper<uint32_t>( const GenericObject & obj,
                              PythonWriter & w, int, bool )
{
  DataSource	& ds = *w.dataSource();
  AsciiDataSourceTraits<uint32_t>::write( ds, (uint32_t) obj.getScalar() );
}


template<typename T> 
void 
genericSequenceHelper( const GenericObject & obj, PythonWriter & w, int ind, 
		       bool writeInternals )
{
  DataSource	& ds = *w.dataSource();
  // using GenericObject:: avoids a bug in gcc-2.96
  const T	& x = obj.GenericObject::value<T>();
  typename T::const_iterator	ix;
  typename T::const_iterator	ex = x.end();
  ValueObject<typename T::value_type>	de;
  bool				first = true;

  ds.putch( '[' );
  for( ix=x.begin(); ix!=ex; ++ix )
    {
      if( first )
	{
	  first = false;
          ds.putch( ' ' );
	}
      else
        {
          ds.putch( ',' );
          ds.putch( ' ' );
        }
      de.getValue() = *ix;
      genericHelper<typename T::value_type>( de, w, ind, writeInternals );
    }
  ds.putch( ' ' );
  ds.putch( ']' );
}


template<typename T> 
void dictHelper( const GenericObject & obj, PythonWriter & w, int indent, 
		 bool writeInternals )
{
  // cout << "dictHelper<" << DataTypeCode<T>::name() << ">\n";

  DataSource				& ds = *w.dataSource();
  const map<T, Object> 
    & x = obj.GenericObject::value<map<T, Object> >();
  typename map<T, Object>::const_iterator	im, em = x.end();
  bool					first = true, hassyntax = false;
  string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind );

  string	synt;
  const SyntaxedInterface *si 
    = obj.GenericObject::getInterface<SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      AsciiDataSourceTraits<string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  for( im=x.begin(); im!=em; ++im )
    {
      if( first )
	first = false;
      else
        {
          ds.putch( ',' );
          ds.putch( sep );
          AsciiDataSourceTraits<string>::write( ds, ind );
        }
      // const_cast: just to avoid referencing a const T
      Object key = Object::reference( const_cast<T &>( im->first ) );
      w.write( *key, indent, "", "", writeInternals );
      AsciiDataSourceTraits<string>::write( ds, " : " );
      w.write( *im->second, indent, "", "", writeInternals );
    }
  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind );
  ds.putch( '}' );
}


template<> 
void dictHelper<string>( const GenericObject & obj, PythonWriter & w, 
			 int indent, bool writeInternals )
{
  DataSource				& ds = *w.dataSource();
  const IterableInterface 
    & y = *obj.getInterface<IterableInterface>();
  Object				im;

  bool					first = true, hassyntax = false;
  string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind );

  string	synt;
  const SyntaxedInterface	*si = obj.getInterface<SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      AsciiDataSourceTraits<string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  string	key;
  im=y.objectIterator();
  while( im->isValid() )
    {
      key = im->key();
      if( ( writeInternals || !w.isInternal( "", key ) ) 
          && ( key != "__syntax__" || !hassyntax ) )
        {
          if( first )
            first = false;
          else
            {
              ds.putch( ',' );
              ds.putch( sep );
              AsciiDataSourceTraits<string>::write( ds, ind );
            }
          w.writeString( ds, key );
          AsciiDataSourceTraits<string>::write( ds, " : " );
          w.write( *im->currentValue(), indent, "", key, writeInternals );
        }
      im->next();
    }

  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind2 );
  ds.putch( '}' );
}


void rcDictHelper( const GenericObject & obj, PythonWriter & w, int indent, 
		   bool writeInternals )
{
  DataSource				& ds = *w.dataSource();
  const map<Object, Object>	& x 
    = obj.value<map<Object, Object> >();
  map<Object, Object>::const_iterator	im, em = x.end();
  bool					first = true, hassyntax = false;
  string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind );

  string	synt;
  const SyntaxedInterface	*si = obj.getInterface<SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      AsciiDataSourceTraits<string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  for( im=x.begin(); im!=em; ++im )
    {
      if( first )
	first = false;
      else
        {
          ds.putch( ',' );
          ds.putch( sep );
          AsciiDataSourceTraits<string>::write( ds, ind );
        }
      w.write( *im->first, indent, "", "", writeInternals );
      AsciiDataSourceTraits<string>::write( ds, " : " );
      w.write( *im->second, indent, "", "", writeInternals );
    }

  ds.putch( sep );
  AsciiDataSourceTraits<string>::write( ds, ind2 );
  ds.putch( '}' );
}


void listHelper( const GenericObject & obj, PythonWriter & w, int indent, 
		 bool writeInternals )
{
  DataSource			& ds = *w.dataSource();
  Object			it = obj.objectIterator();
  bool				first = true;

  ds.putch( '[' );
  ++indent;
  for( it=obj.objectIterator(); it->isValid(); it->next() )
    {
      if( first )
	first = false;
      else
        ds.putch( ',' );
      ds.putch( ' ' );
      w.write( *it->currentValue(), indent, "", "", writeInternals );
    }
  ds.putch( ' ' );
  ds.putch( ']' );
}

} // namespace (internal linkage)

// -----------

PythonWriter::PythonWriter( const std::string& filename, 
			    const SyntaxSet& rules, const HelperSet& helpers )
  : _rules(rules), 
    _datasource( new FileDataSource( filename, 0, DataSource::Write ) ), 
    _singleLine( false ), _catchFunction( 0 )
{
  init( helpers );
  _datasource->open( DataSource::Write );
}


PythonWriter::PythonWriter( const SyntaxSet& rules, const HelperSet& helpers )
  : _rules(rules), _datasource(), _singleLine( false ), _catchFunction( 0 )
{
  init( helpers );
}


PythonWriter::PythonWriter( rc_ptr<DataSource> ds, 
			    const SyntaxSet& rules, const HelperSet& helpers )
  : _rules(rules), _datasource( ds ), _singleLine( false ), _catchFunction( 0 )
{
  init( helpers );
}


PythonWriter::~PythonWriter()
{
  close();
}


void PythonWriter::init( const HelperSet & helpers )
{
  _helpers = helpers;

  if( _helpers.find( "double" ) == _helpers.end() )
    _helpers[ "double" ] = _helpers[ "DOUBLE" ] = &genericHelper<double>;
  {
    ValueObject<double>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<double>;
  }
  if( _helpers.find( "float" ) == _helpers.end() )
    _helpers[ "float" ] = _helpers[ "FLOAT" ] = &genericHelper<float>;
  {
    ValueObject<float>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<float>;
  }
  if( _helpers.find( "int" ) == _helpers.end() )
    _helpers[ "int" ] = _helpers[ "S32" ] = &genericHelper<int>;
  {
    ValueObject<int>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<int>;
  }
  if( _helpers.find( "uint" ) == _helpers.end() )
    _helpers[ "uint" ] = _helpers[ "U32" ] = &genericHelper<unsigned int>;
  {
    ValueObject<unsigned int>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<unsigned int>;
  }
  if( _helpers.find( "short" ) == _helpers.end() )
    _helpers[ "short" ] = _helpers[ "S16" ] = &genericHelper<short>;
  {
    ValueObject<short>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<short>;
  }
  if( _helpers.find( "ushort" ) == _helpers.end() )
    _helpers[ "ushort" ] = _helpers[ "U16" ] = &genericHelper<unsigned short>;
  {
    ValueObject<unsigned short>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<unsigned short>;
  }
  if( _helpers.find( "char" ) == _helpers.end() )
    _helpers[ "char" ] = _helpers[ "S8" ] = &genericHelper<char>;
  {
    ValueObject<char>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<char>;
  }
  if( _helpers.find( "uchar" ) == _helpers.end() )
    _helpers[ "uchar" ] = _helpers[ "U8" ] = &genericHelper<unsigned char>;
  {
    ValueObject<unsigned char>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<unsigned char>;
  }
  if( _helpers.find( "bool" ) == _helpers.end() )
    _helpers[ "bool" ] = &genericHelper<bool>;
  {
    ValueObject<bool>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<bool>;
  }
  if( _helpers.find( "string" ) == _helpers.end() )
    _helpers[ "string" ] = &genericHelper<string>;
  {
    ValueObject<string>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<string>;
  }
  if( _helpers.find( "dictionary" ) == _helpers.end() )
    _helpers[ "dictionary" ] = &dictHelper<string>;
  {
    ValueObject<Dictionary>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &dictHelper<string>;
  }
  {
    ValueObject<PropertySet>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &dictHelper<string>;
  }
  {
    ValueObject<map<int, Object> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &dictHelper<int>;
  }
  {
    ValueObject<map<float, Object> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &dictHelper<float>;
  }
  {
    ValueObject<map<Object, Object> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &rcDictHelper;
  }
  if( _helpers.find( "list" ) == _helpers.end() )
    _helpers[ "list" ] = &listHelper;
  {
    ValueObject<ObjectVector>	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &listHelper;
  }
  if( _helpers.find( "int_vector" ) == _helpers.end() )
    _helpers[ "int_vector" ] = &genericSequenceHelper<vector<int> >;
  {
    ValueObject<vector<int> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericSequenceHelper<vector<int> >;
  }
  if( _helpers.find( "uint_vector" ) == _helpers.end() )
    _helpers[ "uint_vector" ] = &genericSequenceHelper<vector<unsigned int> >;
  {
    ValueObject<vector<unsigned int> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericSequenceHelper<vector<unsigned int> >;
  }

  if( _helpers.find( "float_vector" ) == _helpers.end() )
    _helpers[ "float_vector" ] = &genericSequenceHelper<vector<float> >;
  {
    ValueObject<vector<float> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericSequenceHelper<vector<float> >;
  }
  if( _helpers.find( "string_vector" ) == _helpers.end() )
    _helpers[ "string_vector" ] = &genericSequenceHelper<vector<string> >;
  {
    ValueObject<vector<string> >	x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericSequenceHelper<vector<string> >;
  }
  // ### remove after everything has been moved to intN_t/uintN_t  
  if( _helpers.find( "long" ) == _helpers.end() )
    _helpers[ "long" ] = _helpers[ "LONG" ] = &genericHelper<long>;
  {
    ValueObject<long>	   x;
    if( _helpers.find( x.type() ) == _helpers.end() )
      _helpers[ x.type() ] = &genericHelper<long>;
  }
}


void PythonWriter::writeString( DataSource & ds, string x )
{
  unsigned	i, n = x.length();
  for( i=0; i!=n; ++i )
    switch( x[i] )
      {
      case '\'':
      case '\\':
        x.insert( i, "\\" );
        ++n;
        ++i;
        break;
      case '\n':
        x.replace( i, 1, "\\n" );
        ++n;
        ++i;
        break;
      default:
        break;
      }
  ds.putch( '\'' );
  AsciiDataSourceTraits<string>::write( ds, x );
  ds.putch( '\'' );
}


void
PythonWriter::open( const string& filename )
{
  _datasource = rc_ptr<DataSource>
    ( new FileDataSource( filename, 0, DataSource::Write ) );
  _datasource->open( DataSource::Write );

  // are files opened?
  if( !_datasource->isOpen() )
    io_error::launchErrnoExcept( _datasource->url() );
}


void
PythonWriter::close()
{
  _datasource.reset( 0 );
}


void PythonWriter::attach( ostream & s )
{
  _datasource.reset( new OStreamDataSource( s ) );
}


string
PythonWriter::name() const
{
  return _datasource->url();
}


bool
PythonWriter::operator!() const
{
  return !_datasource || !_datasource->isOpen();
}


bool
PythonWriter::is_open() const
{
  return _datasource && _datasource->isOpen();
}


void PythonWriter::write( const Object & object, bool writeInternals, 
			  bool writevariable )
{
  write( *object, writeInternals, writevariable );
}


void PythonWriter::write( const GenericObject & object, bool writeInternals, 
			  bool writevariable )
{
  if( !_datasource )
    throw file_error( "no IO source\n" );
  if( !_datasource->isOpen() )
    _datasource->open( DataSource::Write );
  if( writevariable )
    AsciiDataSourceTraits<string>::write( *_datasource, "attributes = " );
  write( object, 1, "", "", writeInternals );
  if( !_singleLine )
    _datasource->putch( '\n' );
}


bool PythonWriter::isInternal( const string & syntax, const string & semantic )
{
  if( !semantic.empty() )
    {
      SyntaxSet::const_iterator	is;
      if( syntax.empty() )
	is = _rules.find( "__generic__" );
      else
	is = _rules.find( syntax );
      if( is != _rules.end() )
	{
	  Syntax::const_iterator	ie = is->second.find( semantic );
	  if( ie != is->second.end() 
	      && ie->second.internal )
	    return( true );
	}
    }
  return( false );
}


void PythonWriter::write( const GenericObject & object, int indent, 
			  const string & syntax, const string & semantic, 
			  bool writeInternals )
{
  try
  {
    if( !is_open() )
      throw stream_fail_error( "cannot write", name() );

    if( &object == 0 )	// "None" object
      {
        AsciiDataSourceTraits<string>::write( *dataSource(), "None" );
        if( !is_open() )
          io_error::launchErrnoExcept();
        return;
      }

    string	type = object.type();
    if( type == "object" )
      {
        // Object: de-reference it
        write( *object.value<Object>(), indent, syntax, semantic, 
              writeInternals );
        return;
      }
    // cout << "type: " << type << endl;

    //	try to find type in syntax if semantic / syntaxic atts are provided
    if( !semantic.empty() )
      {
        SyntaxSet::const_iterator	is;
        if( syntax.empty() )
          is = _rules.find( "__generic__" );
        else
          is = _rules.find( syntax );
        if( is != _rules.end() )
          {
            Syntax::const_iterator	ie = is->second.find( semantic );
            if( ie != is->second.end() )
              {
                /* cout << "syntax found, syntax: " << syntax << ", semantic: "
                  << semantic << " -> " << type << endl; */
                type = ie->second.type;
              }
          }
      }

    //	try to find type in helpers
    HelperSet::const_iterator	ih = _helpers.find( type );
    if( ih != _helpers.end() )
      {
        (ih->second)( object, *this, indent, writeInternals );
        if( !is_open() )
          io_error::launchErrnoExcept();
        return;	// OK
      }

    else if( object.isScalar() )
      type = "double";
    if( object.isString() )
      type = "string";
    else if( object.isDictionary() )
      type = "dictionary";
    else if( object.isIterable() )
      type = "list";
    // cout << "type fallback for " << object.type() << ": " << type << endl;

    ih = _helpers.find( type );
    if( ih != _helpers.end() )
      {
        (ih->second)( object, *this, indent, writeInternals );
        if( !is_open() )
          io_error::launchErrnoExcept();
        return;	// OK
      }

    // no helper found
    throw runtime_error( string( "Can't write data of type " ) + type
                        + " in " + name() );
  }
  catch( exception & e )
  {
    if( _catchFunction )
      _catchFunction( *this, e, object );
    else
      throw;
  }
}


void PythonWriter::setCatchFunction( CatchFunction f )
{
  _catchFunction = f;
}

INSTANTIATE_GENERIC_OBJECT_TYPE( PythonWriter::Helper )
INSTANTIATE_GENERIC_OBJECT_TYPE( PythonWriter::HelperSet )
