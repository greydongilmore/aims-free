
%#define PyAims_Volume_%Template1typecode%_defined%

class Volume_%Template1typecode% : carto::RCObject
{
%TypeHeaderCode
#include <cartodata/volume/volume.h>
%Template1typeinclude%
typedef carto::Volume<%Template1% > Volume_%Template1typecode%;
#include <cartodata/volume/volumeoperators.h>
#include <weakrefobject.h>
#define PY_ARRAY_TYPES_PREFIX PyAIMSTypes
%End

%TypeCode
#include <iostream>
#include <pyaims/vector/numpyarrayfunc.h>
#include <pyaims/object/rcptrconv.h>
%Template1sipinclude%
%End

%ConvertToTypeCode
  return pyaims::standardConvertToTypeCode( sipPy,
    sipClass_Volume_%Template1typecode%,
    sipTransferObj, sipIsErr, sipCppPtr );
%End


public:
    class Position4Di
    {
%Docstring
4 ints for position or size, used for Volume views.
%End
    public:
      Position4Di( int x = 0, int y = 0, int z = 0, int t = 0 );
      Position4Di( const Volume_%Template1typecode%::Position4Di & );
      ~Position4Di();
      int operator [] ( int coord ) const;
      int __len__() const;
%MethodCode
      sipRes = 4;
%End
    };

    Volume_%Template1typecode%( int = 1, int = 1, int = 1, int = 1,
      const carto::AllocatorContext& allocatorContext
        = carto::AllocatorContext(), bool = true ) /ReleaseGIL/;
    Volume_%Template1typecode%( int sizeX, int sizeY, int sizeZ,
      int sizeT, int bordersize,
      const carto::AllocatorContext& allocatorContext
      = carto::AllocatorContext(), bool allocated = true ) /ReleaseGIL/;
    Volume_%Template1typecode%( const Volume_%Template1typecode% & )
      /ReleaseGIL/;
    Volume_%Template1typecode%( rc_ptr_Volume_%Template1typecode% other,
      const Volume_%Template1typecode%::Position4Di & pos
        = Volume_%Template1typecode%::Position4Di( 0, 0, 0, 0 ),
      const Volume_%Template1typecode%::Position4Di & size
        = Volume_%Template1typecode%::Position4Di( -1, -1, -1, -1 ),
      const carto::AllocatorContext & allocContext
        = carto::AllocatorContext() );

    virtual ~Volume_%Template1typecode%() /ReleaseGIL/;

    int getSizeX() const;
    int getSizeY() const;
    int getSizeZ() const;
    int getSizeT() const;
    vector_S32 getSize() const;
%Docstring
Number of voxels in each dimension (list of 4 ints)
%End

    vector_FLOAT getVoxelSize() const;
%Docstring
Voxel sizes in mm (list of 4 floats)
%End

    virtual void copyHeaderFrom( const carto::PropertySet & other );
    virtual void copyHeaderFrom( const carto::Object & other );

    carto::AllocatorContext & allocatorContext();
%MethodCode
    // handle constness of C++ returned object
    // and the fact that a copied context is always unallocated
    sipRes = const_cast<carto::AllocatorContext *>(
        &sipCpp->allocatorContext() );
    // Problem: the returned object is actually modifiable, which is not safe.
%End

    void fill( const %Template1% & value ) /ReleaseGIL/;
    bool all() const;
    bool any() const;
    %Template1PyType% min() const;
    %Template1PyType% max() const;

    rc_ptr_Volume_%Template1typecode% refVolume() const;
%Docstring
If the volume is a view into another (larger) one, this returns the "parent" one.
%End

    Volume_%Template1typecode%::Position4Di posInRefVolume() const;
%Docstring
If the volume is a view into another (larger) one, this returns the position in "parent" one.
%End

  %Template1PyType% at( long, long = 0, long = 0, long = 0 ) const;
%Docstring
at(posx, posy=0, posz=0, post=0)

Returns the volume value for the selected voxel.
%End

  %Template1PyType% value( long, long = 0, long = 0,
                           long = 0 ) const;
%Docstring
value(posx, posy=0, posz=0, post=0)

value is an alias to :py:meth:`at`: returns the volume value for the selected voxel.
%End

%MethodCode
 sipRes = %Template1new%( sipCpp->at( a0, a1, a2, a3 ) );
%End

