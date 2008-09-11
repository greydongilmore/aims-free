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

#include <aims/getopt/getopt2.h>
#include <aims/io/reader.h>
#include <aims/io/writer.h>
#include <aims/graph/graphmanip.h>
#include <graph/graph/graph.h>

using namespace aims;
using namespace carto;
using namespace std;

int main( int argc, const char** argv )
{
  try
    {
      AimsApplication	app( argc, argv, 
			     "Merge 2 graphs " 
			     "(cortical folds graphs generally)");
      Reader<Graph>		rg1,rg2;
      Writer<Graph>		wg;
      string key;
      app.addOption( rg1, "-i", "input graph1" );
      app.alias( "-input1", "-i" );
      app.addOption( key, "-k", "matching key" );
      app.alias( "-key", "-k" );
      app.addOption( rg2, "-j", "input graph2" );
      app.alias( "-input2", "-j" );
      app.addOption( wg, "-o", "output (merge) graph [default=input]", 
		     true );
      app.alias( "-output", "-o" );
      app.initialize();

      Graph	*g = rg1.read();
      Graph	*h = rg2.read();
      Graph *m = GraphManip::mergeGraph(key,*g,*h,true );
      
      string	base( wg.fileName() );
      string::size_type	pos = base.rfind( '/' );
      if( pos != string::npos )
	base.erase( 0, pos+1 );
      pos = base.rfind( '.' );
      if( pos != string::npos )
	base.erase( pos, base.length() - pos );
      base += ".data";
      m->setProperty( "filename_base", base );

      if( wg.fileName().empty() )
	wg.setFileName( rg1.fileName() );
      wg.write( *m );

      return EXIT_SUCCESS;
    }

  catch( user_interruption &e )
    {
    }
  catch( exception & e )
    {
      cerr << e.what() << endl;
    }
  return EXIT_FAILURE;
}
