/* Copyright (c) 1995-2005 CEA
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

#include <aims/def/assert.h>
#include <aims/io/argheader.h>
#include <aims/def/general.h>
#include <aims/io/defaultItemR.h>
#include <aims/io/defaultItemW.h>
#include <cartobase/exception/ioexcept.h>

using namespace aims;
using namespace carto;
using namespace std;


string ArgHeader::filename() const
{
  if( _name.length() >= 4 && _name.substr( _name.length() - 4, 4 ) == ".arg" )
    return( _name );
  if( _name.length() >= 8 && 
      _name.substr( _name.length() - 8, 8 ) == ".bundles" )
    return( _name );
  return( _name + ".arg" );
}


void ArgHeader::read( size_t * )
{
  string fileName = filename();
  if( fileName.length() >= 8 && 
      fileName.substr( _name.length() - 8, 8 ) == ".bundles" ) {
    setProperty( "file_type", string( "ARG" ) );
    setProperty( "object_type", string( "Graph" ) );
    setProperty( "data_type", string( "VOID" ) );
    // Read bundles header
    readMinf( fileName );
    // add meta-info to header
    readMinf( fileName + ".minf" );
  } else {
    // ARG format
    ifstream is( fileName.c_str() );
    if( !is )
      io_error::launchErrnoExcept( fileName );
    
    setProperty( "file_type", string( "BUNDLES" ) );
    setProperty( "object_type", string( "Graph" ) );
    setProperty( "data_type", string( "VOID" ) );
    
    // try to read syntactic attribute
    string	l;
    char		c;
    
    do {
      l.erase( 0, l.length() );
      while( !is.eof() ) {
        c = is.get();
        if( c != '\n' && c != '\0' )
          l += c;
        else
          break;
      }
    } while( !is.eof() && ( l.length() == 0 || l[0] == '#' ) );
    if( l.find( "*BEGIN GRAPH" ) == 0 ) {
      unsigned	i = 12;
      while( l.length() > i && ( l[i] == ' ' || l[i] == '\t' ) )
        ++i;
      l.erase( 0, i );
      setProperty( "arg_syntax", l );
    }
    else
      throw wrong_format_error( fileName );
    // add meta-info to header
    readMinf( removeExtension( _name ) + extension() + ".minf" );
  }
}


void ArgHeader::write()
{
  string fileName = filename();
  ofstream os( fileName.c_str() );
  if( !os )
    io_error::launchErrnoExcept( fileName );

// Yann // A vérifier et compléter

  // write meta-info header
  setProperty( "file_type", string( "ARG" ) );
  setProperty( "object_type", string( "Graph" ) );
  setProperty( "data_type", string( "VOID" ) );

  writeMinf( removeExtension( _name ) + extension() + ".minf" );
}


set<string> ArgHeader::extensions() const
{
  set<string>	exts;
  exts.insert( ".arg" );
  exts.insert( ".bundles" );
  return exts;
}
