/* Copyright (c) 1995-2007 CEA
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


#ifndef AIMS_ESTIMATION_LMS_ESTIMATOR_H
#define AIMS_ESTIMATION_LMS_ESTIMATOR_H


#include <aims/config/aimsalgo_config.h>
#include <aims/data/data.h>
#include <aims/vector/vector.h>
#include <aims/def/assert.h>
#include <aims/math/gausslu.h>
#include <aims/estimation/m-estimator.h>

//
// Linear Mean Squared M-estimator
//
template < int D >
class AIMSALGO_API LMSEstimator : public MEstimator< D >
{
  public:
    LMSEstimator() : MEstimator<D>() { }
    virtual ~LMSEstimator() { }

    void doit( const AimsData< AimsVector< float, D > >& x,
               const AimsData< float >& y, float& a,
               AimsVector< float, D >& b );
};


template < int D > inline
void
LMSEstimator<D>::doit( const AimsData< AimsVector< float, D > >& x,
                       const AimsData< float >& y, float& a,
                       AimsVector< float, D >& b )
{
  ASSERT( x.dimY() == 1 && x.dimZ() == 1 && x.dimT() == 1 );
  ASSERT( y.dimY() == 1 && y.dimZ() == 1 && y.dimT() == 1 );
  ASSERT( x.dimX() == y.dimX() );

  int N = y.dimX();
  AimsData<float> mat( D + 1, D + 1 );
  AimsData<float> vec( D + 1 );

  mat = 0.0;
  vec = 0.0;
  mat( 0, 0 ) = float( N );
  int k, n;
  for ( k = 1; k <= D; k++ )
  {
    for ( n = 0; n < N; n++ )
      mat( k, 0 ) += x( n ).item( k - 1 );
    mat( 0, k ) = mat( k, 0 );
  }

  int k1, k2;
  for ( k1 = 1; k1 <= D; k1++ )
    for ( k2 = 1; k2 <= k1; k2++ )
    {
      for ( n = 0; n < N; n++ )
        mat( k1, k2 ) += x( n ).item( k1 - 1 ) * x( n ).item( k2 - 1 );
      mat( k2, k1 ) = mat( k1, k2 );
    }

  
  for ( n = 0; n < N; n++ )
  {
    vec( 0 ) += y( n );
    for ( k = 1; k <= D; k++ )
      vec( k ) += y( n ) * x( n ).item( k - 1 );
  }

  AimsData<float> res = AimsLinearResolutionLU( mat, vec );
  a = res( 0 );
  for ( k = 1; k <= D; k++ )
    b.item( k - 1 ) = res( k );
}


#endif
