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


#ifndef AIMS_SURFACE_SURFACEGEN_H
#define AIMS_SURFACE_SURFACEGEN_H

#include <aims/mesh/surface.h>
#include <cartobase/object/attributed.h>

namespace aims
{

  /// Factory class for various common shapes of meshes
  class SurfaceGenerator
  {
  public:
    typedef AimsSurfaceTriangle* 
    (*GenFunction)( const carto::GenericObject & );

    struct Generator
    {
      virtual ~Generator() {}

      virtual AimsSurfaceTriangle* 
      generator( const carto::GenericObject & ) const = 0;
      virtual carto::Object parameters() const = 0;
    };

    /// generic factory function (calls lower-level functions)
    static AimsSurfaceTriangle* 
    generate( const carto::GenericObject & params );
    /// generic factory function (calls lower-level functions)
    static AimsSurfaceTriangle* generate( carto::Object params );

    /// description of known shapes and parameters (vector of dicts)
    static carto::Object description();

    static AimsSurfaceTriangle* cube( const carto::GenericObject & params );
    static AimsSurfaceTriangle* cube( const Point3df & center, float radius, 
				      bool smoothnormal = false );

    static AimsSurfaceTriangle* 
    cylinder( const carto::GenericObject & params );
    static AimsSurfaceTriangle* cylinder( const Point3df & p1, 
					  const Point3df & p2, float radius, 
					  float radius2, unsigned nfacets, 
					  bool closed, bool smooth = false );

    static AimsSurfaceTriangle* cone( const carto::GenericObject & params );
    static AimsSurfaceTriangle* cone( const Point3df & arrow, 
				      const Point3df & base, float radius, 
				      unsigned nfacets, bool closed, 
				      bool smooth = false );

    static AimsSurfaceTriangle* 
    arrow( const carto::GenericObject & params );
    static AimsSurfaceTriangle* arrow( const Point3df & arrow, 
				       const Point3df & base, float radius, 
				       float arrowradius, unsigned nfacets, 
				       float arrowlengthfract );

    /** Sphere generation functions by Manik Bhattacharjee (CNRS UPR640 - LENA)

        Example of config file:
        \code
attributes = {
  'type' :  'sphere',
  'point1' : [ 11, 11, -10 ],
  'radius' : 45,
  'nfacets' : 100,
}        \endcode
     */
    static AimsSurfaceTriangle* 
    icosahedron( const carto::GenericObject & params );
    static AimsSurfaceTriangle* icosahedron( const Point3df & center, 
					     float radius );

    static AimsSurfaceTriangle* 
    sphere( const carto::GenericObject & params );
    static AimsSurfaceTriangle* sphere( const Point3df & p1, float radius, 
                                        unsigned nfacets );
  };

}


#endif
