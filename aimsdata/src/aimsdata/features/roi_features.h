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

#ifndef AIMS_ROI_FEATURES_H
#define AIMS_ROI_FEATURES_H


#include <iostream>
#include <cartobase/config/verbose.h>
#include <cartobase/object/property.h>
#include <aims/roi/roiIterator.h>
#include <aims/resampling/linearInterpolator.h>
#include <aims/features/point_set_features.h>


namespace aims {

  //---------------//
 //  RoiFeatures  //
//---------------//

class RoiFeatures
{
public:

  void addImageStatistics( const std::string &name, 
                           const carto::rc_ptr< Interpolator > & );
  void computeFeatures( const carto::rc_ptr< RoiIterator > & );
  void write( const std::string &fileName ) const;
  void write( std::ostream &out ) const;

private:


  typedef std::map< std::string,
                    carto::rc_ptr< Interpolator > > Interpolators_t;

  ScalarSetFeatures _scalarSetFeatures;
  Interpolators_t _interpolators;

  carto::rc_ptr< RoiIterator > _roiIterator;
  carto::Object _result;

};



} // namespace aims

#endif // ifndef AIMS_ROI_FEATURES_H
