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

/*
 *  Image flip
 */
#include <aims/io/process.h>
#include <aims/io/reader.h>
#include <aims/io/writer.h>
#include <aims/io/finder.h>
#include <aims/getopt/getopt2.h>
#include <aims/utility/flip.h>

using namespace aims;
using namespace carto;
using namespace std;


template<class T>
static bool flip( Process & p, const string & filename, Finder & f );

class Flip : public Process
{
public:
  Flip( const string & fout, const string& mode );

  template<class T> 
  bool flip_m( VolumeRef<T> & data, const string & filename, Finder & f );

private:
  string	fileout;
  string        mode ;
};

Flip::Flip( const string & fout, const string& mm )
	      : Process(), fileout( fout ), mode (mm)
{
  registerProcessType( "Volume", "U8", &flip<uint8_t> );
  registerProcessType( "Volume", "S8", &flip<int8_t> );
  registerProcessType( "Volume", "U16", &flip<uint16_t> );
  registerProcessType( "Volume", "S16", &flip<int16_t> );
  registerProcessType( "Volume", "S32", &flip<int32_t> );
  registerProcessType( "Volume", "FLOAT", &flip<float> );
  registerProcessType( "Volume", "DOUBLE", &flip<double> );
  registerProcessType( "Volume", "RGB", &flip<AimsRGB> );
  registerProcessType( "Volume", "RGBA", &flip<AimsRGBA> );
}

template<class T> 
bool flip( Process & p, const string & filename, Finder & f )
{
  Flip	& dp = (Flip &) p;
  VolumeRef<T> data;
  return( dp.flip_m( data, filename, f ) );
}


template<class T> 
bool Flip::flip_m( VolumeRef<T> & data, const string & filename, Finder & f )
{
  Reader<Volume<T> >	reader( filename );
  string	format = f.format();
  data.reset( reader.read( 1, &format ) );
  if( !data.get() )
    return( false );
  VolumeRef<T> outimage ;
  
  AimsFlip<T> flip ;
  
  if( mode == "XX" )
    outimage = flip.doXX(data);
  else if( mode == "YY" )
      outimage = flip.doYY(data);
  else if( mode == "ZZ" )
      outimage = flip.doZZ(data);
  else if( mode == "XXYY" )
      outimage = flip.doXXYY(data);
  else if( mode == "XXZZ" )
      outimage = flip.doXXZZ(data);
  else if( mode == "YYZZ" )
      outimage = flip.doYYZZ(data);
  else if( mode == "XXYYZZ" )
      outimage = flip.doXXYYZZ(data);
  else if( mode == "XY" )
      outimage = flip.doXY(data);
  else if( mode == "XZ" )
      outimage = flip.doXZ(data);
  else if( mode == "YZ" )
      outimage = flip.doYZ(data);
  else
      throw runtime_error("Wrong mode") ;

  Writer< VolumeRef<T> > writer( fileout );
  return( writer.write( outimage ) );
}


int main( int argc, const char **argv )
{
  try
    {
      string filein,fileout, mode;

      AimsApplication app(argc, argv, "Image flip" ) ;
      app.addOption( filein, "-i", "input image to flip" ) ;
      app.alias( "--input", "-i" );
      app.addOption( fileout, "-o", "output flipped image" ) ;
      app.alias( "--output", "-o" );
      app.addOption( mode, "-m", "flip image XX, YY, ZZ, XXYY, XXZZ, YYZZ, XXYYZZ, or flip axes : XY, XZ, YZ ") ;
  
      app.initialize() ;

      Flip	proc( fileout, mode );
      if( !proc.execute( filein ) )
        {
          cerr << "Couldn't process\n";
          return( 1 );
        }

    }
  catch( user_interruption & )
    {
      return EXIT_FAILURE;
    }
  catch( exception & e )
    {
      cerr << e.what() << endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
