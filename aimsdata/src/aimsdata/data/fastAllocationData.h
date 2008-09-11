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

#ifndef AIMS_DATA_FASTALLOCATIONDATA_H
#define AIMS_DATA_FASTALLOCATIONDATA_H

#include <aims/data/data.h>
#include <cartobase/allocator/allocator.h>

namespace aims {

  template <typename T>
  class AimsFastAllocationData : public AimsData<T>
  {
  public:

    static const carto::AllocatorContext fastAllocatorContext;

    inline AimsFastAllocationData( int dimx = 1, int dimy = 1, int dimz = 1, 
                                   int dimt = 1, int borderw = 0 ) :
      AimsData<T>( dimx, dimy, dimz, dimt, borderw,
                AimsFastAllocationData::fastAllocatorContext ) {}

    inline AimsFastAllocationData( const AimsFastAllocationData<T> &other ) :
      AimsData<T>( other ) {}

    inline AimsFastAllocationData( const AimsData<T> &other ) :
      AimsData<T>( other ) {}

    inline AimsFastAllocationData<T> &operator =
      ( const AimsFastAllocationData<T> &other ) 
    {
      if ( &other != this ) {
        static_cast<AimsData<T> &>( *this ) = other;
      }
      return *this;
    }

    inline AimsFastAllocationData<T> &operator =
      ( const AimsData<T> &other ) 
    {
      if ( &other != this ) {
        static_cast<AimsData<T> &>( *this ) = other;
      }
      return *this;
    }

    AimsFastAllocationData<T> & operator = ( const T &v )
    {
      AimsData<T>::operator =( v );
      return *this;
    }

    inline virtual ~AimsFastAllocationData() {};
  };


  template <typename T>
  const carto::AllocatorContext 
  AimsFastAllocationData<T>::fastAllocatorContext( &carto::MemoryAllocator::singleton() );


  extern template class AimsFastAllocationData<uint8_t>;
  extern template class AimsFastAllocationData<int8_t>;
  extern template class AimsFastAllocationData<uint16_t>;
  extern template class AimsFastAllocationData<int16_t>;
  extern template class AimsFastAllocationData<uint32_t>;
  extern template class AimsFastAllocationData<int32_t>;
  extern template class AimsFastAllocationData<float>;
  extern template class AimsFastAllocationData<double>;

} // namespace aims

#endif // ifndef AIMS_DATA_FASTALLOCATIONDATA_H
