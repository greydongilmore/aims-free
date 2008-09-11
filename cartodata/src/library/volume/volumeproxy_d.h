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

#ifndef CARTODATA_VOLUME_VOLUMEPROXY_D_H
#define CARTODATA_VOLUME_VOLUMEPROXY_D_H


#include <cartodata/volume/volumeproxy.h>


namespace carto
{


  template < typename T >
  VolumeProxy< T >::VolumeProxy( int sizeX, int sizeY, int sizeZ, int sizeT )
    : Headered(),
      _sizeX( sizeX ),
      _sizeY( sizeY ),
      _sizeZ( sizeZ ),
      _sizeT( sizeT )
  {

    header().addBuiltinProperty( "sizeX", _sizeX );
    header().addBuiltinProperty( "sizeY", _sizeY );
    header().addBuiltinProperty( "sizeZ", _sizeZ );
    header().addBuiltinProperty( "sizeT", _sizeT );

  }


  template < typename T >
  VolumeProxy< T >::VolumeProxy( const VolumeProxy< T >& other )
    : RCObject(), 
      Headered( other ),
      _sizeX( other._sizeX ),
      _sizeY( other._sizeY ),
      _sizeZ( other._sizeZ ),
      _sizeT( other._sizeT )
  {

    header().addBuiltinProperty( "sizeX", _sizeX );
    header().addBuiltinProperty( "sizeY", _sizeY );
    header().addBuiltinProperty( "sizeZ", _sizeZ );
    header().addBuiltinProperty( "sizeT", _sizeT );

  }


  template < typename T >
  VolumeProxy< T >::~VolumeProxy()
  {
  }


  template < typename T >
  VolumeProxy< T >& 
  VolumeProxy< T >::operator=( const VolumeProxy< T >& other )
  {

    if( this == &other )
      return *this;

    this->Headered::operator=( other );
    _sizeX = other._sizeX;
    _sizeY = other._sizeY;
    _sizeZ = other._sizeZ;
    _sizeT = other._sizeT;

    return *this;

  }

}

#endif
