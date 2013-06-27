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

#include <cartobase/object/object_d.h>
#include <cartodata/volume/volume_d.h>
#include <cartodata/volume/volumeutil_d.h>
#include <functional>

#ifdef USE_SOMA_IO
  #include <soma-io/image/voxelrgb_d.h>
  #include <soma-io/image/voxelrgba_d.h>
  #include <soma-io/image/voxelhsv.h>
  using namespace soma;
#endif

using namespace carto;

template class VolumeProxy< int8_t >;
template class VolumeProxy< uint8_t >;
// ### remove after everything has been moved to intN_t/uintN_t
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
template class VolumeProxy< char >;
#endif
template class VolumeProxy< int16_t >;
template class VolumeProxy< uint16_t >;
template class VolumeProxy< int32_t >;
template class VolumeProxy< uint32_t >;
template class VolumeProxy< long >;
template class VolumeProxy< unsigned long >;
template class VolumeProxy< float >;
template class VolumeProxy< double >;
template class VolumeProxy< cfloat >;
template class VolumeProxy< cdouble >;
template class VolumeProxy< std::map<int, float> >;
#ifdef USE_SOMA_IO
template class VolumeProxy< VoxelRGB >;
template class VolumeProxy< VoxelRGBA >;
template class VolumeProxy< VoxelHSV >;
#endif

template class Volume< int8_t >;
template class Volume< uint8_t >;
// ### remove after everything has been moved to intN_t/uintN_t
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
template class Volume< char >;
#endif
template class Volume< int16_t >;
template class Volume< uint16_t >;
template class Volume< int32_t >;
template class Volume< uint32_t >;
template class Volume< long >;
template class Volume< unsigned long >;
template class Volume< float >;
template class Volume< double >;
template class Volume< cfloat >;
template class Volume< cdouble >;
template class Volume< std::map<int, float> >;
#ifdef USE_SOMA_IO
template class Volume< VoxelRGB >;
template class Volume< VoxelRGBA >;
template class Volume< VoxelHSV >;
#endif

template class Creator<Volume< int8_t > >;
template class Creator<Volume< uint8_t > >;
// ### remove after everything has been moved to intN_t/uintN_t
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
template class Creator<Volume< char > >;
#endif
template class Creator<Volume< int16_t > >;
template class Creator<Volume< uint16_t > >;
template class Creator<Volume< int32_t > >;
template class Creator<Volume< uint32_t > >;
template class Creator<Volume< long > >;
template class Creator<Volume< unsigned long > >;
template class Creator<Volume< float > >;
template class Creator<Volume< double > >;
template class Creator<Volume< cfloat > >;
template class Creator<Volume< cdouble > >;
template class Creator<Volume< std::map<int, float> > >;
#ifdef USE_SOMA_IO
template class Creator< Volume< VoxelRGB > >;
template class Creator< Volume< VoxelRGBA > >;
template class Creator< Volume< VoxelHSV > >;
#endif

// utilities

