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


#ifndef AIMS_ROI_MASKITERATOR_H
#define AIMS_ROI_MASKITERATOR_H

#include <aims/data/data.h>
#include <aims/io/reader.h>
#include <aims/roi/roi.h>
#include <aims/roi/voxelSampling.h>
#include <cartobase/smart/rcptr.h>
#include <cartobase/type/string_conversion.h>
#include <aims/resampling/motion.h>
#include <memory>
#include <string>

namespace aims
{

  struct specifiedLabels
  {
    Point3d firstPoint;
    Point3d lastPoint;
  };

  //---------------------------------------------------------------------------
  class MaskIterator : public carto::RCObject
  {
  public:
    virtual inline ~MaskIterator() {}

    // Returns the current point in voxel referential
    virtual const Point3d &value() const = 0;
    // Returns the current point in image or graph referential
    virtual const Point3df valueMillimeters() const = 0;
    virtual void next() = 0;
    virtual bool isValid() const = 0;
    virtual void restart() = 0;

    // Get voxel size
    virtual const Point3df voxelSize() const = 0;
    // Check if a point (in voxel referential) belongs to the mask
    virtual bool contains( const Point3d & ) const = 0;
    // Check if a point (in millimeter graph/image) belongs to the mask
    virtual bool contains( const Point3df & ) const = 0;
    // Get maximum coordinate
    virtual const Point3d volumeDimension() const = 0;
    virtual std::string regionName() const = 0;
  };


  //---------------------------------------------------------------------------
  class MotionedMaskIterator : public MaskIterator
  {
  public:
    MotionedMaskIterator( const carto::rc_ptr< MaskIterator > &,
                          const Motion & );
    virtual ~MotionedMaskIterator();

    virtual const Point3d &value() const;
    virtual const Point3df valueMillimeters() const;
    virtual void next();
    virtual bool isValid() const;
    virtual void restart();

    virtual const Point3df voxelSize() const;
    virtual bool contains( const Point3d & ) const;
    virtual bool contains( const Point3df & ) const;
    virtual const Point3d volumeDimension() const;
    virtual std::string regionName() const;

  protected:
    carto::rc_ptr< MaskIterator > _maskIterator;
    Motion _motion;
    Motion _inverseMotion;    
  };

  //---------------------------------------------------------------------------
  template <class T>
  class MaskIteratorOf : public MaskIterator
  {
  };


  //---------------------------------------------------------------------------
  class NodeFilter
  {
  public:

    inline virtual ~NodeFilter() {}

    virtual bool filter( const carto::AttributedObject & ) const;
  };


  //---------------------------------------------------------------------------
  class LabelNodeFilter : public NodeFilter
  {
    int _label;

  public:

    LabelNodeFilter( int label ) : _label( label ) {}
    inline virtual ~LabelNodeFilter() {}

    virtual bool filter( const carto::AttributedObject & ) const;
  };


  //---------------------------------------------------------------------------
  class NameNodeFilter : public NodeFilter
  {
    std::string _name;

  public:

    NameNodeFilter( const std::string &name ) : _name( name ) {}
    inline virtual ~NameNodeFilter() {}

    virtual bool filter( const carto::AttributedObject & ) const;
  };


  //---------------------------------------------------------------------------
  template <>
  class MaskIteratorOf< Graph > : public MaskIterator
  {
    const Graph *_roi;
    carto::rc_ptr<Graph> _roiLife;
    Graph::const_iterator _itRoi;
    std::list<std::string> _attributeNames;
    std::list<std::string>::const_iterator _itAttributeNames;
    carto::rc_ptr< BucketMap<Void> > _bucketMap;
    const BucketMap<Void>::Bucket *_bucket;
    BucketMap<Void>::Bucket::const_iterator _itPoints;
    std::auto_ptr< NodeFilter > _nodeFilter;
    carto::rc_ptr< VoxelSampler > _voxelSampler;

