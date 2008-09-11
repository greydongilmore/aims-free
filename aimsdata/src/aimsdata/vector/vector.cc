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

 #include <aims/vector/vector.h>
 #include <cartobase/object/object_d.h>

// these instantiations are not necessary since everything is inline
// in AimsVector
template class AimsVector<short,2>;
template class AimsVector<short,3>;
template class AimsVector<short,4>;
template class AimsVector<int,2>;
template class AimsVector<int,3>;
template class AimsVector<int,4>;
template class AimsVector<float,2>;
template class AimsVector<float,3>;
template class AimsVector<float,4>;
template class AimsVector<double,2>;
template class AimsVector<double,3>;
template class AimsVector<double,4>;
template class AimsVector<unsigned,2>;
template class AimsVector<unsigned,3>;
template class AimsVector<unsigned,4>;

namespace carto {

INSTANTIATE_GENERIC_OBJECT_TYPE( Point2d )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3d )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4d )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2du )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3du )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4du )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2d> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3d> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4d> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2du> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3du> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4du> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dd> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dd> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dd> )

} // namespace carto