#define instantiate_volutil( T ) \
template class VolumeUtil<T>; \
template VolumeRef<T> \
VolumeUtil<T>::apply( Scaler<T,double>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Scaler<T,double>, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( Scaler<T,float>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Scaler<T,float>, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( Scaler<T,long>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Scaler<T,long>, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( std::negate<T>, const VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( Divider<T,double>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Divider<T,double>, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( Divider<T,float>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Divider<T,float>, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( Divider<T,long>, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( Divider<T,long>, VolumeRef<T> & );

#define instantiate_volutil2( T, BinaryFunction ) \
template VolumeRef<T> \
VolumeUtil<T>::apply( BinaryFunction, const VolumeRef<T> &, \
                      const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( BinaryFunction, VolumeRef<T> &, \
                          const VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( UnaryFromConstantBinaryFunctor<T, \
                      BinaryFunction >, const VolumeRef<T> & ); \
template void \
VolumeUtil<T>::selfApply( UnaryFromConstantBinaryFunctor<T, \
                          BinaryFunction >, VolumeRef<T> & ); \
template VolumeRef<T> \
VolumeUtil<T>::apply( UnaryFromConstantBinaryFunctor2<T, \
                      BinaryFunction >, const VolumeRef<T> & ); \

instantiate_volutil( int8_t )
instantiate_volutil2( int8_t, std::plus<int8_t> )
instantiate_volutil2( int8_t, std::minus<int8_t> )
instantiate_volutil2( int8_t, std::multiplies<int8_t> )
instantiate_volutil2( int8_t, std::divides<int8_t> )

instantiate_volutil( uint8_t )
instantiate_volutil2( uint8_t, std::plus<uint8_t> )
instantiate_volutil2( uint8_t, std::minus<uint8_t> )
instantiate_volutil2( uint8_t, std::multiplies<uint8_t> )
instantiate_volutil2( uint8_t, std::divides<uint8_t> )

// ### remove after everything has been moved to intN_t/uintN_t
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
instantiate_volutil( char )
instantiate_volutil2( char, std::plus<char> )
instantiate_volutil2( char, std::minus<char> )
instantiate_volutil2( char, std::multiplies<char> )
instantiate_volutil2( char, std::divides<char> )
#endif

instantiate_volutil( int16_t )
instantiate_volutil2( int16_t, std::plus<int16_t> )
instantiate_volutil2( int16_t, std::minus<int16_t> )
instantiate_volutil2( int16_t, std::multiplies<int16_t> )
instantiate_volutil2( int16_t, std::divides<int16_t> )

instantiate_volutil( uint16_t )
instantiate_volutil2( uint16_t, std::plus<uint16_t> )
instantiate_volutil2( uint16_t, std::minus<uint16_t> )
instantiate_volutil2( uint16_t, std::multiplies<uint16_t> )
instantiate_volutil2( uint16_t, std::divides<uint16_t> )

instantiate_volutil( int32_t )
instantiate_volutil2( int32_t, std::plus<int32_t> )
instantiate_volutil2( int32_t, std::minus<int32_t> )
instantiate_volutil2( int32_t, std::multiplies<int32_t> )
instantiate_volutil2( int32_t, std::divides<int32_t> )

instantiate_volutil( uint32_t )
instantiate_volutil2( uint32_t, std::plus<uint32_t> )
instantiate_volutil2( uint32_t, std::minus<uint32_t> )
instantiate_volutil2( uint32_t, std::multiplies<uint32_t> )
instantiate_volutil2( uint32_t, std::divides<uint32_t> )

instantiate_volutil( long )
instantiate_volutil2( long, std::plus<long> )
instantiate_volutil2( long, std::minus<long> )
instantiate_volutil2( long, std::multiplies<long> )
instantiate_volutil2( long, std::divides<long> )

instantiate_volutil( unsigned long )
instantiate_volutil2( unsigned long, std::plus<unsigned long> )
instantiate_volutil2( unsigned long, std::minus<unsigned long> )
instantiate_volutil2( unsigned long, std::multiplies<unsigned long> )
instantiate_volutil2( unsigned long, std::divides<unsigned long> )

instantiate_volutil( float )
instantiate_volutil2( float, std::plus<float> )
instantiate_volutil2( float, std::minus<float> )
instantiate_volutil2( float, std::multiplies<float> )
instantiate_volutil2( float, std::divides<float> )

instantiate_volutil( double )
instantiate_volutil2( double, std::plus<double> )
instantiate_volutil2( double, std::minus<double> )
instantiate_volutil2( double, std::multiplies<double> )
instantiate_volutil2( double, std::divides<double> )

instantiate_volutil( cfloat )
instantiate_volutil2( cfloat, std::plus<cfloat> )
instantiate_volutil2( cfloat, std::minus<cfloat> )
instantiate_volutil2( cfloat, std::multiplies<cfloat> )
instantiate_volutil2( cfloat, std::divides<cfloat> )

instantiate_volutil( cdouble )
instantiate_volutil2( cdouble, std::plus<cdouble> )
instantiate_volutil2( cdouble, std::minus<cdouble> )
instantiate_volutil2( cdouble, std::multiplies<cdouble> )
instantiate_volutil2( cdouble, std::divides<cdouble> )

namespace carto {

  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< int8_t > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< uint8_t > )
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< char > )
#endif
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< int16_t > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< uint16_t > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< int32_t > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< uint32_t > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< long > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< unsigned long > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< float > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< double > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< cfloat > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( VolumeRef< cdouble > )

  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< int8_t > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< uint8_t > > )
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< char > > )
#endif
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< int16_t > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< uint16_t > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< int32_t > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< uint32_t > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< long > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< unsigned long > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< float > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< double > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< cfloat > > )
  INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<Volume< cdouble > > )


} // namespace carto