    static void 
    _findBucketAttributeNames( const Graph &graph,
			       const Vertex &node,
			       std::list<std::string> &attributeNames );

    Point3df _voxelSize;
    void _getRequiredAttributes();
    
  public:

    MaskIteratorOf( const Graph &roi,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const Graph &roi, int label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const Graph &roi, const std::string &label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const carto::rc_ptr<Graph> &roi,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const carto::rc_ptr<Graph> &roi, int label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const carto::rc_ptr<Graph> &roi,
                    const std::string &label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName, int label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName, const std::string &label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    virtual ~MaskIteratorOf();

    virtual const Point3df voxelSize() const { return _voxelSize; }
    virtual const Point3d &value() const;
    virtual const Point3df valueMillimeters() const;
    virtual void next();
    virtual bool isValid() const;
    virtual void restart();

    virtual bool contains( const Point3d & ) const;
    virtual bool contains( const Point3df & ) const;
    virtual const Point3d volumeDimension() const;
    virtual std::string regionName() const;
  };



  //---------------------------------------------------------------------------
  template <class T>
  class MaskIteratorOf< AimsData<T> > : public MaskIterator
  {
    mutable AimsData<T> *_data;
    bool _freeData;
    Point3d _current;
    T _label;
    Point3d _lastPoint;
    bool _useLabel;
    carto::rc_ptr< VoxelSampler > _voxelSampler;

  public:

    MaskIteratorOf( const AimsData<T> &data,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const AimsData<T> &data, const T &label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const AimsData<T> &data, const specifiedLabels &labels,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName, const T &label,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    MaskIteratorOf( const std::string &fileName, const specifiedLabels &labels,
                    carto::rc_ptr< VoxelSampler > voxelSampler =
                    carto::rc_ptr< VoxelSampler >() );
    virtual ~MaskIteratorOf();

    virtual const Point3df voxelSize() const
    {
      return Point3df( _data->sizeX(), _data->sizeY(), _data->sizeZ() );
    }
    virtual const Point3d &value() const;
    virtual const Point3df valueMillimeters() const;
    virtual void next();
    virtual bool isValid() const;
    virtual void restart();

    virtual bool contains( const Point3d & ) const;
    virtual bool contains( const Point3df & ) const;
    virtual const Point3d volumeDimension() const;
    virtual std::string regionName() const;

  protected:

    virtual void restart( const T &label );
    virtual void restart( const specifiedLabels &labels );

  };




  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const AimsData<T> &data,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( const_cast< AimsData<T> *>( &data ) ),
    _freeData( false ),
    _voxelSampler( voxelSampler )
  {
    _useLabel = false;
    restart();
  }

  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const AimsData<T> &data, const T  &label,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( const_cast< AimsData<T> *>( &data ) ),
    _freeData( false ),
    _voxelSampler( voxelSampler )
  {
    restart( label );
  }

  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const AimsData<T> &data, const specifiedLabels &labels,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( const_cast< AimsData<T> *>( &data ) ),
    _freeData( false ),
    _voxelSampler( voxelSampler )
  {
    restart( labels );
  }

  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const std::string &fileName,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( new AimsData<T> ),
    _freeData( true ),
    _voxelSampler( voxelSampler )
  {
    Reader< AimsData<T> > reader( fileName );
    reader.read( *_data );
    _useLabel = false;
    restart();
  }

  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const std::string &fileName, const T &label,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( new AimsData<T> ),
    _freeData( true ),
    _voxelSampler( voxelSampler )
  {
    Reader< AimsData<T> > reader( fileName );
    reader.read( *_data );
    restart( label );
  }


  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::
  MaskIteratorOf( const std::string &fileName, const specifiedLabels &labels,
                  carto::rc_ptr< VoxelSampler > voxelSampler ) :
    _data( new AimsData<T> ),
    _freeData( true ),
    _voxelSampler( voxelSampler )
  {
    Reader< AimsData<T> > reader( fileName );
    reader.read( *_data );
    restart( labels );
  }

