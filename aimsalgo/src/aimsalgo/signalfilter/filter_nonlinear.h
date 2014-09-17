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

#ifndef AIMS_SIGNALFILTER_NONLINEAR_H
#define AIMS_SIGNALFILTER_NONLINEAR_H

//--- aims -------------------------------------------------------------------
#include <aims/algorithm/imagealgorithm.h>          // ImageAlgorithmInterface
#include <aims/signalfilter/filteringimagealgorithm.h> // FilteringImageAlgorithm
#include <aims/signalfilter/filteringfunction_nonlinear.h>      // *FilterFunc
#include <aims/connectivity/structuring_element.h>       // StructuringElement
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/datatypetraits.h>                   // DataTypeTraits
#include <cartobase/object/object.h>                                 // Object
#include <cartobase/smart/rcptr.h>                                   // rc_ptr
//--- std --------------------------------------------------------------------
#include <set>
#include <map>
#include <string>
//----------------------------------------------------------------------------

// All classes declared here are derived from aims::FilteringImageAlgorithm,
// templated with the appropriate filtering function.

namespace aims {
  //==========================================================================
  // NON LINEAR FILTERING ALGORITHM: FACTORY
  //==========================================================================
  /// Factory class for non linear filtering image algorithms
  /// Here are the registered algorithms:
  /// \code
  /// registerFilter( "min", MinFilter<T>() );
  /// registerFilter( "max", MaxFilter<T>() );
  /// registerFilter( "med", MedianFilter<T>() );
  /// registerFilter( "median", MedianFilter<T>() );
  /// registerFilter( "notnullmedian", NotNullMedianFilter<T>() );
  /// registerFilter( "mea", MeanFilter<T>() );
  /// registerFilter( "mean", MeanFilter<T>() );
  /// registerFilter( "notnullmean", NotNullMeanFilter<T>() );
  /// registerFilter( "maj", MajorityFilter<T>() );
  /// registerFilter( "majority", MajorityFilter<T>() );
  /// registerFilter( "dif", ExtremaDifferenceFilter<T>() );
  /// registerFilter( "difference", ExtremaDifferenceFilter<T>() );
  /// registerFilter( "sum", SumFilter<T>() );
  /// \code
  template <typename T>
  class NonLinFilterFactory
  {
    public:
      static FilteringImageAlgorithmInterface<T> * create( const std::string & name,
                                                           const StructuringElement & strel,
                                                           carto::Object options = carto::none() );
      static void registerFilter( const std::string & name,
                                  const FilteringImageAlgorithmInterface<T> & func );
      static std::set<std::string> names();
    protected:
      static void init();
      static std::map<std::string,carto::rc_ptr<FilteringImageAlgorithmInterface<T> > > & _map();
  };
}

//============================================================================
// NON LINEAR FILTERING ALGORITHM: DERIVED CLASSES
//============================================================================
#define AIMS_DECLARE_FILTER_NONLINEAR( NAME, FUNC, STREL )                   \
  template <typename T>                                                      \
  class NAME: public FilteringImageAlgorithm<T,                              \
                       FUNC<typename carto::DataTypeTraits<T>::ChannelType> >\
  {                                                                          \
    public:                                                                  \
      typedef FilteringImageAlgorithm<T,                                     \
                FUNC<typename carto::DataTypeTraits<T>::ChannelType> >       \
              FilteringImageAlgorithmType;                                   \
                                                                             \
      typedef typename FilteringImageAlgorithmType::FilterFuncType           \
              FilterFuncType;                                                \
                                                                             \
      NAME( const StructuringElement & strel = STREL,                        \
            carto::Object options = carto::none() )                          \
        : FilteringImageAlgorithmType( strel, options ) {}                   \
  }

namespace aims {
  AIMS_DECLARE_FILTER_NONLINEAR( MinFilter, MinFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( MaxFilter, MaxFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( MedianFilter, MedianFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( NotNullMedianFilter, NotNullMedianFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( MeanFilter, MeanFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( NotNullMeanFilter, NotNullMeanFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( MajorityFilter, MajorityFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( ExtremaDifferenceFilter, ExtremaDifferenceFilterFunc, strel::Cube(1.) );
  AIMS_DECLARE_FILTER_NONLINEAR( SumFilter, SumFilterFunc, strel::Cube(1.) );
}

#endif
