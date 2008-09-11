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

#ifndef CARTODATA_IO_VOLUMEDATASOURCE_H
#define CARTODATA_IO_VOLUMEDATASOURCE_H

#include <cartodata/volume/volumeproxy.h>
#include <cartobase/datasource/chaindatasource.h>
#include <cartobase/object/object.h>
#include <cartobase/wip/io/itemreader.h>
#include <cartobase/wip/io/itemwriter.h>
#include <memory>

namespace carto
{

  template <typename T> 
  class VolumeDataSource : public VolumeProxy<T>, public ChainDataSource
  {
  public:
    VolumeDataSource( rc_ptr<DataSource> ds, Object header, 
                      const std::string & url );
    virtual ~VolumeDataSource();

    virtual DataSource* clone() const;
    virtual int iterateMode() const;
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char * data, unsigned long maxlen );
    virtual long writeBlock( const char * data, unsigned long len );
    virtual int getch();
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    virtual bool allowsMemoryMapping() const;

    virtual bool setpos( int x, int y = 0, int z = 0, int t = 0 );

  private:
    void updateParams();

    bool				_binary;
    bool				_byteswap;
    std::auto_ptr<ItemReader<T> >	_itemr;
    std::auto_ptr<ItemWriter<T> >	_itemw;
  };

}

#endif

