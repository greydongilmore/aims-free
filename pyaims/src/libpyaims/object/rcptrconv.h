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

#ifndef CARTO_PYAIMS_OBJECT_RCPTRCONV_H
#define CARTO_PYAIMS_OBJECT_RCPTRCONV_H

#include <sip.h>
#include <cartobase/smart/sharedptr.h>

namespace pyaims
{

  template <typename T> inline
  T* fromT( PyObject * sipPy, sipWrapperType* tclass, PyObject *transferObj );

  template <typename T, typename RcT> inline
  T* fromRcptr( PyObject * sipPy, sipWrapperType* rcclass,
                PyObject *transferObj );

  template <typename T> inline
  T* fromRcptrs( PyObject * sipPy, PyObject* tclass, PyObject* rcclass,
                 PyObject* wsclass, PyObject* wkclass, PyObject *transferObj );

  inline PyObject* extractPyObjectFromProxy( PyObject* sipPy );
  inline int canConvertFromProxy( PyObject *sipPy, sipWrapperType* tclass );

  template <typename T> inline
  T* fromProxy( PyObject * sipPy, sipWrapperType* tclass,
                PyObject *transferObj, int *state, int *iserr );

  template <typename T> inline
  int standardConvertToTypeCode( PyObject* sipPy, sipWrapperType* tclass,
                                 PyObject *sipTransferObj, int *sipIsErr,
                                 T** sipCppPtr );

  // ---

  template <typename T> inline
  T* fromT( PyObject * sipPy, sipWrapperType* tclass, PyObject *transferObj )
  {
    if( sipCanConvertToInstance( sipPy, tclass, SIP_NO_CONVERTORS ) )
    {
      int state = 0;
      int sipIsErr = 0;
      void *obj = sipConvertToInstance( sipPy, tclass, transferObj,
                                        SIP_NO_CONVERTORS, &state,
                                        &sipIsErr );
      /* *don't* call sipReleaseInstance here because we are not done
      with the object. But sipReleaseInstance should be called later on it
      (normally internally by sip).
      */
      return reinterpret_cast<T *>( obj );
    }
    return 0;
  }

  template <typename T, typename RcT> inline 
  T* fromRcptr( PyObject * sipPy, sipWrapperType* rcclass,
                PyObject *transferObj )
  {
    if( sipCanConvertToInstance( sipPy, rcclass, SIP_NO_CONVERTORS ) )
    {
      int state = 0;
      int sipIsErr = 0;
      void *obj = sipConvertToInstance( sipPy, rcclass, transferObj,
                                        SIP_NO_CONVERTORS, &state, &sipIsErr );
      RcT *robj = reinterpret_cast<RcT *>( obj );
      /* release rc_ptr instance after using sipConvertToInstance: allow it
      to be deleted later.
      We don't need the rc_ptr instance any longer, but its reference count
      must not go down to 0 otherwise the contained T is deleted.
      However it should not be such if we get an existing rc_ptr.
      So we *must* use SIP_NO_CONVERTORS here.
      */
      sipReleaseInstance( obj, rcclass, state );
      return robj->get();
    }
    return 0;
  }


  template <typename T> inline
  T* fromRcptrs( PyObject * sipPy, PyObject* tclass, PyObject* rcclass,
                 PyObject* wsclass, PyObject* wkclass, PyObject *transferObj )
  {
    return fromT<T>( sipPy, tclass, transferObj )
        || fromRcptr<T, carto::rc_ptr<T> >( sipPy, rcclass, transferObj )
        || fromRcptr<T, carto::weak_shared_ptr<T> >( sipPy, wsclass,
            transferObj )
        || fromRcptr<T, carto::weak_ptr<T> >( sipPy, wkclass, transferObj );
  }


  inline PyObject* extractPyObjectFromProxy( PyObject* sipPy )
  {
    if( !PyObject_HasAttrString( sipPy, "get" ) )
      return 0;
    PyObject* pointee = PyObject_CallMethod( sipPy, (char *) "get", NULL );
    if( !pointee )
    {
      PyErr_Clear();
      return 0;
    }
    return pointee;
  }


  inline int canConvertFromProxy( PyObject *sipPy, sipWrapperType* tclass )
  {
    PyObject *inside = extractPyObjectFromProxy( sipPy );
    if( !inside )
      return 0;
    int res = sipCanConvertToInstance( inside, tclass, SIP_NOT_NONE );
    Py_DECREF( inside );
    return res;
  }


  template <typename T> inline
  T* fromProxy( PyObject * sipPy, sipWrapperType* tclass,
                PyObject *transferObj, int *state, int *iserr )
  {
    PyObject *inside = extractPyObjectFromProxy( sipPy );
    if( !inside )
      return 0;
    int res = sipCanConvertToInstance( inside, tclass, SIP_NOT_NONE );
    if( !res )
    {
      Py_DECREF( inside );
      return 0;
    }
    T *sipRes = (T*) sipConvertToInstance( inside, tclass, transferObj,
                                           SIP_NOT_NONE, state, iserr );
    Py_DECREF( inside );
    if( *iserr )
    {
      sipReleaseInstance( sipRes, tclass, *state );
      sipRes = 0;
    }
    return sipRes;
  }


  template <typename T> inline
  int standardConvertToTypeCode( PyObject* sipPy, sipWrapperType* tclass,
                                 PyObject *sipTransferObj, int *sipIsErr,
                                 T** sipCppPtr )
  {
    if( !sipIsErr )
      return sipCanConvertToInstance( sipPy, tclass,
                                      SIP_NOT_NONE | SIP_NO_CONVERTORS )
        || pyaims::canConvertFromProxy( sipPy, tclass );

    if( sipPy == Py_None )
    {
      *sipCppPtr = 0;
      return 0;
    }

    int state = 0;

    T * dat = (T *) sipForceConvertToInstance( sipPy, tclass, sipTransferObj,
                                               SIP_NO_CONVERTORS, &state,
                                               sipIsErr );
    if( dat && *sipIsErr )
    {
      sipReleaseInstance( dat, tclass, state );
      dat = 0;
    }
    else if( dat )
    {
      *sipCppPtr = dat;
      // return 0; //sipGetState(sipTransferObj);
      // Still don't understand this state/return codes, Denis 2011/12/20
      return state;
    }

    *sipIsErr = 0;
    *sipCppPtr = fromProxy<T>( sipPy, tclass, sipTransferObj, &state,
                               sipIsErr );
    if( !*sipIsErr )
    {
      PyErr_Clear();
      // return 0; //sipGetState(sipTransferObj);
      return state;
    }
    return 0;
  }

}

#endif

