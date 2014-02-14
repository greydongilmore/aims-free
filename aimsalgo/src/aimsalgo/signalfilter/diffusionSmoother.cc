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


#include <aims/signalfilter/diffusionSmoother_d.h>

using namespace aims;

template class DiffusionSmoother<int16_t>;
template class DiffusionSmoother<float>;
template class MaskedDiffusionSmoother<int16_t, std::vector<Point3df> >;
template class MaskedDiffusionSmoother<float, std::vector<Point3df> >;
template class MaskedDiffusionSmoother<int16_t, AimsData<short> >;
template class MaskedDiffusionSmoother<float, AimsData<short> >;

namespace aims {
template <>
AimsData<float> BaseDiffusionSmoother<float>::laplacian = 
	BaseDiffusionSmoother<float>::init_laplacian();

template <>
AimsData<float> BaseDiffusionSmoother<int16_t>::laplacian = 
	BaseDiffusionSmoother<float>::init_laplacian();

template<typename T>
AimsData<float> BaseDiffusionSmoother<T>::init_laplacian(void)
{
      AimsData<float>	laplacian(3, 3, 3, 1);
      laplacian = 0.;
      /* WARNING: this kernel is only suitable for isotropic, 1mm voxels.
         Moreover I don't explain the 1/2 factor on the whole kernel
         (I would expect coefs 1 -6 1 instead of 0.5 -3 0.5)
         Anyway, this kernel is now completely rebuilt in doSmoothing() to
         take into account voxel size, so it is not used as is.
         (Denis, 2014/02/14)
      */
      laplacian(1,1,0) = laplacian(1,0,1) = laplacian(0,1,1) =
	      laplacian(2,1,1) = laplacian(1,2,1) = laplacian(1,1,2) = 0.5;
      laplacian(1,1,1) = -3;

      return laplacian;
}

};