  //---------------------------------------------------------------------------
  template <class T>
  MaskIteratorOf< AimsData<T> >::~MaskIteratorOf()
  {
    if ( _freeData ) delete _data;
  }


  //---------------------------------------------------------------------------
  template <class T>
  const Point3d &MaskIteratorOf< AimsData<T> >::value() const
  {
    return _current;
  }

  //---------------------------------------------------------------------------
  template <class T>
  const Point3df MaskIteratorOf< AimsData<T> >::valueMillimeters() const
  {
    if ( _voxelSampler.isNull() ) {
      return Point3df( _current[0] * _data->sizeX(),
                       _current[1] * _data->sizeY(),
                       _current[2] * _data->sizeZ() );
    } else {
      const Point3df &voxelSample = _voxelSampler->value();
      return Point3df( ( _current[0] + voxelSample[ 0 ] ) * _data->sizeX(),
                       ( _current[1] + voxelSample[ 1 ] ) * _data->sizeY(),
                       ( _current[2] + voxelSample[ 2 ] ) * _data->sizeZ() );
    }
  }

  //---------------------------------------------------------------------------
  template <class T>
  void MaskIteratorOf< AimsData<T> >::next()
  {
    if ( (_useLabel && (*_data)( _current ) == (T) _label) ||
      (!_useLabel && (*_data)( _current  ) ) )
    {
      // Next sample in voxel
      if ( ! _voxelSampler.isNull() ) 
      {
        _voxelSampler->next();
        if ( _voxelSampler->isValid() ) 
          return;
      }
    }
    
    if ( ! _voxelSampler.isNull() )
    {
      _voxelSampler->restart();
    }
    
    if ( _current == _lastPoint ) // (to make it not valid )
    {
      _current[ 0 ] = _data->dimX();
      _current[ 1 ] = _data->dimY();
      _current[ 2 ] = _data->dimZ();
    }
    // Next point
    do {
      ++_current[ 0 ];
      if ( _current[ 0 ] == _data->dimX() ) {
        _current[ 0 ] = 0;
        ++_current[ 1 ];
        if ( _current[ 1 ] == _data->dimY() ) {
          _current[ 1 ] = 0;
          ++_current[ 2 ];
        }
      }
    } while( isValid() && ( _useLabel ? 
                            (*_data)( _current ) != (T) _label :
                            ! (*_data)( _current  ) ) );
  }


  //---------------------------------------------------------------------------
  template <class T>
  bool MaskIteratorOf< AimsData<T> >::isValid() const
  {
    return _current[ 2 ] < _data->dimZ();
  }

  //---------------------------------------------------------------------------
  template <class T>
  void MaskIteratorOf< AimsData<T> >::restart()
  {
    if ( ! _voxelSampler.isNull() ) 
    {
      _voxelSampler->restart();
    }
    
    _current[ 0 ] = _current[ 1 ] = _current[ 2 ] = 0;
    if ( isValid() &&  ( _useLabel ? 
                         (*_data)( _current ) != (T) _label :
                         ! (*_data)( _current  ) ) ) next();
  }

  //---------------------------------------------------------------------------
  template <class T>
  void MaskIteratorOf< AimsData<T> >::restart( const T &label )
  {
    if ( ! _voxelSampler.isNull() ) 
    {
      _voxelSampler->restart();
    }
    
    _label = label;
    _useLabel = true;
    _current[ 0 ] = _current[ 1 ] = _current[ 2 ] = 0;
    if ( isValid() && (  (*_data)( _current ) != _label ) )
    { 
      next();
    }
  }

  //---------------------------------------------------------------------------
  template <class T>
  void MaskIteratorOf< AimsData<T> >::restart( const specifiedLabels &labels )
  {
    if ( ! _voxelSampler.isNull() ) 
    {
      _voxelSampler->restart();
    }
    
    _label = (*_data)( labels.firstPoint );
    _current = labels.firstPoint;
    _lastPoint = labels.lastPoint;
  }


