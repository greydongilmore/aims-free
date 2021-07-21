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

/*
 *  Data writer class
 */

// this source is only here to force instanciation of some
// of the most useful Writer templates

#include <aims/config/aimsdata_config.h>
#include <soma-io/io/writer_d.h>
#include <soma-io/io/formatdictionary_d.h>
#include <aims/io/writer_d.h>
#include <aims/mesh/texture.h>

using namespace std;
using namespace carto;
using namespace aims;

AIMS_INSTANTIATE_WRITER( Texture1d);
AIMS_INSTANTIATE_WRITER( Texture2d);
AIMS_INSTANTIATE_WRITER( TimeTexture<double> );
AIMS_INSTANTIATE_WRITER( TimeTexture<int8_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<uint8_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<int16_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<uint16_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<int32_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<uint32_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<int64_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<uint64_t> );
AIMS_INSTANTIATE_WRITER( TimeTexture<Point2d> );
