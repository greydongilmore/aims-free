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
#ifndef CARTODATA_IO_VOLUMEUTILIO_H
#define CARTODATA_IO_VOLUMEUTILIO_H

//--- cartodata ----------------------------------------------------------------
#include <cartodata/volume/volume.h>
//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>                          // #define soma
#include <soma-io/writer/pythonwriter.h>                         // print header
//--- cartobase ----------------------------------------------------------------
#include <cartobase/config/verbose.h>                             // printheader
#include <cartobase/object/object.h>
//--- system -------------------------------------------------------------------
#include <iostream>                                               // printheader
#include <vector>                                                  // 4D vectors
//------------------------------------------------------------------------------

namespace soma {

  template <typename T>
  class VolumeUtilIO
  {
    public:
      //=== READ ===============================================================
      /// list of properties triggering partial reading and/or borders
      static std::set<std::string> listReadProperties();

      /// Manages all the volumes necessary and returns the final VolumeView
      /// 
      /// This method is called from VolumeFormatReader::createAndRead(). 
      /// It reads all the parameters from options' properties and 
      /// removes them afterwards. It then performs necessary allocation
      /// and reading and returns obj as a fully usable VolumeView.
      static carto::Volume<T>*
      read( //carto::Volume<T>* obj,
            carto::rc_ptr<DataSourceInfo> dsi,
            carto::Object options );

      /// Worker for partial reading case
      static carto::Volume<T>* 
      readPartial( //carto::Volume<T>* obj, 
                   carto::rc_ptr<DataSourceInfo> dsi,
                   std::vector<int> position, std::vector<int> frame,
                   std::vector<int> borders, carto::Object options );

      /// Worker for full reading case
      static carto::Volume<T>* 
      readFull( //carto::Volume<T>* obj,
                carto::rc_ptr<DataSourceInfo> dsi,
                std::vector<int> borders, carto::Object options );
  };

  //============================================================================
  //   U T I L I T I E S
  //============================================================================
  // to be moved/deleted

  inline void
  printheader( int verbose, carto::Object hdr )
  {
    if( verbose <= carto::debugMessageLevel ) {
      soma::PythonWriter  pw;
      pw.attach( std::cout );
      pw.write( hdr );
    }
  }
}

#endif