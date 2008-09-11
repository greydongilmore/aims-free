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

#include <aims/plugin/jpegplugin.h>
#include <aims/io/jpegformat_d.h>
#include <aims/io/jpegfinderformat.h>
#include <aims/io/finder.h>
#include <aims/rgb/rgb.h>
#include <aims/io/baseFormats_cartovolume.h>
#ifdef __APPLE__
#include <aims/io/macosxbugs.h>
#endif

using namespace aims;
using namespace carto;
using namespace std;

namespace
{

  bool initjpeg()
  {
    new JpegPlugin;
    return true;
  }

  bool jpeginit = initjpeg();

}

JpegPlugin::JpegPlugin() : Plugin()
{
  vector<string>	ext;
  ext.push_back( "jpg" );
  JpegFormat<uint8_t>	*df2 = new JpegFormat<uint8_t>;
  FileFormatDictionary<AimsData<uint8_t> >::registerFormat( "JPEG", df2, ext );
  VolumeFormat<uint8_t>	*vf2 = new VolumeFormat<uint8_t>( "JPEG" );
  FileFormatDictionary<Volume<uint8_t> >::registerFormat( "JPEG", vf2, ext );

  JpegFormat<AimsRGB>	*df8 = new JpegFormat<AimsRGB>;
#ifdef AIMS_APPLE_GCC33BUG
  macosxbugs::fileFormatDictionary_dataRGB_registerFormat( "JPEG", df8, ext );
#else
  FileFormatDictionary<AimsData<AimsRGB> >::registerFormat( "JPEG", df8, ext );
#endif
  VolumeFormat<AimsRGB>	*vf8 = new VolumeFormat<AimsRGB>( "JPEG" );
  FileFormatDictionary<Volume<AimsRGB> >::registerFormat( "JPEG", vf8, ext );
  Finder::registerFormat( "JPEG", new FinderJpegFormat, ext );
}


JpegPlugin::~JpegPlugin()
{
}


string JpegPlugin::name() const
{
  return "JPEG IO";
}


bool JpegPlugin::noop()
{
  return true;
}