 void setValue( %Template1%, long, long = 0, long = 0,
                long = 0 );
%Docstring
setValue(value, x, y=0, z=0, t=0)

Set the voxel value at the given position.
%End

%MethodCode
 sipCpp->at( a1, a2, a3, a4 ) = %Template1deref%a0;
%End

%#ifdef PyAims_Volume_U8_defined%
 void setValue( int, long, long = 0, long = 0, long = 0 );
%Docstring
Set a voxel value at given position
%End
%MethodCode
 sipCpp->at( a1, a2, a3, a4 ) = %Template1deref%a0;
%End
%#endif%

  SIP_PYOBJECT header() /Factory/;
%Docstring
The header contains all meta-data.
%End
%MethodCode
  carto::PropertySet	& ps = sipCpp->header();
  carto::Object* h = new carto::Object( carto::Object::reference( ps ) );
  sipRes = sipConvertFromNewType( h, sipType_carto_Object, 0 );
  // set into header a reference to the volume to forbid it to die before the
  // python header object
  if( PyObject_SetAttrString( sipRes, "_volref", sipSelf ) == -1 )
  {
    std::cerr << "cannot set header ._volref" << std::endl;
  }
%End

  Volume_%Template1typecode%* operator + ( Volume_%Template1typecode% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r2( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 + r2;
  r1.release();
  r2.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator += ( Volume_%Template1typecode% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r2( a0 );
  r1 += r2;
  r1.release();
  r2.release();
%End

  Volume_%Template1typecode%* operator + ( const %Template1% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 + %Template1deref%a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator += ( const %Template1% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 += %Template1deref%a0;
  r1.release();
%End

/*
  Volume_%Template1typecode%* __radd__( const %Template1% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = %Template1deref%a0 + r1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End
*/

  Volume_%Template1typecode%* __neg__() const /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = -r1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  Volume_%Template1typecode%* operator - ( Volume_%Template1typecode% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r2( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 - r2;
  r1.release();
  r2.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator -= ( Volume_%Template1typecode% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r2( a0 );
  r1 -= r2;
  r1.release();
  r2.release();
%End

  Volume_%Template1typecode%* operator - ( const %Template1% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 - %Template1deref%a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator -= ( const %Template1% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 -= %Template1deref%a0;
  r1.release();
%End

/*
  Volume_%Template1typecode%* __rsub__( const %Template1% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = %Template1deref%a0 - r1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End
*/

  Volume_%Template1typecode%* operator * ( double ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 * a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  Volume_%Template1typecode%* operator * ( long ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 * a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator *= ( double ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 *= a0;
  r1.release();
%End

  void operator *= ( long ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 *= a0;
  r1.release();
%End

/*
  Volume_%Template1typecode%* __rmul__( double ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = r1 * a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  Volume_%Template1typecode%* __rmul__( long ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = r1 * a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End
*/

  Volume_%Template1typecode%* operator / ( double ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 / a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  Volume_%Template1typecode%* operator / ( long ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r3 = r1 / a1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator /= ( double ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 /= a0;
  r1.release();
%End

  void operator /= ( long ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  r1 /= a0;
  r1.release();
%End

%%Template1defScalar%%
%#ifdef PYAIMS_SCALAR%

  Volume_%Template1typecode%* operator * ( Volume_%Template1typecode% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r2( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 * r2;
  r1.release();
  r2.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator *= ( Volume_%Template1typecode% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r2( a0 );
  r1 *= r2;
  r1.release();
  r2.release();
%End

  Volume_%Template1typecode%* operator / ( Volume_%Template1typecode% & )
    /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a0 );
  carto::VolumeRef<%Template1% > r2( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 / r2;
  r1.release();
  r2.release();
  sipRes = r3.get();
  r3.release();
%End

  void operator /= ( Volume_%Template1typecode% & ) /ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r2( a0 );
  r1 /= r2;
  r1.release();
  r2.release();
%End

/*
  Volume_%Template1typecode%* __rdiv__( double ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( sipCpp );
  carto::VolumeRef<%Template1% > r3 = a0 / r1;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End
*/

%#endif%

// numpy bindings

%%Template1defNumpyBindings%%
%#if defined( PYAIMS_SCALAR ) || defined( PYAIMS_NUMPY_BINDINGS) %

  Volume_%Template1typecode%( SIP_PYOBJECT )
    [(int, int, int, int, %Template1% *)];
%MethodCode
  static std::list<std::set<int> > compatibletypes;
  if( compatibletypes.empty() )
  {
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_BYTE );
      tl.insert( PyArray_INT8 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_UBYTE );
      tl.insert( PyArray_UINT8 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_SHORT );
      tl.insert( PyArray_INT16 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_USHORT );
      tl.insert( PyArray_UINT16 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_INT32 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      // tl.insert( PyArray_UINT );
      tl.insert( PyArray_UINT32 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      // tl.insert( PyArray_INT );
      tl.insert( PyArray_INT64 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_UINT );
      tl.insert( PyArray_UINT64 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      tl.insert( PyArray_FLOAT32 );
    }
    compatibletypes.push_back( std::set<int>() );
    {
      std::set<int> & tl = compatibletypes.back();
      // tl.insert( PyArray_FLOAT );
      tl.insert( PyArray_DOUBLE );
      tl.insert( PyArray_FLOAT64 );
    }
  }
  PyArrayObject *arr = 0;
  if( !PyArray_Check( a0 ) )
  {
    sipIsErr = 1;
    PyErr_SetString( PyExc_TypeError, "wrong argument type" );
  }
  else
  {
    arr = (PyArrayObject *) a0;

    /* I comment this out because transposed arrays are not seen as contiguous
    if( !PyArray_ISCONTIGUOUS( arr ) )
    {
      sipIsErr = 1;
      PyErr_SetString( PyExc_RuntimeError,
                       "Building a Volume from a non-contiguous array is not "
                       "supported" );
    }
    else
    */

    if( arr->nd < 0 || arr->nd >4 )
    {
      sipIsErr = 1;
      PyErr_SetString( PyExc_RuntimeError,
                       "Array dimensions are not compatible with Volume" );
    }
    else if( arr->descr->type_num != %Template1NumType% )
    {
      std::list<std::set<int> >::const_iterator ict, ect = compatibletypes.end();
      bool ok = false;
      int i = 0;
      for( ict=compatibletypes.begin(); ict!=ect; ++ict, ++i )
        if( ict->find( arr->descr->type_num ) != ict->end() )
        {
          if( ict->find( %Template1NumType% ) != ict->end() )
          {
            ok = true;
            break;
          }
          else
            break;
        }
      if( !ok )
      {
        sipIsErr = 1;
        PyErr_SetString( PyExc_TypeError, "wrong array data type" );
      }
    }
  }
  if( !sipIsErr )
  {
    // retreive dimensions
    int dims[4] = { 1, 1, 1, 1 };
    int inc = 1, start = 0;
    // TODO: retreive exact strides and react accordingly
    if( PyArray_NDIM( arr ) >= 2 )
    {
      if( PyArray_STRIDES( arr )[1] < PyArray_STRIDES( arr )[0] )
      {
        // increments higher index first
        inc = -1;
        start = arr->nd-1;
      }
    }
    dims[0] = arr->dimensions[ start ];
    if( arr->nd >= 2 )
    {
      dims[1] = arr->dimensions[ start + inc ];
      if( arr->nd >= 3 )
        {
          dims[2] = arr->dimensions[ start + inc * 2 ];
          if( arr->nd >= 4 )
            dims[3] = arr->dimensions[ start + inc * 3 ];
        }
    }

    sipCpp = new sipVolume_%Template1typecode%( dims[0], dims[1], dims[2],
                                                dims[3],
                                                ( %Template1% *) arr->data );
    // keep ref to the array to prevent its destruction
    PyObject_SetAttrString( (PyObject *) sipSelf, "_arrayext", a0 );
  }
  else
  {
    sipIsErr = 1;
    sipCpp = 0;
  }

%End

  SIP_PYOBJECT arraydata() /Factory/;
%Docstring
.. note::

    *arraydata()* returns a numpy array to the internal memory block, without strides. Given the internal ordering of Aims Volumes, the resulting numpy array is indexed as [t][z][y][x]. This order corresponds to the numpy "fortran" order: ``order='F'``
    If you need the inverse, more natural, [x][y][z][t] ordering, use the following:

    >>> volarray = numpy.array(volume, copy=False)

    or:

    >>> volarray = numpy.asarray(volume)

.. note::

    The array conversion is currently only supported for scalar volumes, and is not present on volumes of types RGB or RGBA.
%End

%MethodCode
  std::vector<int> dims(4);
  dims[3] = sipCpp->getSizeX();
  dims[2] = sipCpp->getSizeY();
  dims[1] = sipCpp->getSizeZ();
  dims[0] = sipCpp->getSizeT();
  size_t strides[4];
  strides[3] = sizeof( %Template1% );
  carto::rc_ptr<Volume_%Template1typecode% > ref = sipCpp->refVolume();
  if( ref.get() && ref->allocatorContext().isAllocated() )
  {
    strides[2] = strides[3] * ref->getSizeX();
    strides[1] = strides[2] * ref->getSizeY();
    strides[0] = strides[1] * ref->getSizeZ();
  }
  else
  {
    strides[2] = strides[3] * dims[3];
    strides[1] = strides[2] * dims[2];
    strides[0] = strides[1] * dims[1];
  }
  sipRes = aims::initNumpyArray( sipSelf, %Template1NumType%, 4, &dims[0],
                                 (char *) &sipCpp->at( 0 ), false, strides );
%End

  SIP_PYOBJECT __array__() /Factory/;
%MethodCode
  std::vector<int> dims(4);
  dims[3] = sipCpp->getSizeX();
  dims[2] = sipCpp->getSizeY();
  dims[1] = sipCpp->getSizeZ();
  dims[0] = sipCpp->getSizeT();
  size_t strides[4];
  strides[3] = sizeof( %Template1% );
  carto::rc_ptr<Volume_%Template1typecode% > ref = sipCpp->refVolume();
  if( ref.get() && ref->allocatorContext().isAllocated() )
  {
    strides[2] = strides[3] * ref->getSizeX();
    strides[1] = strides[2] * ref->getSizeY();
    strides[0] = strides[1] * ref->getSizeZ();
  }
  else
  {
    strides[2] = strides[3] * dims[3];
    strides[1] = strides[2] * dims[2];
    strides[0] = strides[1] * dims[1];
  }
  sipRes = aims::initNumpyArray( sipSelf, %Template1NumType%, 4, &dims[0],
                                 (char *) &sipCpp->at( 0 ), true, strides );
%End

  void checkResize();
%MethodCode
  std::vector<int> dims(4);
  dims[3] = sipCpp->getSizeX();
  dims[2] = sipCpp->getSizeY();
  dims[1] = sipCpp->getSizeZ();
  dims[0] = sipCpp->getSizeT();
  size_t strides[4];
  strides[3] = sizeof( %Template1% );
  carto::rc_ptr<Volume_%Template1typecode% > ref = sipCpp->refVolume();
  if( ref.get() && ref->allocatorContext().isAllocated() )
  {
    strides[2] = strides[3] * ref->getSizeX();
    strides[1] = strides[2] * ref->getSizeY();
    strides[0] = strides[1] * ref->getSizeZ();
  }
  else
  {
    strides[2] = strides[3] * dims[3];
    strides[1] = strides[2] * dims[2];
    strides[0] = strides[1] * dims[1];
  }
  aims::resizeNumpyArray( sipSelf, 4, &dims[0], (char *) &sipCpp->at( 0 ),
    strides );
%End

  void _arrayDestroyedCallback( SIP_PYOBJECT );
%MethodCode
  // remove weak reference in volume
  PyObject_DelAttrString( sipSelf, "_arrayref" );
  // dec reference to self that was manually incremented in arraydata()
  // when building the numpy array
  Py_DECREF( sipSelf );
  // the aditional ref to self will be deleted when the callback
  // (method bound on self) is destroyed, just after now
  // so self can be destroyed safely
%End

%#endif%

};


%#ifdef PYAIMS_SCALAR%

Volume_%Template1typecode% * operator + ( %Template1%, Volume_%Template1typecode% & ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 + a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

Volume_%Template1typecode% * operator - ( %Template1%, Volume_%Template1typecode% & ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a1 );
  carto::VolumeRef<%Template1% > r3 = -r1 + a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

Volume_%Template1typecode% * operator * ( double, Volume_%Template1typecode% & ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 * a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

Volume_%Template1typecode% * operator * ( long, Volume_%Template1typecode% & ) /Factory, ReleaseGIL/;
%MethodCode
  carto::VolumeRef<%Template1% > r1( a1 );
  carto::VolumeRef<%Template1% > r3 = r1 * a0;
  r1.release();
  sipRes = r3.get();
  r3.release();
%End

%#endif%

%#undef PyAims_Volume_%Template1typecode%_defined%

