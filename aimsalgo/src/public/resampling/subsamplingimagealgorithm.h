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

#ifndef AIMSALGO_RESAMPLING_SUBSAMPLINGIMAGEALGORITHM_H
#define AIMSALGO_RESAMPLING_SUBSAMPLINGIMAGEALGORITHM_H

#include <iostream>
#include <cartobase/config/verbose.h>
#include <cartobase/type/string_conversion.h>
#include <cartobase/type/types.h>
#include <cartobase/type/datatypetraits.h>
#include <cartobase/type/datatypeinfo.h>
#include <aims/algorithm/imagealgorithm.h>
#include <aims/data/data.h>
#include <aims/signalfilter/nonlin_filt-func.h>
#include <aims/utility/progress.h>
#include <aims/vector/vector.h>

namespace aims {

  namespace singlechannel {

  /**
   *  \brief aims::singlechannel::SubSamplingImageAlgorithm is the algorithm to apply subsampling on single channel image.
   *
   *  The \c aims::SubSamplingImageAlgorithm class is used 
   *  to subsample a single channel image.
   *  \tparam VoxelType type of voxel that will be subsampled.
   *  \tparam FilteringFunctionType type of subsampling to use (\c MeanFilterFunc, \c MedianFilterFunc, \c MinFilterFunc, \c MaxFilterFunc, ... ).
   * 
   *  \sa MeanFilterFunc, MedianFilterFunc, MinFilterFunc, MaxFilterFunc, MajorityFilterFunc
   */
    template< class VoxelType, class FilteringFunctionType >
    class SubSamplingImageAlgorithm :
      public ImageAlgorithmInterface< VoxelType >
    {
      public:

        SubSamplingImageAlgorithm( int sx = 3, int sy = 3, int sz = 1, carto::Object options = carto::none() );
        virtual ~SubSamplingImageAlgorithm() { }

        /// \c ImageAlgorithmInterface<VoxelType>::getOutputImageDimensions method implementation.
        /// Returns the output dimensions based on an input dimensions.
        /// \return \c Point4d output dimensions of the subsampled image.
        virtual const Point4dl getOutputImageDimensions( const Point4dl & dims ) {
          return Point4dl( dims[0] / _win_size_x,
                           dims[1] / _win_size_y,
                           dims[2] / _win_size_z,
                           dims[3] );
        
        }
        
        /// \c ImageAlgorithmInterface<VoxelType>::getOutputImageVoxelSize method implementation.
        /// Returns the output voxel size based on an input voxel size.
        /// \param Point4d voxelsize voxel size of the input image.
        /// \return \c Point4d voxel size of the output image.
        virtual const Point4df getOutputImageVoxelSize( const Point4df & voxelsize ) {
          return Point4df( voxelsize[0] * _win_size_x,
                           voxelsize[1] * _win_size_y,
                           voxelsize[2] * _win_size_z,
                           voxelsize[3] );
        }
      
        /// \c ImageAlgorithmInterface<VoxelType>::execute method implementation.
        /// Execute the subsampling algorithm on the input image using the \c FilteringFunctionType
        /// \param in AimsData<VoxelType> input image to subsample
        /// \return AimsData< VoxelType > subsampled image
        virtual AimsData< VoxelType > execute( const AimsData<VoxelType>& in );

    protected:

      int                         _win_size_x;
      int                         _win_size_y;
      int                         _win_size_z;
      const FilteringFunctionType _func;
      carto::Object               _options;
    };


    template< class VoxelType, class FilteringFunctionType >
    inline 
    SubSamplingImageAlgorithm< VoxelType, FilteringFunctionType >::SubSamplingImageAlgorithm(int sx, int sy, int sz, carto::Object options )
      : _win_size_x(sx), 
        _win_size_y(sy),
        _win_size_z(sz),
        _options(options)
    {
    }