  //---------------------------------------------------------------------------
  template <class T>
  bool MaskIteratorOf< AimsData<T> >::contains( const Point3d &p ) const
  {
    if ( p[ 0 ] >= 0 && p[ 0 ] < _data->dimX() &&
	 p[ 1 ] >= 0 && p[ 1 ] < _data->dimY() &&
	 p[ 2 ] >= 0 && p[ 2 ] < _data->dimZ() ) {
      if ( _useLabel ) {
	return (*_data)( p ) == _label;
      } else {
	return (*_data)( p );
      }
    }
    return false;
  }

  //---------------------------------------------------------------------------
  template <class T>
  bool MaskIteratorOf< AimsData<T> >::contains( const Point3df &p ) const
  {
    const Point3d pixel( (short) rint( p[ 0 ] / _data->sizeX() ),
			 (short) rint( p[ 1 ] / _data->sizeY() ),
			 (short) rint( p[ 2 ] / _data->sizeZ() ) );
    return contains( pixel );
  }


  //---------------------------------------------------------------------------
  template <class T>
  const Point3d MaskIteratorOf< AimsData<T> >::volumeDimension() const
  {
    return Point3d( _data->dimX(), _data->dimY(), _data->dimZ() );
  }


  //---------------------------------------------------------------------------
  template <class T>
  std::string MaskIteratorOf< AimsData<T> >::regionName() const
  {
    if ( _useLabel ) return carto::toString( _label );
    return "unknown";
  }


  //---------------------------------------------------------------------------
  // maskIteratorFactories (to be continued)

  //---------------------------------------------------------------------------
  carto::rc_ptr< MaskIterator > getMaskIterator( const std::string &fileName );

  //---------------------------------------------------------------------------
  carto::rc_ptr< MaskIterator > 
  getMaskIterator( const std::string &fileName,
                   carto::rc_ptr< VoxelSampler > voxelSampler );

  //---------------------------------------------------------------------------
  template <class T>
  carto::rc_ptr< MaskIterator > getMaskIterator( const AimsData< T > &data )
  {
    return carto::rc_ptr< MaskIterator >
      ( new MaskIteratorOf< AimsData<T> >( data ) );
  }

  //---------------------------------------------------------------------------
  template <class T>
  carto::rc_ptr< MaskIterator >
  getMaskIterator( const AimsData< T > &data,
                   carto::rc_ptr< VoxelSampler > voxelSampler )
  {
    return carto::rc_ptr< MaskIterator >
      ( new MaskIteratorOf< AimsData<T> >( data, voxelSampler ) );
  }

  //---------------------------------------------------------------------------
  carto::rc_ptr< MaskIterator > getMaskIterator( const std::string &fileName,
                                                 const Motion &motion );

  //---------------------------------------------------------------------------
  carto::rc_ptr< MaskIterator >
  getMaskIterator( const std::string &fileName,
                   carto::rc_ptr< VoxelSampler > voxelSampler,
                   const Motion &motion );

  //---------------------------------------------------------------------------
  template <class T>
  carto::rc_ptr< MaskIterator > getMaskIterator( const AimsData< T > &data,
                                                 const Motion &motion )
  {
    return carto::rc_ptr< MaskIterator >
      ( new MotionedMaskIterator( getMaskIterator( data ), motion ) );
  }

  //---------------------------------------------------------------------------
  template <class T>
  carto::rc_ptr< MaskIterator >
  getMaskIterator( const AimsData< T > &data,
                   carto::rc_ptr< VoxelSampler > voxelSampler,
                   const Motion &motion )
  {
    return carto::rc_ptr< MaskIterator >
      ( new MotionedMaskIterator( getMaskIterator( data, voxelSampler ), 
                                  motion ) );
  }


} // namespace aims

#endif
