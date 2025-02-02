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

// we don't want to issue a warning
#ifndef AIMSDATA_CLASS_NO_DEPREC_WARNING
#define AIMSDATA_CLASS_NO_DEPREC_WARNING
#endif

/*
 *  IO classes
 */

/* this source is only here to force instanciation of some of the most useful 
   Reader templates */

#include <aims/io/fileFormat_d.h>
#include <aims/io/baseFormats_volume_d.h>
#include <aims/rgb/rgb.h>
#include <aims/hsv/hsv.h>
#include <aims/io/fileFormat_cartovolume_d.h>

using namespace aims;
using namespace carto;

namespace aims
{

template<> void
FileFormatDictionary<Volume<AimsRGB> >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<AimsRGB> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<AimsRGB>	*f = new VolumeRefFormat<AimsRGB>;
  registerFormat( "ALLVOLUMES", f, ext );
}

template<> void 
FileFormatDictionary<Volume<AimsRGBA> >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<AimsRGBA> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<AimsRGBA>	*f = new VolumeRefFormat<AimsRGBA>;
  registerFormat( "ALLVOLUMES", f, ext );
}

template<> void
FileFormatDictionary<Volume<AimsHSV> >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<AimsHSV> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<AimsHSV>	*f = new VolumeRefFormat<AimsHSV>;
  registerFormat( "ALLVOLUMES", f, ext );
}

template<> void 
FileFormatDictionary<Volume<Point3df> >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<Point3df> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<Point3df>	*f = new VolumeRefFormat<Point3df>;
  registerFormat( "ALLVOLUMES", f, ext );
}

template<> void 
FileFormatDictionary<Volume<Point3d> >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<Point3d> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<Point3d>	*f = new VolumeRefFormat<Point3d>;
  registerFormat( "ALLVOLUMES", f, ext );
}

template<> void 
FileFormatDictionary<Volume<AimsVector<float, 6> > >::registerBaseFormats()
{
}

template<> void 
FileFormatDictionary<VolumeRef<AimsVector<float, 6> > >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  VolumeRefFormat<AimsVector<float, 6> > 
    *f = new VolumeRefFormat<AimsVector<float, 6> >;
  registerFormat( "ALLVOLUMES", f, ext );
}


template<> void 
FileFormatDictionary<AimsData<AimsRGB> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  VolumeFormat<AimsRGB> *vf;

#ifdef AIMS_JPEG
  ext.clear();
  ext.push_back( "jpg" );
  JpegFormat<AimsRGB>	*jf = new JpegFormat<AimsRGB>;
  registerFormat( "JPEG", jf, ext );
  vf = new VolumeFormat<AimsRGB>( "JPEG" );
  FileFormatDictionary<Volume<AimsRGB> >::registerFormat( "JPEG", vf, ext );
#endif

  ext.clear();
  ext.push_back( "" );
  SomaIOAimsDataFormat<AimsRGB>  *df = new SomaIOAimsDataFormat<AimsRGB>;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  vf = new VolumeFormat<AimsRGB>( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<AimsRGB> >::registerFormat( "SOMAIO_VOLUMES", 
                                                          vf, ext );
}

