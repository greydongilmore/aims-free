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

#ifndef CARTODATA_VOLUME_VOLUMEREF_D_H
#define CARTODATA_VOLUME_VOLUMEREF_D_H

#include <cartodata/volume/volumeref.h>
#include <cartodata/volume/volumeref_d_inline.h>
#include <cartodata/volume/volumeref_d_operators.h>
#include <cartodata/volume/volumeref_d_instantiate.h>
#include <cartodata/volume/volumebase_d.h>
#include <soma-io/allocator/allocator.h>
#include <cartobase/object/object.h>

namespace carto {

  template <typename T>
  void Creator<VolumeRef<T> >::setup( VolumeRef<T> & obj, Object header,
                                   const AllocatorContext & context,
                                   Object options )
  {
    Creator<Volume<T> >::setup( *obj, header, context, options );
  }

  template <typename T>
  VolumeRef<T>* Creator<VolumeRef<T> >::create( Object /* header */,
                                   const AllocatorContext & /* context */,
                                   Object /* options */ )
  {
    return new VolumeRef<T>;
  }
  
    
  template < typename T >
  void VolumeRef<T>::copyHeaderFrom( const Object & other )
  {
    (*this)->copyHeaderFrom( other );
  }

} // namespace carto

#endif // CARTODATA_VOLUME_VOLUMEREF_D_H