    template< class VoxelType, class FilteringFunctionType > 
    inline
    AimsData< VoxelType > SubSamplingImageAlgorithm< VoxelType, FilteringFunctionType >::execute( const AimsData< VoxelType >& in )
    {
//       std::cout << "Input dim: [" << carto::toString(in.dimX()) << ", "
//                                   << carto::toString(in.dimY()) << ", "
//                                   << carto::toString(in.dimZ()) << ", "
//                                   << carto::toString(in.dimT()) << "]" << std::endl;
//       std::cout << "Window dim: [" << carto::toString(_win_size_x) << ", "
//                                    << carto::toString(_win_size_y) << ", "
//                                    << carto::toString(_win_size_z) << "]" << std::endl;
//       std::cout << "Output dim: [" << carto::toString(in.dimX() / _win_size_x) << ", "
//                                    << carto::toString(in.dimY() / _win_size_y) << ", "
//                                    << carto::toString(in.dimZ() / _win_size_z) << "]" << std::endl;
      Point4dl dim = getOutputImageDimensions( Point4dl( in.dimX(),
                                                         in.dimY(),
                                                         in.dimZ(),
                                                         in.dimT() ) );
/*                                                       
      std::cout << "Output dim: [" << carto::toString(dim[0]) << ", "
                                   << carto::toString(dim[1]) << ", "
                                   << carto::toString(dim[2]) << ", "
                                   << carto::toString(dim[3]) << "]" << std::endl;*/
      
      Point3df size;
          
      size[0] = in.sizeX() * _win_size_x;
      size[1] = in.sizeY() * _win_size_y;
      size[2] = in.sizeZ() * _win_size_z;
    
      // Data OUT
      AimsData< VoxelType > out( dim[0], dim[1], dim[2], dim[3] );
      if( in.header() ) {
        out.setHeader( in.header()->cloneHeader( true ) );
      }
      out.setSizeXYZT( size[0], size[1], size[2], in.sizeT() );

//       if (carto::verbose) {
//         std::cout << "Input volume dimensions: [" << in.dimX() << ", "
//                                                   << in.dimY() << ", "
//                                                   << in.dimZ() << "]" << std::endl
// 
//                   << "Subsampled volume dimensions: [" << out.dimX() << ", "
//                                                        << out.dimY() << ", "
//                                                        << out.dimZ() << "]" << std::endl
// 
//                   << "Input voxel size: [" << in.sizeX() << ", "
//                                            << in.sizeY() << ", "
//                                            << in.sizeZ() << "]" << std::endl
// 
//                   << "Subsampled voxel size: [" << out.sizeX() << ", "
//                                                 << out.sizeY() << ", "
//                                                 << out.sizeZ() << "]" << std::endl;
//       }

      if (out.dimT() * out.dimZ() * out.dimY() > 0) {
        // Window used to process
//         AimsData< VoxelType > win( _win_size_x * _win_size_y * _win_size_z );

        // Creates a view in the AimsData that has windows dimensions
        // and that 
        carto::VolumeRef<VoxelType> win(
          new carto::Volume<VoxelType>(
            in.volume(), 
            typename carto::Volume<VoxelType>::Position4Di( 0, 0, 0, 0 ),
            typename carto::Volume<VoxelType>::Position4Di( _win_size_x, 
                                                            _win_size_y, 
                                                            _win_size_z,
                                                            1 )
          )
        );
        
        int32_t i, j, k, t;
        unsigned m, n, l;
        aims::Progression progress(out.dimT() * out.dimZ() * out.dimY() - 1);
        
        if (carto::verbose)
          std::cout << "Subsampling progress: ";
      
        for( t = 0; t < (int32_t)out.dimT(); ++t )
          for( k = 0; k < (int32_t)out.dimZ(); ++k )
            for( j = 0; j < (int32_t)out.dimY(); ++j, ++progress ) {
              if (carto::verbose) {
                // Display progression
              /* The "normal" operator << should be as:
                 std::cout << progress << std::flush;
                 but it doesn't work in gcc 4.0, there is a namespace
                 confusion, so we have to specify ::operator <<
              */
              ::operator << ( std::cout, progress ) << std::flush;
              }

              for( i = 0; i < (int32_t)out.dimX(); ++i ) {
//                 for( l = 0; l < _win_size_z; ++l )
//                   for( n = 0; n < _win_size_y; ++n )
//                     for( m = 0; m < _win_size_x; ++m ) {
//                       win( l * _win_size_x * _win_size_y + n * _win_size_x + m ) = in( i * _win_size_x + m , j * _win_size_y + n, k * _win_size_z + l, t );
//                     }
                    
//                 out( i, j, k, t ) = _func.doit(carto::VolumeRef<VoxelType>(win.volume()));

                // Set view position in the volume
                win->setPosInRefVolume( typename carto::Volume<VoxelType>::Position4Di(i * _win_size_x,
                                                                                       j * _win_size_y,
                                                                                       k * _win_size_z, 
                                                                                       t) );
      

                out( i, j, k, t ) = _func.doit(win);
              }
            }

        if (carto::verbose)
          std::cout << std::endl;
      }
      
      return out;
    }
    
  }
   
  /**
   *  \brief aims::SubSamplingImageAlgorithm is the algorithm to subsample image.
   *
   *  The \c aims::SubSamplingImageAlgorithm class is used 
   *  to subsample both single and multi channel image.
   */
  template <class VoxelType, class FilterType>
  class SubSamplingImageAlgorithm :
    public ImageAlgorithm<VoxelType,  
              aims::singlechannel::SubSamplingImageAlgorithm<
                typename carto::DataTypeTraits<VoxelType>::ChannelType, 
                FilterType > 
              >
  {
    public:
      typedef ImageAlgorithm<VoxelType,  
              aims::singlechannel::SubSamplingImageAlgorithm<
                typename carto::DataTypeTraits<VoxelType>::ChannelType, 
                FilterType > 
              >
              ImageAlgorithmType;
              
      typedef typename ImageAlgorithmType::SingleChannelImageAlgorithmType 
              SingleChannelImageAlgorithmType;
              
      typedef FilterType FilterFuncType;
      
      SubSamplingImageAlgorithm( int sx = 3, int sy = 3, int sz = 1, carto::Object options = carto::none() )
        : ImageAlgorithmType(
            SingleChannelImageAlgorithmType(sx, sy, sz, options)
          )
      {
      }
      virtual ~SubSamplingImageAlgorithm() { }
  };

}

#endif