template<> void 
FileFormatDictionary<AimsData<AimsRGBA> >::registerBaseFormats()
{
  std::vector<std::string>	ext;

  ext.push_back( "" );
  SomaIOAimsDataFormat<AimsRGBA>  *df = new SomaIOAimsDataFormat<AimsRGBA>;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  VolumeFormat<AimsRGBA> *vf = new VolumeFormat<AimsRGBA>( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<AimsRGBA> >::registerFormat( "SOMAIO_VOLUMES", 
                                                           vf, ext );
}

template<> void 
FileFormatDictionary<AimsData<AimsHSV> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  SomaIOAimsDataFormat<AimsHSV>  *df = new SomaIOAimsDataFormat<AimsHSV>;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  VolumeFormat<AimsHSV> *vf = new VolumeFormat<AimsHSV>( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<AimsHSV> >::registerFormat( "SOMAIO_VOLUMES", 
                                                          vf, ext );
}

template<> void 
FileFormatDictionary<AimsData<Point3df> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  SomaIOAimsDataFormat<Point3df>  *df = new SomaIOAimsDataFormat<Point3df>;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  VolumeFormat<Point3df> *vf = new VolumeFormat<Point3df>( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<Point3df> >::registerFormat( "SOMAIO_VOLUMES", 
                                                           vf, ext );
}

template<> void 
FileFormatDictionary<AimsData<Point3d> >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  SomaIOAimsDataFormat<Point3d>  *df = new SomaIOAimsDataFormat<Point3d>;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  VolumeFormat<Point3d> *vf = new VolumeFormat<Point3d>( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<Point3d> >::registerFormat( "SOMAIO_VOLUMES", 
                                                          vf, ext );
}

template<> void 
FileFormatDictionary<AimsData<AimsVector<float,6> > >::registerBaseFormats()
{
  std::vector<std::string>	ext;
  ext.push_back( "" );
  SomaIOAimsDataFormat<AimsVector<float,6> >  *df
    = new SomaIOAimsDataFormat<AimsVector<float,6> >;
  registerFormat( "SOMAIO_VOLUMES", df, ext );
  VolumeFormat<AimsVector<float, 6> > *vf
    = new VolumeFormat<AimsVector<float,6> >( "SOMAIO_VOLUMES" );
  FileFormatDictionary<Volume<AimsVector<float,6> > >::registerFormat(
    "SOMAIO_VOLUMES", vf, ext );
}


template class FileFormatDictionary<AimsData<AimsRGB> >;
template class FileFormatDictionary<AimsData<AimsRGBA> >;
template class FileFormatDictionary<AimsData<AimsHSV> >;
template class FileFormatDictionary<AimsData<Point3df> >;
template class FileFormatDictionary<AimsData<Point3d> >;
template class FileFormatDictionary<AimsData<AimsVector<float,6> > >;

template class FileFormatDictionary<Volume<AimsRGB> >;
template class FileFormatDictionary<Volume<AimsRGBA> >;
template class FileFormatDictionary<Volume<AimsHSV> >;
template class FileFormatDictionary<Volume<Point3df> >;
template class FileFormatDictionary<Volume<Point3d> >;
template class FileFormatDictionary<Volume<AimsVector<float,6> > >;

template class FileFormatDictionary<VolumeRef<AimsRGB> >;
template class FileFormatDictionary<VolumeRef<AimsRGBA> >;
template class FileFormatDictionary<VolumeRef<AimsHSV> >;
template class FileFormatDictionary<VolumeRef<Point3df> >;
template class FileFormatDictionary<VolumeRef<Point3d> >;
template class FileFormatDictionary<VolumeRef<AimsVector<float,6> > >;

template class FileFormat<AimsData<AimsRGB> >;
template class FileFormat<AimsData<AimsRGBA> >;
template class FileFormat<AimsData<AimsHSV> >;
template class FileFormat<AimsData<Point3df> >;
template class FileFormat<AimsData<Point3d> >;
template class FileFormat<AimsData<AimsVector<float,6> > >;
template class FileFormat<Volume<AimsRGB> >;
template class FileFormat<Volume<AimsRGBA> >;
template class FileFormat<Volume<AimsHSV> >;
template class FileFormat<Volume<Point3df> >;
template class FileFormat<Volume<Point3d> >;
template class FileFormat<Volume<AimsVector<float,6> > >;

template class FileFormat<VolumeRef<AimsRGB> >;
template class FileFormat<VolumeRef<AimsRGBA> >;
template class FileFormat<VolumeRef<AimsHSV> >;
template class FileFormat<VolumeRef<Point3df> >;
template class FileFormat<VolumeRef<Point3d> >;
template class FileFormat<VolumeRef<AimsVector<float,6> > >;

template class VolumeFormat<AimsRGB>;
template class VolumeFormat<AimsRGBA>;
template class VolumeFormat<AimsHSV>;
template class VolumeFormat<Point3df>;
template class VolumeFormat<Point3d>;
template class VolumeFormat<AimsVector<float,6> >;

template class VolumeRefFormat<AimsRGB>;
template class VolumeRefFormat<AimsRGBA>;
template class VolumeRefFormat<AimsHSV>;
template class VolumeRefFormat<Point3df>;
template class VolumeRefFormat<Point3d>;
template class VolumeRefFormat<AimsVector<float,6> >;

} // namespace aims


static bool _volvecdic()
{
  FileFormatDictionary<AimsData<AimsRGB> >::init();
  FileFormatDictionary<AimsData<AimsRGBA> >::init();
  FileFormatDictionary<AimsData<AimsHSV> >::init();
  FileFormatDictionary<AimsData<Point3df> >::init();
  FileFormatDictionary<AimsData<Point3d> >::init();
  FileFormatDictionary<AimsData<AimsVector<float,6> > >::init();

  FileFormatDictionary<Volume<AimsRGB> >::init();
  FileFormatDictionary<Volume<AimsRGBA> >::init();
  FileFormatDictionary<Volume<AimsHSV> >::init();
  FileFormatDictionary<Volume<Point3df> >::init();
  FileFormatDictionary<Volume<Point3d> >::init();
  FileFormatDictionary<Volume<AimsVector<float,6> > >::init();

  FileFormatDictionary<VolumeRef<AimsRGB> >::init();
  FileFormatDictionary<VolumeRef<AimsRGBA> >::init();
  FileFormatDictionary<VolumeRef<AimsHSV> >::init();
  FileFormatDictionary<VolumeRef<Point3df> >::init();
  FileFormatDictionary<VolumeRef<Point3d> >::init();
  FileFormatDictionary<VolumeRef<AimsVector<float,6> > >::init();

  return true;
}

static bool volvecdic __attribute__((unused)) = _volvecdic();

