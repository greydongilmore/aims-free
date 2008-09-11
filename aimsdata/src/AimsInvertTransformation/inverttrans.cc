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
#include <aims/io/motionR.h>
#include <aims/io/motionW.h>
#include <aims/resampling/motion.h>
#include <cartobase/stream/fileutil.h>

using namespace aims;
using namespace carto;
using namespace std;


int main( int argc, const char** argv )
{
  try
    {
      string	r;
      string	w;
      AimsApplication	app( argc, argv, "Inverts an Aims/anatomist "
			     "transformation" );
      app.addOption( r, "-i", "input transformation" );
      app.addOption( w, "-o", "output (inverted) transformation "
		     "(default: <input>_inverted.trm)", true );
      app.initialize();

      Motion		m;
      MotionReader	mr( r );
      mr.read( m );
      m = m.inverse();
      if( w.empty() )
        {
          w = FileUtil::removeExtension( r ) + "_inverted.trm";
        }
      MotionWriter	mw( w );
      mw.write( m );
    }
  catch( user_interruption & )
    {
    }
  catch( exception & e )
    {
      cerr << e.what() << endl;
    }
}

