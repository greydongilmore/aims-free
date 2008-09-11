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

/*
 * 3D linear image interpolation
 */
#include <aims/resampling/linearInterpolator.h>
#include <aims/io/process.h>
#include <aims/io/finder.h>
#include <aims/io/reader.h>

using namespace std;
using namespace carto;

namespace aims {


  //----------------//
 //  Interpolator  //
//----------------//

//-----------------------------------------------------------------------------
Interpolator::~Interpolator() {}


  //--------------------------------//
 //  linear interpolator factories //
//--------------------------------//

//-----------------------------------------------------------------------------
class LinearInterpolatorFactory : public Process
{
  friend rc_ptr< Interpolator > getLinearInterpolator( const string & );

  LinearInterpolatorFactory();
  template <typename T>
  static bool createInterpolator( Process & p, const string & filename, 
                                  Finder & finder );

  rc_ptr< Interpolator > interpolator;
};


//-----------------------------------------------------------------------------
LinearInterpolatorFactory:: LinearInterpolatorFactory()
{
  registerProcessType( "Volume", "S8", &createInterpolator<int8_t> );
  registerProcessType( "Volume", "U8", &createInterpolator<byte> );
  registerProcessType( "Volume", "S16", &createInterpolator<int16_t> );
  registerProcessType( "Volume", "U16", &createInterpolator<uint16_t> );
  registerProcessType( "Volume", "S32", &createInterpolator<int32_t> );
  registerProcessType( "Volume", "U32", &createInterpolator<uint32_t> );
  registerProcessType( "Volume", "FLOAT", &createInterpolator<float> );
  registerProcessType( "Volume", "DOUBLE", &createInterpolator<double> );
}


//-----------------------------------------------------------------------------
template <typename T>
bool LinearInterpolatorFactory::createInterpolator( Process & p,
                                                    const string & filename, 
                                                    Finder & finder )
{
  AimsData<T> image;
  Reader< AimsData<T> > r( filename );
  r.setMode( MAP_RO );
  string format = finder.format();
  if( ! r.read( image, 0, &format ) ) {
    return false;
  }
  LinearInterpolatorFactory &factory = 
    static_cast< LinearInterpolatorFactory &>( p );
  factory.interpolator = 
    rc_ptr< Interpolator >( new LinearInterpolator<T>( image ) );
  return true;
}


//-----------------------------------------------------------------------------
rc_ptr< Interpolator > getLinearInterpolator( const string &fileName )
{
  LinearInterpolatorFactory factory;
  factory.execute( fileName );
  return factory.interpolator;
}


//-----------------------------------------------------------------------------
template carto::rc_ptr< Interpolator > 
 getLinearInterpolator( const AimsData<uint8_t> & );
template carto::rc_ptr< Interpolator >
 getLinearInterpolator( const AimsData<int8_t> & );
template carto::rc_ptr< Interpolator > 
 getLinearInterpolator( const AimsData<uint16_t> & );
template carto::rc_ptr< Interpolator >
 getLinearInterpolator( const AimsData<int16_t> & );
template carto::rc_ptr< Interpolator > 
 getLinearInterpolator( const AimsData<uint32_t> & );
template carto::rc_ptr< Interpolator >
 getLinearInterpolator( const AimsData<int32_t> & );
template carto::rc_ptr< Interpolator >
 getLinearInterpolator( const AimsData<float> & );
template carto::rc_ptr< Interpolator >
 getLinearInterpolator( const AimsData<double> & );


  //--------------------------//
 //   LinearInterpolator<T>  //
//--------------------------//

//-----------------------------------------------------------------------------
template class LinearInterpolator<uint8_t>;
template class LinearInterpolator<int8_t>;
template class LinearInterpolator<uint16_t>;
template class LinearInterpolator<int16_t>;
template class LinearInterpolator<uint32_t>;
template class LinearInterpolator<int32_t>;
template class LinearInterpolator<float>;
template class LinearInterpolator<double>;


} // namespace aims
