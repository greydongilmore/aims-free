# -*- coding: utf-8 -*-
#  This software and supporting documentation are distributed by
#      Institut Federatif de Recherche 49
#      CEA/NeuroSpin, Batiment 145,
#      91191 Gif-sur-Yvette cedex
#      France
#
# This software is governed by the CeCILL-B license under
# French law and abiding by the rules of distribution of free software.
# You can  use, modify and/or redistribute the software under the
# terms of the CeCILL-B license as circulated by CEA, CNRS
# and INRIA at the following URL "http://www.cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-B license and that you accept its terms.

from __future__ import absolute_import
import sys
import platform
import sip

def classInNamespace(include, cls, namespace, cppnamespace=None,
                     cppclass=None):
    if cppnamespace is None:
        cppnamespace = namespace
    if cppclass is None:
        cppclass = cls
    return {'typecode': cls,
            'pyFromC': 'pyaimsConvertFrom_' + namespace + '_' + cls,
            'CFromPy': 'pyaimsConvertTo_' + namespace + '_' + cls,
            'castFromSip': '(' + cppnamespace + '::' + cppclass + ' *)',
            'deref': '*',
            'pyderef': '*',
            'address': '&',
            'pyaddress': '&',
            'defScalar': '',
            'defNumpyBindings': '',
            'new': 'new ' + cppnamespace + '::' + cppclass,
            'NumType': 'NPY_OBJECT',
            'PyType': cppnamespace + '::' + cppclass,
            'sipClass': namespace + '_' + cls,
            'typeinclude':
            '#include <' + include + '>',
            'sipinclude': '#ifndef PYAIMS_' + namespace.upper() + '_' + cls.upper() + '_CHECK_DEFINED\n'
            '#define PYAIMS_' +
            namespace.upper() + '_' + cls.upper() + '_CHECK_DEFINED\n'
            'inline int pyaims' + cls + '_Check( PyObject* o )\n'
            '{ return sipCanConvertToType( o, sipFindType( "' + namespace +
            '_' + cls + '" )'
            ', SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
            '#endif\n'
            '#ifndef PYAIMS_' +
            namespace.upper() + '_' + cls.upper() + '_CONVERT_DEFINED\n'
            '#define PYAIMS_' +
            namespace.upper() + '_' + cls.upper() + '_CONVERT_DEFINED\n'
            'inline void* pyaimsConvertTo_' +
            namespace + '_' + cls + '( PyObject* o )\n'
            '{ int iserr = 0;\n'
            '  void *ptr = sipForceConvertToType( o, sipFindType( "' +
            namespace + '_' + cls +
            '" ), 0, 0, 0, &iserr );\n'
            '  if( iserr ) return 0;\n'
            '  return ptr;\n}\n'
            'inline PyObject* pyaimsConvertFrom_' +
            namespace + '_' + cls + '( void * a )\n'
            '{ return sipConvertFromType( a, sipFindType( "' +
            namespace + '_' + cls + '" ), 0 ); }\n'
            '#endif\n',
            'module': 'aims',
            'testPyType': 'pyaims' + cls + '_Check',
            'compareElement': '&',
            }


def classInCartoNamespace(include, cls, cppclass=None):
    return classInNamespace(include, cls, 'carto', cppclass=cppclass)


def classInAimsNamespace(include, cls, cppclass=None):
    return classInNamespace(include, cls, 'aims', cppclass=cppclass)


def classInSomaNamespace(include, cls, cppclass=None):
    return classInNamespace(include, cls, 'soma', cppclass=cppclass)


def classOutsideNamespace(include, cls, cppclass=None, typecode=None):
    if cppclass is None:
        cppclass = cls
    if typecode is None:
        typecode = cls
    return {'typecode': typecode,
            'pyFromC': 'pyaimsConvertFrom_' + cls,
            'CFromPy': 'pyaimsConvertTo_' + cls,
            'castFromSip': '(' + cppclass + ' *)',
            'deref': '*',
            'pyderef': '*',
            'address': '&',
            'pyaddress': '&',
            'defScalar': '',
            'defNumpyBindings': '',
            'new': 'new ' + cppclass,
            'NumType': 'NPY_OBJECT',
            'PyType': cppclass,
            'sipClass': cls,
            'typeinclude':
            '#include <' + include + '>',
            'sipinclude': '#ifndef PYAIMS_' + cls.upper() + '_CHECK_DEFINED\n'
            '#define PYAIMS_' + cls.upper() + '_CHECK_DEFINED\n'
            'inline int pyaims' + cls + '_Check( PyObject* o )\n'
            '{ return sipCanConvertToType( o, sipFindType( "' +
            cls +
            '" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
            '#endif\n'
            '#ifndef PYAIMS_' + cls.upper() + '_CONVERT_DEFINED\n'
            '#define PYAIMS_' + cls.upper() + '_CONVERT_DEFINED\n'
            'inline void* pyaimsConvertTo_' + cls + '( PyObject* o )\n'
            '{ int iserr = 0;\n'
            '  void *ptr = sipForceConvertToType( o, sipFindType( "' +
            cls + '" ), 0, 0, 0, &iserr );\n'
            '  if( iserr ) return 0;\n'
            '  return ptr;\n}\n'
            'inline PyObject* pyaimsConvertFrom_' + cls + '( void * a )\n'
            '{ return sipConvertFromType( a, sipFindType( "' +
            cls + '" ), 0 ); }\n'
            '#endif\n',
            'module': 'aims',
            'testPyType': 'pyaims' + cls + '_Check',
            'compareElement': '&',
            }

def rc_ptr_class(cls, typesub):
    cldef = typesub[cls]
    if cls.endswith('>'):
        spc = ' '
    else:
        spc = ''
    key = 'carto::rc_ptr<%s%s>' % (cls, spc)
    tcode = cldef['typecode']
    value = {
        'typecode': 'rc_ptr_%s' % tcode,
        'pyFromC': 'pyaimsConvertFrom_rc_ptr_%s' % tcode,
        'CFromPy': 'pyaimsConvertTo_rc_ptr_%s' % tcode,
        'castFromSip': '',
        'deref': '*',
        'pyderef': '*',
        'address': '&',
        'pyaddress': '&',
        'defScalar': '',
        'defNumpyBindings': '',
        'new': 'new carto::rc_ptr<%s > >' % cls,
        'NumType': 'PyArray_OBJECT',
        'PyType': 'rc_ptr_%s' % tcode,
        'sipClass': 'rc_ptr_%s' % tcode,
        'typeinclude': '%s\n'
        '#include <cartobase/smart/rcptr.h>' % cldef['typeinclude'],
        'sipinclude': '%s\n'
        '#ifndef PYAIMS_RC_PTR_%s_CHECK_DEFINED\n'
        '#define PYAIMS_RC_PTR_%s_CHECK_DEFINED\n'
        'inline int pyaimsRcptr_%s_Check( PyObject* o )\n'
        '{ return sipCanConvertToType( o, sipFindType( "rc_ptr_%s" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
        '#endif\n'
        '#ifndef PYAIMS_RC_PTR_%s_CONVERT_DEFINED\n'
        '#define PYAIMS_RC_PTR_%s_CONVERT_DEFINED\n'
        'inline carto::rc_ptr<%s >* pyaimsConvertTo_rc_ptr_%s( PyObject* o )\n'
        '{ int iserr = 0;\n'
        '  void *ptr = sipForceConvertToType( o, sipFindType( "rc_ptr_%s" ), 0, 0, 0, &iserr );\n'
        '  if( iserr ) return 0;\n'
        '  return reinterpret_cast<carto::rc_ptr<%s > *>( ptr );\n}\n'
        'inline PyObject* pyaimsConvertFrom_rc_ptr_%s( void * a )\n'
        '{ return sipConvertFromType( a, sipFindType( "rc_ptr_%s" ), 0 ); }\n'
        '#endif\n'
        % (cldef['sipinclude'], tcode.upper(), tcode.upper(), tcode, tcode,
           tcode.upper(), tcode.upper(), cls, tcode, tcode, cls, tcode, tcode),
        'module': 'aims',
        'testPyType': 'pyaimsRcptr_%s_Check' % tcode,
        'defIsRcptr': '#define IS_RCPTR 1',
    }
    return (key, value)


def completeTypesSub(typessub):
    for x, y in typessub.items():
        if 'compareElement' not in y:
            if(x.endswith('*')):
                y['compareElement'] = ''
            else:
                y['compareElement'] = '&'
        if 'NumDims' not in y:
            y['NumDims'] = 'std::vector<int>()'
        if 'NumType_Descr' not in y:
            y['NumType_Descr'] = '0'
        for key in ('defNumpyIsSubArray', 'defIsRcptr'):
            if key not in y:
                y[key] = ''


if 'options' in globals() and hasattr(options, 'target_platform'):
    target_platform = options.target_platform
else:
    target_platform = '-'.join([platform.system().lower(), 
                                platform.architecture()[0][:2]])
    
typessub = {'bool':
           {'typecode': 'BOOL',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_BOOL',
               'PyType': 'bool',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
           'signed char':
           {'typecode': 'S8',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_BYTE',
               'PyType': 'short',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'unsigned char':
           {'typecode': 'U8',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_UBYTE',
               'PyType': 'unsigned short',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'short':
           {'typecode': 'S16',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_SHORT',
               'PyType': 'short',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'unsigned short':
           {'typecode': 'U16',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_USHORT',
               'PyType': 'uint16_t',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'int':
           {'typecode': 'S32',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::PyaimsInt_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_INT32',
               'PyType': 'int',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'unsigned':
           {'typecode': 'U32',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::uint32_FromPy',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_UINT32',
               'PyType': 'unsigned',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'long':
            {'typecode': 'S64',
               'pyFromC': 'PyLong_FromLong',
               'CFromPy': 'PyLong_AsLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_INT64',
               'PyType': 'long',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'unsigned long':
            {'typecode': 'ULONG',
               'pyFromC': 'PyLong_FromLong',
               'CFromPy': 'PyLong_AsUnsignedLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_UINT',
               'PyType': 'unsigned long',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'unsigned long long':
           {'typecode': 'ULONGLONG',
               'pyFromC': 'PyLong_FromLongLong',
               'CFromPy': 'PyLong_AsUnsignedLongLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_UINT64',
               'PyType': 'unsigned long long',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
            },
            'float':
           {'typecode': 'FLOAT',
               'pyFromC': 'PyFloat_FromDouble',
               'CFromPy': 'carto::PyaimsFloat_AsDouble',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_FLOAT32',
               'PyType': 'float',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsNumber_Check',
               'compareElement': '',
            },
            'double':
           {'typecode': 'DOUBLE',
               'pyFromC': 'PyFloat_FromDouble',
               'CFromPy': 'carto::PyaimsFloat_AsDouble',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'NPY_FLOAT64',
               'PyType': 'double',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsNumber_Check',
               'compareElement': '',
            },
            'std::string':
           {'typecode': 'STRING',
               'pyFromC': 'carto::PyString_FromStdString',
               'CFromPy': 'carto::PyString_ToStdString',
               'castFromSip': '(std::string *)',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new std::string',
               'NumType': 'NPY_STRING',
               'PyType': 'std::string',
               'sipClass': '',
               'typeinclude': '#include <string>',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyStdString_Check',
               'compareElement': '',
            },
            'Void':
           {'typecode': 'VOID',
               'pyFromC': 'PyNone_FromVoid',
               'CFromPy': 'PyVoid',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Void',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Void',
               'sipClass': 'Void',
               'typeinclude': '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_VOID_DEFINED\n'
               '#define PYAIMS_VOID_DEFINED\n'
               'inline int pyaimsVoid_Check( PyObject *o )\n'
               '{ return o == Py_None; }\n'
               'inline Void* PyVoid( PyObject* )\n'
               '{ return new Void; }\n'
               'inline PyObject* PyNone_FromVoid( const Void* )\n'
               '{ return Py_None; }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVoid_Check',
               'compareElement': '',
            },

            'cfloat':
           {'typecode': 'CFLOAT',
               'pyFromC': 'PyComplex_FromDoubles',
               'CFromPy': 'PyComplex_AsDouble',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'new': 'new cfloat',
               'NumType': 'NPY_COMPLEX64',
               'PyType': 'cfloat',
               'sipClass': '',
               'typeinclude': '#include <cartobase/type/types.h>',
               'sipinclude': '',
               'module': 'aims',
               'testPyType': 'PyComplex_Check',
               'compareElement': '',
            },
            'cdouble':
           {'typecode': 'CDOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'new': 'new cdouble',
               'NumType': 'NPY_COMPLEX128',
               'PyType': 'cdouble',
               'sipClass': '',
               'typeinclude': '#include <cartobase/type/types.h>',
               'sipinclude': '',
               'module': 'aims',
               'testPyType': 'PyComplex_Check',
               'compareElement': '',
            },

            'AimsRGB':
           {'typecode': 'RGB',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'new': 'new AimsRGB',
               'NumType': 'NPY_AimsRGB()',
               'NumType_Descr': 'NPY_AimsRGB_Descr()',
               'PyType': 'AimsRGB',
               'sipClass': 'AimsRGB',
               'typeinclude': '#include <aims/rgb/rgb.h>',
               'sipinclude': '#ifndef PYAIMS_RGB_DEFINED\n'
               '#define PYAIMS_RGB_DEFINED\n'
               'inline int pyaimsRGB_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsRGB" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsRGB_Check',
               'compareElement': '',
            },
            'AimsRGBA':
           {'typecode': 'RGBA',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'new': 'new AimsRGBA',
               'NumType': 'NPY_AimsRGBA()',
               'NumType_Descr': 'NPY_AimsRGBA_Descr()',
               'PyType': 'AimsRGBA',
               'sipClass': 'AimsRGBA',
               'typeinclude': '#include <aims/rgb/rgb.h>',
               'sipinclude': '#ifndef PYAIMS_RGBA_DEFINED\n'
               '#define PYAIMS_RGBA_DEFINED\n'
               'inline int pyaimsRGBA_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsRGBA" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsRGBA_Check',
               'compareElement': '',
            },

            'AimsHSV':
           {'typecode': 'HSV',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'new': 'new AimsHSV',
               'NumType': 'NPY_AimsHSV()',
               'NumType_Descr': 'NPY_AimsHSV_Descr()',
               'PyType': 'AimsHSV',
               'sipClass': 'AimsHSV',
               'typeinclude': '#include <aims/hsv/hsv.h>',
               'sipinclude': '#ifndef PYAIMS_HSV_DEFINED\n'
               '#define PYAIMS_HSV_DEFINED\n'
               'inline int pyaimsHSV_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsHSV" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsHSV_Check',
               'compareElement': '',
            },

            'Point2df':
           {'typecode': 'POINT2DF',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<float,2>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'new': 'new Point2df',
               'NumType': 'NPY_AimsVector<float, 2>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<float, 2>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'Point2df',
               'sipClass': 'AimsVector_FLOAT_2',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_FLOAT_2\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_FLOAT_2\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<float,2>()\n'
               '{ return sipFindType( "AimsVector_FLOAT_2" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<float,2>',
               'compareElement': '',
            },

            'Point3df':
           {'typecode': 'POINT3DF',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<float,3>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'new': 'new Point3df',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<float, 3>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<float, 3>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'Point3df',
               'sipClass': 'AimsVector_FLOAT_3',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_FLOAT_3.h"\n'
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_FLOAT_3\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_FLOAT_3\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<float,3>()\n'
               '{ return sipFindType( "AimsVector_FLOAT_3" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<float,3>',
               'compareElement': '',
            },

            'Point3dd':
           {'typecode': 'POINT3DD',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<double,3>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'new': 'new Point3dd',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<double, 3>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<double, 3>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'Point3dd',
               'sipClass': 'AimsVector_DOUBLE_3',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_DOUBLE_3.h"\n'
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_DOUBLE_3\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_DOUBLE_3\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<double,3>()\n'
               '{ return sipFindType( "AimsVector_DOUBLE_3" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<double,3>',
               'compareElement': '',
            },

            'Point3d':
           {'typecode': 'POINT3D',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<int16_t,3>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'new': 'new Point3d',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<int16_t, 3>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<int16_t, 3>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'Point3d',
               'sipClass': 'AimsVector_S16_3',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_S16_3.h"\n'
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_S16_3\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_S16_3\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<int16_t,3>()\n'
               '{ return sipFindType( "AimsVector_S16_3" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<int16_t,3>',
               'compareElement': '',
            },

             'Point3dl' : \
             { 'typecode' : 'POINT3DL',
               'pyFromC' : 'pyaimsConvertFrom_AimsVector',
               'CFromPy' : 'pyaimsConvertTo_AimsVector<int32_t,3>',
               'castFromSip' : '',
               'deref' : '*',
               'pyderef' : '*',
               'address' : '&', 
               'pyaddress' : '&', 
               'defScalar' : '',
               'new' : 'new Point3dl', 
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<int32_t, 3>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<int32_t, 3>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType' : 'Point3dl',
               'sipClass' : 'AimsVector_S32_3',
               'typeinclude' : '#include <pyaims/vector/vector.h>', 
               'sipinclude' : '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_S32_3.h"\n' 
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_S32_3\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_S32_3\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<int32_t,3>()\n'
               '{ return sipFindType( "AimsVector_S32_3" ); }\n'
               '#endif',
               'module' : 'aims', 
               'testPyType' : 'pyaimsCheck_AimsVector<int32_t,3>', 
               'compareElement' : '',
               },

             'Point4dl' : \
             { 'typecode' : 'POINT4DL',
               'pyFromC' : 'pyaimsConvertFrom_AimsVector',
               'CFromPy' : 'pyaimsConvertTo_AimsVector<int64_t,4>',
               'castFromSip' : '',
               'deref' : '*',
               'pyderef' : '*',
               'address' : '&', 
               'pyaddress' : '&', 
               'defScalar' : '',
               'new' : 'new Point4dl', 
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<int64_t, 4>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<int64_t, 4>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType' : 'Point4dl',
               'sipClass' : 'AimsVector_S64_4',
               'typeinclude' : '#include <pyaims/vector/vector.h>', 
               'sipinclude' : '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_S64_4.h"\n' 
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_S64_4\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_S64_4\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<int64_t,4>()\n'
               '{ return sipFindType( "AimsVector_S64_4" ); }\n'
               '#endif',
               'module' : 'aims', 
               'testPyType' : 'pyaimsCheck_AimsVector<int64_t,4>', 
               'compareElement' : '',
               },
             'AimsVector<unsigned,2>' : \
             { 'typecode' : 'AimsVector_U32_2',
               'pyFromC' : 'pyaimsConvertFrom_AimsVector',
               'CFromPy' : 'pyaimsConvertTo_AimsVector<uint32_t,2>',
               'castFromSip' : '',
               'deref' : '*',
               'pyderef' : '*',
               'address' : '&', 
               'pyaddress' : '&', 
               'defScalar' : '',
               'new' : 'new AimsVector_U32_2', 
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<uint32_t, 2>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<uint32_t, 2>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType' : 'AimsVector_U32_2',
               'sipClass' : 'AimsVector_U32_2',
               'typeinclude' : '#include <pyaims/vector/vector.h>', 
               'sipinclude' : '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_U32_2.h"\n' 
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_U32_2\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_U32_2\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<uint32_t,2>()\n'
               '{ return sipFindType( "AimsVector_U32_2" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<uint32_t,2>',
               'compareElement': '',
            },

            'AimsVector<unsigned,3>':
           {'typecode': 'AimsVector_U32_3',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<uint32_t,3>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'new': 'new AimsVector_U32_3',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<uint32_t, 3>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<uint32_t, 3>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'AimsVector_U32_3',
               'sipClass': 'AimsVector_U32_3',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_U32_3.h"\n'
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_U32_3\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_U32_3\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<uint32_t,3>()\n'
               '{ return sipFindType( "AimsVector_U32_3" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<uint32_t,3>',
               'compareElement': '',
            },

            'AimsVector<unsigned,4>':
           {'typecode': 'AimsVector_U32_4',
               'pyFromC': 'pyaimsConvertFrom_AimsVector',
               'CFromPy': 'pyaimsConvertTo_AimsVector<uint32_t,4>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'new': 'new AimsVector_U32_4',
               'defNumpyBindings': '#define PYAIMS_NUMPY_BINDINGS',
               'NumType': 'NPY_AimsVector<uint32_t, 4>()',
               'NumType_Descr': 'NPY_AimsVector_Descr<uint32_t, 4>()',
               'defNumpyIsSubArray': '#define PYAIMS_NPY_IS_SUBARRAY',
               'PyType': 'AimsVector_U32_4',
               'sipClass': 'AimsVector_U32_4',
               'typeinclude': '#include <pyaims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsVector_U32_4.h"\n'
               '#endif\n'
               '#include <pyaims/vector/sipvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_AIMSVECTOR_U32_4\n'
               '#define PYAIMS_WRAPPER_AIMSVECTOR_U32_4\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_AimsVector<uint32_t,4>()\n'
               '{ return sipFindType( "AimsVector_U32_4" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_AimsVector<uint32_t,4>',
               'compareElement': '',
            },

            'carto::Semantic':
            classInCartoNamespace('cartobase/object/syntax.h',
                                  'Semantic'),

            'Tree':
           {'typecode': 'Tree',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Tree',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Tree',
               'sipClass': 'Tree',
               'typeinclude': '#include <graph/tree/tree.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTree.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TREE_DEFINED\n'
               '#define PYAIMS_TREE_DEFINED\n'
               'inline int pyaimsTree_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Tree" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTree_Check',
            },
            'aims::Hierarchy':
           {'typecode': 'Hierarchy',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::Hierarchy',
               'NumType': 'NPY_OBJECT',
               'PyType': 'aims::Hierarchy',
               'sipClass': 'aims_Hierarchy',
               'typeinclude': '#include <aims/roi/hie.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipaimsHierarchy.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_HIERARCHY_DEFINED\n'
               '#define PYAIMS_HIERARCHY_DEFINED\n'
               'inline int pyaimsHierarchy_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "aims_Hierarchy" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsHierarchy_Check',
            },
            'Graph':
            {'typecode': 'Graph',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Graph',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Graph',
               'sipClass': 'Graph',
               'typeinclude': '#include <graph/graph/graph.h>',
               'sipinclude': '#ifndef PYAIMS_GRAPH_DEFINED\n'
               '#define PYAIMS_GRAPH_DEFINED\n'
               'inline int pyaimsGraph_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Graph" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsGraph_Check',
            },
            'Vertex':
            {'typecode': 'Vertex',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Vertex',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Vertex',
               'sipClass': 'Vertex',
               'typeinclude': '#include <graph/graph/vertex.h>',
               'sipinclude': '#ifndef PYAIMS_VERTEX_DEFINED\n'
               '#define PYAIMS_VERTEX_DEFINED\n'
               'inline int pyaimsVertex_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Vertex" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVertex_Check',
            },
            'Edge':
            {'typecode': 'Edge',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Edge',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Edge',
               'sipClass': 'Edge',
               'typeinclude': '#include <graph/graph/edge.h>',
               'sipinclude': '#ifndef PYAIMS_EDGE_DEFINED\n'
               '#define PYAIMS_EDGE_DEFINED\n'
               'inline int pyaimsEdge_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Edge" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsEdge_Check',
            },
            'carto::Syntax':
            classInCartoNamespace('cartobase/object/syntax.h',
                                  'Syntax'),
            'AimsSurfaceTriangle':
           {'typecode': 'AimsSurfaceTriangle',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsSurfaceTriangle',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsSurfaceTriangle',
               'sipClass': 'AimsSurfaceTriangle',
               'typeinclude': '#include <aims/mesh/surface.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipAimsSurfaceTriangle.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_SURFACETRIANGLE_DEFINED\n'
               '#define PYAIMS_SURFACETRIANGLE_DEFINED\n'
               'inline int pyaimsSurfaceTriangle_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsSurfaceTriangle" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsSurfaceTriangle_Check',
            },
            'carto::GenericObject':
           {'typecode': 'GenericObject',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::GenericObject',
               'NumType': 'NPY_OBJECT',
               'PyType': 'carto::GenericObject',
               'sipClass': 'carto_GenericObject',
               'typeinclude': '#include <cartobase/object/object.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipcartoGenericObject.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_GENERICOBJECT_DEFINED\n'
               '#define PYAIMS_GENERICOBJECT_DEFINED\n'
               'inline int pyaimsGenericObject_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "carto_GenericObject" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsGenericObject_Check',
            },

            'carto::Volume<bool>':
            {'typecode': 'Volume_BOOL',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<bool>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_BOOL',
               'sipClass': 'Volume_BOOL',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_BOOL.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_BOOL_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_BOOL_CHECK_DEFINED\n'
               'inline int pyaimsVolume_BOOL_Check( PyObject* o )\n'
               '{ return sipCanConvertToInstance( o, sipClass_Volume_BOOL, SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_BOOL_Check',
            },
            'carto::Volume<int8_t>':
            {'typecode': 'Volume_S8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<int8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_S8',
               'sipClass': 'Volume_S8',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#ifndef PYAIMS_VOLUME_S8_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_S8_CHECK_DEFINED\n'
               'inline int pyaimsVolume_S8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_S8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_S8_Check',
            },
            'carto::Volume<uint8_t>':
           {'typecode': 'Volume_U8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<uint8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_U8',
               'sipClass': 'Volume_U8',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_U8.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_U8_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_U8_CHECK_DEFINED\n'
               'inline int pyaimsVolume_U8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_U8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_U8_Check',
            },
            'carto::Volume<int16_t>':
           {'typecode': 'Volume_S16',
               'pyFromC': 'sipConvertFrom_Volume_S16',
               'CFromPy': 'pyaimsConvertTo_Volume_S16',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<int16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_S16',
               'sipClass': 'Volume_S16',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_S16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_S16_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_S16_CHECK_DEFINED\n'
               'inline int pyaimsVolume_S16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_S16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_S16_Check',
            },
            'carto::Volume<uint16_t>':
           {'typecode': 'Volume_U16',
               'pyFromC': 'sipConvertFrom_Volume_U16',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<uint16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_U16',
               'sipClass': 'Volume_U16',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_U16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_U16_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_U16_CHECK_DEFINED\n'
               'inline int pyaimsVolume_U16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_U16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_U16_Check',
            },
            'carto::Volume<int32_t>':
           {'typecode': 'Volume_S32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<int32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_S32',
               'sipClass': 'Volume_S32',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_S32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_S32_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_S32_CHECK_DEFINED\n'
               'inline int pyaimsVolume_S32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_S32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_S32_Check',
            },
            'carto::Volume<uint32_t>':
           {'typecode': 'Volume_U32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<uint32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_U32',
               'sipClass': 'Volume_U32',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_U32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_U32_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_U32_CHECK_DEFINED\n'
               'inline int pyaimsVolume_U32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_U32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_U32_Check',
            },
            'carto::Volume<float>':
           {'typecode': 'Volume_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_FLOAT',
               'sipClass': 'Volume_FLOAT',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_FLOAT.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsVolume_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_FLOAT_Check',
            },
            'carto::Volume<double>':
           {'typecode': 'Volume_DOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<double>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_DOUBLE',
               'sipClass': 'Volume_DOUBLE',
               'typeinclude': '#include <cartodata/volume/volume.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_DOUBLE.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_DOUBLE_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_DOUBLE_CHECK_DEFINED\n'
               'inline int pyaimsVolume_DOUBLE_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_DOUBLE" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_DOUBLE_Check',
            },
            'carto::Volume<AimsRGB>':
           {'typecode': 'Volume_RGB',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<AimsRGB>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_RGB',
               'sipClass': 'Volume_RGB',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <aims/rgb/rgb.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_RGB.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_RGB_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_RGB_CHECK_DEFINED\n'
               'inline int pyaimsVolume_RGB_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_RGB" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_RGB_Check',
            },
            'carto::Volume<AimsRGBA>':
           {'typecode': 'Volume_RGBA',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<AimsRGBA>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_RGBA',
               'sipClass': 'Volume_RGBA',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <aims/rgb/rgb.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipVolume_RGBA.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_VOLUME_RGBA_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_RGBA_CHECK_DEFINED\n'
               'inline int pyaimsVolume_RGBA_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_RGBA" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_RGBA_Check',
            },
            'carto::Volume<AimsHSV>':
           {'typecode': 'Volume_HSV',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<AimsHSV>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_HSV',
               'sipClass': 'Volume_HSV',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <aims/hsv/hsv.h>',
               'sipinclude': '#ifndef PYAIMS_VOLUME_HSV_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_HSV_CHECK_DEFINED\n'
               'inline int pyaimsVolume_HSV_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_HSV" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_HSV_Check',
            },
            'carto::Volume<cfloat>':
           {'typecode': 'Volume_CFLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<cfloat>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_CFLOAT',
               'sipClass': 'Volume_CFLOAT',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_VOLUME_CFLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_CFLOAT_CHECK_DEFINED\n'
               'inline int pyaimsVolume_CFLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_CFLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_CFLOAT_Check',
            },
            'carto::Volume<cdouble>':
           {'typecode': 'Volume_CDOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<cdouble>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_CDOUBLE',
               'sipClass': 'Volume_CDOUBLE',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_VOLUME_CDOUBLE_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_CDOUBLE_CHECK_DEFINED\n'
               'inline int pyaimsVolume_CDOUBLE_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_CDOUBLE" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_CDOUBLE_Check',
            },
            'carto::Volume<Point3df>':
           {'typecode': 'Volume_POINT3DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::Volume<Point3df>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Volume_POINT3DF',
               'sipClass': 'Volume_POINT3DF',
               'typeinclude':
               '#include <cartodata/volume/volume.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_VOLUME_POINT3DF_CHECK_DEFINED\n'
               '#define PYAIMS_VOLUME_POINT3DF_CHECK_DEFINED\n'
               'inline int pyaimsVolume_POINT3DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Volume_POINT3DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsVolume_POINT3DF_Check',
            },

            '1':
           {'typecode': '1',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': '',
               'NumType': '',
               'PyType': '',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '',
            },
            '2':
           {'typecode': '2',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': '',
               'NumType': '',
               'PyType': '',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '',
            },
            '3':
           {'typecode': '3',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': '',
               'NumType': '',
               'PyType': '',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '',
            },
            '4':
           {'typecode': '4',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': '',
               'NumType': '',
               'PyType': '',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '',
            },

            'AimsTimeSurface<2,Void>':
           {'typecode': 'AimsTimeSurface_2_VOID',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<2,Void>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_2_VOID',
               'sipClass': 'AimsTimeSurface_2_VOID',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_2_VOID_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_2_VOID_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_2_VOID_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_2_VOID" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_2_VOID_Check',
            },
            'AimsTimeSurface<3,Void>':
           {'typecode': 'AimsTimeSurface_3_VOID',
               'pyFromC': 'pyaimsConvertFrom_TIMESURFACE_3_VOID',
               'CFromPy': 'pyaimsConvertTo_TIMESURFACE_3_VOID',
               'castFromSip': '(AimsTimeSurface<3, Void> *)',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<3,Void>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_3_VOID',
               'sipClass': 'AimsTimeSurface_3_VOID',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_3_VOID_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_3_VOID_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_3_VOID_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_3_VOID" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMESURFACE_3_VOID_CONVERT_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_3_VOID_CONVERT_DEFINED\n'
               'inline void* pyaimsConvertTo_TIMESURFACE_3_VOID( PyObject* o )\n'
               '{ int iserr = 0;\n'
               '  void *ptr = sipForceConvertToType( o, sipFindType( "TIMESURFACE_3_VOID" ), 0, 0, 0, &iserr );\n'
               '  if( iserr ) return 0;\n'
               '  return ptr;\n}\n'
               'inline PyObject* pyaimsConvertFrom_TIMESURFACE_3_VOID( void * a )\n'
               '{ return sipConvertFromType( a, sipFindType( "AimsTimeSurface_3_VOID" ), 0 ); }\n'
               '#endif\n',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_3_VOID_Check',
            },
            'AimsTimeSurface<4,Void>':
           {'typecode': 'AimsTimeSurface_4_VOID',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<4,Void>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_4_VOID',
               'sipClass': 'AimsTimeSurface_4_VOID',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_4_VOID_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_4_VOID_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_4_VOID_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_4_VOID" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_4_VOID_Check',
            },

            'AimsTimeSurface<2,float>':
           {'typecode': 'AimsTimeSurface_2_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<2,float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_2_FLOAT',
               'sipClass': 'AimsTimeSurface_2_FLOAT',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_2_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_2_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_2_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_2_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_2_FLOAT_Check',
            },
            'AimsTimeSurface<3,float>':
           {'typecode': 'AimsTimeSurface_3_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<3,float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_3_FLOAT',
               'sipClass': 'AimsTimeSurface_3_FLOAT',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_3_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_3_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_3_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_3_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_3_FLOAT_Check',
            },
            'AimsTimeSurface<4,float>':
           {'typecode': 'AimsTimeSurface_4_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<4,float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_4_FLOAT',
               'sipClass': 'AimsTimeSurface_4_FLOAT',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_4_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_4_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_4_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_4_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_4_FLOAT_Check',
            },

            'AimsTimeSurface<2,Point2df>':
           {'typecode': 'AimsTimeSurface_2_POINT2DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<2,Point2df>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_2_POINT2DF',
               'sipClass': 'AimsTimeSurface_2_POINT2DF',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_2_POINT2DF_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_2_POINT2DF_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_2_POINT2DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_2_POINT2DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_2_POINT2DF_Check',
            },
            'AimsTimeSurface<3,Point2df>':
           {'typecode': 'AimsTimeSurface_3_POINT2DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<3,Point2DF>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_3_POINT2DF',
               'sipClass': 'AimsTimeSurface_3_POINT2DF',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_3_POINT2DF_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_3_POINT2DF_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_3_POINT2DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_3_POINT2DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_3_POINT2DF_Check',
            },
            'AimsTimeSurface<4,Point2df>':
           {'typecode': 'AimsTimeSurface_4_POINT2DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new AimsTimeSurface<4,Point2df>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'AimsTimeSurface_4_POINT2DF',
               'sipClass': 'AimsTimeSurface_4_POINT2DF',
               'typeinclude':
               '#include <aims/mesh/surface.h>',
               'sipinclude': '#ifndef PYAIMS_TIMESURFACE_4_POINT2DF_CHECK_DEFINED\n'
               '#define PYAIMS_TIMESURFACE_4_POINT2DF_CHECK_DEFINED\n'
               'inline int pyaimsTimeSurface_4_POINT2DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "AimsTimeSurface_4_POINT2DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeSurface_4_POINT2DF_Check',
            },

            'Texture<float>':
            {'typecode': 'Texture_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_FLOAT',
               'sipClass': 'Texture_FLOAT',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_FLOAT.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsTexture_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_FLOAT_Check',
            },
            'Texture<double>':
            {'typecode': 'Texture_DOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<double>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_DOUBLE',
               'sipClass': 'Texture_DOUBLE',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#ifndef PYAIMS_TEXTURE_DOUBLE_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_DOUBLE_CHECK_DEFINED\n'
               'inline int pyaimsTexture_DOUBLE_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_DOUBLE" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_DOUBLE_Check',
            },
            'Texture<int8_t>':
            {'typecode': 'Texture_S8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<int8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_S8',
               'sipClass': 'Texture_S8',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_S8.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_S8_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_S8_CHECK_DEFINED\n'
               'inline int pyaimsTexture_S8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_S8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_S8_Check',
            },
            'Texture<uint8_t>':
            {'typecode': 'Texture_U8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<uint8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_U8',
               'sipClass': 'Texture_U8',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_U8.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_U8_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_U8_CHECK_DEFINED\n'
               'inline int pyaimsTexture_U8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_U8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_U8_Check',
            },
            'Texture<int16_t>':
            {'typecode': 'Texture_S16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<int16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_S16',
               'sipClass': 'Texture_S16',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_S16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_S16_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_S16_CHECK_DEFINED\n'
               'inline int pyaimsTexture_S16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_S16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_S16_Check',
            },
            'Texture<uint16_t>':
            {'typecode': 'Texture_U16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<uint16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_U16',
               'sipClass': 'Texture_U16',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_U16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_U16_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_U16_CHECK_DEFINED\n'
               'inline int pyaimsTexture_U16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_U16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_U16_Check',
            },
            'Texture<int32_t>':
            {'typecode': 'Texture_S32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<int32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_S32',
               'sipClass': 'Texture_S32',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_S32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_S32_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_S32_CHECK_DEFINED\n'
               'inline int pyaimsTexture_S32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_S32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_FLOAT_Check',
            },
            'Texture<uint32_t>':
            {'typecode': 'Texture_U32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<uint32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_U32',
               'sipClass': 'Texture_U32',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_U32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_U32_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_U32_CHECK_DEFINED\n'
               'inline int pyaimsTexture_U32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_U32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_FLOAT_Check',
            },
            'Texture<int64_t>':
            {'typecode': 'Texture_S64',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<int64_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_S64',
               'sipClass': 'Texture_S64',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_S64.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_S64_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_S64_CHECK_DEFINED\n'
               'inline int pyaimsTexture_S64_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_S64" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_S64_Check',
            },
            'Texture<uint64_t>':
            {'typecode': 'Texture_U64',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<uint64_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_U64',
               'sipClass': 'Texture_U64',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_U64.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_U64_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_U64_CHECK_DEFINED\n'
               'inline int pyaimsTexture_U64_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_U64" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_U64_Check',
            },
            'Texture<Point2df>':
            {'typecode': 'Texture_POINT2DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Texture<Point2df>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Texture_POINT2DF',
               'sipClass': 'Texture_POINT2DF',
               'typeinclude':
               '#include <aims/mesh/texture.h>\n'
               '#include <aims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTexture_POINT2DF.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TEXTURE_POINT2DF_CHECK_DEFINED\n'
               '#define PYAIMS_TEXTURE_POINT2DF_CHECK_DEFINED\n'
               'inline int pyaimsTexture_POINT2DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "Texture_POINT2DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTexture_POINT2DF_Check',
            },

            'TimeTexture<float>':
            {'typecode': 'TimeTexture_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_FLOAT',
               'sipClass': 'TimeTexture_FLOAT',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_FLOAT.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_FLOAT_Check',
            },
            'TimeTexture<double>':
            {'typecode': 'TimeTexture_DOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<double>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_DOUBLE',
               'sipClass': 'TimeTexture_DOUBLE',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#ifndef PYAIMS_TIMETEXTURE_DOUBLE_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_DOUBLE_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_DOUBLE_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_DOUBLE" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_DOUBLE_Check',
            },
            'TimeTexture<int8_t>':
            {'typecode': 'TimeTexture_S8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<int8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_S8',
               'sipClass': 'TimeTexture_S8',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_S8.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_S8_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_S8_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_S8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_S8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_S8_Check',
            },
            'TimeTexture<uint8_t>':
            {'typecode': 'TimeTexture_U8',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<uint8_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_U8',
               'sipClass': 'TimeTexture_U8',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_U8.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_U8_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_U8_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_U8_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_U8" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_U8_Check',
            },
            'TimeTexture<int16_t>':
            {'typecode': 'TimeTexture_S16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<int16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_S16',
               'sipClass': 'TimeTexture_S16',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_S16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_S16_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_S16_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_S16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_S16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_S16_Check',
            },
            'TimeTexture<uint16_t>':
            {'typecode': 'TimeTexture_U16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<uint16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_U16',
               'sipClass': 'TimeTexture_U16',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_U16.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_U16_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_U16_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_U16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_U16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_U16_Check',
            },
            'TimeTexture<int32_t>':
            {'typecode': 'TimeTexture_S32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<int32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_S32',
               'sipClass': 'TimeTexture_S32',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_S32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_S32_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_S32_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_S32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_S32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_S32_Check',
            },
            'TimeTexture<uint32_t>':
            {'typecode': 'TimeTexture_U32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<uint32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_U32',
               'sipClass': 'TimeTexture_U32',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_U32.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_U32_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_U32_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_U32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_U32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_U32_Check',
            },
            'TimeTexture<int64_t>':
            {'typecode': 'TimeTexture_S64',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<int64_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_S64',
               'sipClass': 'TimeTexture_S64',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_S64.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_S64_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_S64_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_S64_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_S64" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_S64_Check',
            },
            'TimeTexture<uint64_t>':
            {'typecode': 'TimeTexture_U64',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<uint64_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_U64',
               'sipClass': 'TimeTexture_U64',
               'typeinclude':
               '#include <aims/mesh/texture.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_U64.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_U64_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_U64_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_U64_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_U64" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_U64_Check',
            },
            'TimeTexture<Point2df>':
            {'typecode': 'TimeTexture_POINT2DF',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new TimeTexture<Point2df>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'TimeTexture_POINT2DF',
               'sipClass': 'TimeTexture_POINT2DF',
               'typeinclude':
               '#include <aims/mesh/texture.h>\n'
               '#include <aims/vector/vector.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipTimeTexture_POINT2DF.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_TIMETEXTURE_POINT2DF_CHECK_DEFINED\n'
               '#define PYAIMS_TIMETEXTURE_POINT2DF_CHECK_DEFINED\n'
               'inline int pyaimsTimeTexture_POINT2DF_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "TimeTexture_POINT2DF" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsTimeTexture_POINT2DF_Check',
            },

            'aims::BucketMap<Void>':
           {'typecode': 'BucketMap_VOID',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<Void>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_VOID',
               'sipClass': 'BucketMap_VOID',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipBucketMap_VOID.h"\n'
               '#endif\n'
               '#ifndef PYAIMS_BUCKETMAP_VOID_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_VOID_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_VOID_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_VOID" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_VOID_Check',
            },

            'aims::BucketMap<int16_t>':
           {'typecode': 'BucketMap_S16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<int16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_S16',
               'sipClass': 'BucketMap_S16',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_S16_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_S16_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_S16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_S16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_S16_Check',
            },

            'aims::BucketMap<uint16_t>':
           {'typecode': 'BucketMap_U16',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<uint16_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_U16',
               'sipClass': 'BucketMap_U16',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_U16_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_U16_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_U16_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_U16" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_U16_Check',
            },

            'aims::BucketMap<int32_t>':
           {'typecode': 'BucketMap_S32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<int32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_S32',
               'sipClass': 'BucketMap_S32',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_S32_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_S32_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_S32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_S32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_S32_Check',
            },

            'aims::BucketMap<uint32_t>':
           {'typecode': 'BucketMap_U32',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<uint32_t>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_U32',
               'sipClass': 'BucketMap_U32',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_U32_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_U32_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_U32_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_U32" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_U32_Check',
            },

            'aims::BucketMap<float>':
           {'typecode': 'BucketMap_FLOAT',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<float>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_FLOAT',
               'sipClass': 'BucketMap_FLOAT',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_FLOAT_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_FLOAT_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_FLOAT_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_FLOAT" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_FLOAT_Check',
            },

            'aims::BucketMap<double>':
           {'typecode': 'BucketMap_DOUBLE',
               'pyFromC': '',
               'CFromPy': '',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new aims::BucketMap<double>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'BucketMap_DOUBLE',
               'sipClass': 'BucketMap_DOUBLE',
               'typeinclude':
               '#include <aims/bucket/bucket.h>\n'
               '#include <cartobase/type/types.h>',
               'sipinclude': '#ifndef PYAIMS_BUCKETMAP_DOUBLE_CHECK_DEFINED\n'
               '#define PYAIMS_BUCKETMAP_DOUBLE_CHECK_DEFINED\n'
               'inline int pyaimsBucketMap_DOUBLE_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "BucketMap_DOUBLE" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsBucketMap_DOUBLE_Check',
            },

            'soma::Transformation3d':
            classInSomaNamespace(
                'soma-io/transformation/transformation.h', 'Transformation3d'),

            'aims::AffineTransformation3d':
            classInAimsNamespace(
                'aims/transformation/affinetransformation3d.h',
                'AffineTransformation3d'),

            'aims::TransformationChain3d':
            classInAimsNamespace(
                'aims/transformation/transformationchain3d.h',
                'TransformationChain3d'),

            'aims::TransformationGraph3d':
            classInAimsNamespace(
                'aims/transformation/transformationgraph3d.h',
                'TransformationGraph3d'),

            'carto::Site':
            classInCartoNamespace('cartodata/wip/roi/siteIterator.h',
                                  'Site'),
            'carto::SiteIterator':
            classInCartoNamespace('cartodata/wip/roi/siteIterator.h',
                                  'SiteIterator'),

            'aims::Interpolator':
            classInAimsNamespace('aims/resampling/linearInterpolator.h',
                                 'Interpolator'),

            'carto::Object':
            classInCartoNamespace('cartobase/object/object.h',
                                  'Object'),

            'Graph *':
           {'typecode': 'GraphPtr',
               'pyFromC': 'pyaimsConvertFrom_GraphP',
               'CFromPy': 'pyaimsConvertTo_GraphP',
               'castFromSip': '(Graph *)',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Graph *',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Graph *',
               'sipClass': 'Graph *',
               'typeinclude': '#include <graph/graph/graph.h>',
               'sipinclude': '#include <pyaims/graph.h>',
               'module': 'aims',
               'testPyType': 'pyaimsGraphP_Check',
               'compareElement': '',
            },

            'carto::rc_ptr<Graph>':
           {'typecode': 'rc_ptr_Graph',
               'pyFromC': 'pyaimsConvertFrom_GraphR',
               'CFromPy': 'pyaimsConvertTo_GraphR',
               'castFromSip': '(carto::rc_ptr<Graph> *)',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::rc_ptr<Graph>',
               'NumType': 'NPY_OBJECT',
               'PyType': 'rc_ptr_Graph',
               'sipClass': 'rc_ptr_Graph',
               'typeinclude': '#include <graph/graph/graph.h>',
               'sipinclude': '#include <pyaims/graph.h>',
               'module': 'aims',
               'testPyType': 'pyaimsGraphR_Check',
            },


            'Vertex *':
           {'typecode': 'VertexPtr',
               'pyFromC': 'pyaimsConvertFrom_VertexP',
               'CFromPy': 'pyaimsConvertTo_VertexP',
               'castFromSip': '(Vertex *)',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Vertex *',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Vertex *',
               'sipClass': 'Vertex *',
               'typeinclude': '#include <graph/graph/vertex.h>',
               'sipinclude': '#include <pyaims/graph.h>',
               'module': 'aims',
               'testPyType': 'pyaimsVertexP_Check',
               'compareElement': '',
            },

            'Edge *':
           {'typecode': 'EdgePtr',
               'pyFromC': 'pyaimsConvertFrom_EdgeP',
               'CFromPy': 'pyaimsConvertTo_EdgeP',
               'castFromSip': '(Edge *)',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new Edge *',
               'NumType': 'NPY_OBJECT',
               'PyType': 'Edge *',
               'sipClass': 'Edge *',
               'typeinclude': '#include <graph/graph/edge.h>',
               'sipinclude': '#include <pyaims/graph.h>',
               'module': 'aims',
               'testPyType': 'pyaimsEdgeP_Check',
               'compareElement': '',
            },

            'carto::Info':
            classInCartoNamespace('cartobase/config/info.h',
                                  'Info'),

            'carto::Info *':
           {'typecode': 'InfoPtr',
               'pyFromC': 'pyaimsConvertFrom_InfoP',
               'CFromPy': 'pyaimsConvertTo_InfoP',
               'castFromSip': '(carto::Info *)',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '',
               'defNumpyBindings': '',
               'defNoObject': '#define PYAIMS_NOOBJECT',
               'new': 'new carto::Info *',
               'NumType': 'NPY_OBJECT',
               'PyType': 'carto::Info *',
               'sipClass': 'carto::Info *',
               'typeinclude': '#include <cartobase/config/info.h>',
               'sipinclude': '#if SIP_VERSION < 0x040700\n'
               '#include "sipaimssipInfo.h"\n'
               '#endif\n'
               '#include <pyaims/info.h>',
               'module': 'aims',
               'testPyType': 'pyaimsInfoP_Check',
               'compareElement': '',
            },

            'std::set<unsigned>':
            {'typecode': 'set_U32',
               'pyFromC': 'pyaimsConvertFrom_set',
               'CFromPy': 'pyaimsConvertTo_set<unsigned>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new set_U32',
               'NumType': 'NPY_OBJECT',
               'PyType': 'set_U32',
               'sipClass': 'set_U32',
               'typeinclude': '#include <set>',
               'sipinclude': '#include <pyaims/vector/sipset.h>\n'
               '#ifndef PYAIMS_WRAPPER_SET_U32\n'
               '#define PYAIMS_WRAPPER_SET_U32\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_set<unsigned>()\n'
               '{ return sipFindType( "set_U32" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_set<unsigned>',
               'compareElement': '',
            },

            'std::vector<Point3df>':
            {'typecode': 'vector_POINT3DF',
               'pyFromC': 'pyaimsConvertFrom_vector',
               'CFromPy': 'pyaimsConvertTo_vector<Point3df>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new vector_POINT3DF',
               'NumType': 'NPY_OBJECT',
               'PyType': 'vector_POINT3DF',
               'sipClass': 'vector_POINT3DF',
               'typeinclude': '#include <vector>\n'
               '#include <aims/vector/vector.h>',
               'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_VECTOR_POINT3DF\n'
               '#define PYAIMS_WRAPPER_VECTOR_POINT3DF\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_vector<Point3df>()\n'
               '{ return sipFindType( "vector_POINT3DF" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_vector<Point3df>',
               'compareElement': '',
            },

            'std::vector<int32_t>':
            {
                'typecode': 'vector_S32',
                'pyFromC': 'pyaimsConvertFrom_vector',
                'CFromPy': 'pyaimsConvertTo_vector<int32_t>',
                'castFromSip': '',
                'deref': '*',
                'pyderef': '*',
                'address': '&',
                'pyaddress': '&',
                'defScalar': '',
                'defNumpyBindings': '',
                'new': 'new vector_S32',
                'NumType': 'NPY_OBJECT',
                'PyType': 'vector_S32',
                'sipClass': 'vector_S32',
                'typeinclude': '#include <vector>',
                'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
                '#ifndef PYAIMS_WRAPPER_VECTOR_S32\n'
                '#define PYAIMS_WRAPPER_VECTOR_S32\n'
                'template <> inline const sipTypeDef*\n'
                'sipType_vector<int32_t>()\n'
                '{ return sipFindType( "vector_S32" ); }\n'
                '#endif',
                'module': 'aims',
                'testPyType': 'pyaimsCheck_vector<int32_t>',
                'compareElement': '',
              },

            'std::vector<float>':
            {
                'typecode': 'vector_FLOAT',
                'pyFromC': 'pyaimsConvertFrom_vector',
                'CFromPy': 'pyaimsConvertTo_vector<float>',
                'castFromSip': '',
                'deref': '*',
                'pyderef': '*',
                'address': '&',
                'pyaddress': '&',
                'defScalar': '',
                'defNumpyBindings': '',
                'new': 'new vector_FLOAT',
                'NumType': 'PyArray_OBJECT',
                'PyType': 'vector_FLOAT',
                'sipClass': 'vector_FLOAT',
                'typeinclude': '#include <vector>',
                'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
                '#ifndef PYAIMS_WRAPPER_VECTOR_FLOAT\n'
                '#define PYAIMS_WRAPPER_VECTOR_FLOAT\n'
                'template <> inline const sipTypeDef*\n'
                'sipType_vector<float>()\n'
                '{ return sipFindType( "vector_FLOAT" ); }\n'
                '#endif',
                'module': 'aims',
                'testPyType': 'pyaimsCheck_vector<float>',
                'compareElement': '',
              },

            'std::vector<std::string>':
            {'typecode': 'vector_STRING',
               'pyFromC': 'pyaimsConvertFrom_vector',
               'CFromPy': 'pyaimsConvertTo_vector<std::string>',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new vector_STRING',
               'NumType': 'NPY_OBJECT',
               'PyType': 'vector_STRING',
               'sipClass': 'vector_STRING',
               'typeinclude': '#include <vector>\n'
               '#include <string>',
               'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
               '#ifndef PYAIMS_WRAPPER_VECTOR_STRING\n'
               '#define PYAIMS_WRAPPER_VECTOR_STRING\n'
               'template <> inline const sipTypeDef*\n'
               'sipType_vector<std::string>()\n'
               '{ return sipFindType( "vector_STRING" ); }\n'
               '#endif',
               'module': 'aims',
               'testPyType': 'pyaimsCheck_vector<std::string>',
               'compareElement': '',
            },

            'std::list<uint32_t>':
            {
                'typecode': 'list_U32',
                'pyFromC': 'pyaimsConvertFrom_list',
                'CFromPy': 'pyaimsConvertTo_list<uint32_t>',
                'castFromSip': '',
                'deref': '*',
                'pyderef': '*',
                'address': '&',
                'pyaddress': '&',
                'defScalar': '',
                'defNumpyBindings': '',
                'new': 'new list_U32',
                'NumType': 'NPY_OBJECT',
                'PyType': 'list_U32',
                'sipClass': 'list_U32',
                'typeinclude': '#include <list>',
                'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
                '#ifndef PYAIMS_WRAPPER_LIST_U32\n'
                '#define PYAIMS_WRAPPER_LIST_U32\n'
                'template <> inline const sipTypeDef*\n'
                'sipType_list<uint32_t>()\n'
                '{ return sipFindType( "list_U32" ); }\n'
                '#endif',
                'module': 'aims',
                'testPyType': 'pyaimsCheck_list<uint32_t>',
                'compareElement': '',
            },
            'std::list<AimsTimeSurface<3,Void> >':
            {
                'typecode': 'list_AimsTimeSurface_3_VOID',
                'pyFromC': 'pyaimsConvertFrom_list',
                'CFromPy': 'pyaimsConvertTo_list<AimsTimeSurface<3,Void> >',
                'castFromSip': '',
                'deref': '*',
                'pyderef': '*',
                'address': '&',
                'pyaddress': '&',
                'defScalar': '',
                'defNumpyBindings': '',
                'new': 'new list_AimsTimeSurface_3_VOID',
                'NumType': 'NPY_OBJECT',
                'PyType': 'list_AimsTimeSurface_3_VOID',
                'sipClass': 'list_AimsTimeSurface_3_VOID',
                'typeinclude': '#include <list>\n'
                    '#include <aims/mesh/surface.h>\n',
                'sipinclude': '#include <pyaims/vector/sipstdvector.h>\n'
                '#include <aims/mesh/surface.h>\n'
                '#ifndef PYAIMS_WRAPPER_LIST_AIMSTIMESURFACE_3_VOID\n'
                '#define PYAIMS_WRAPPER_LIST_AIMSTIMESURFACE_3_VOID\n'
                'template <> inline const sipTypeDef*\n'
                'sipType_list<AimsTimeSurface<3,Void> >()\n'
                '{ return sipFindType( "list_AimsTimeSurface_3_VOID" ); }\n'
                '#endif',
                'module': 'aims',
                'testPyType': 'pyaimsCheck_list<AimsTimeSurface<3,Void> >',
                'compareElement': '',
            },

            'carto::PluginLoader::PluginFile':
            classInNamespace('cartobase/plugin/plugin.h',
                             'PluginFile', 'carto_PluginLoader',
                             'carto::PluginLoader'),

            'aims::SparseMatrix':
            classInAimsNamespace('aims/sparsematrix/sparseMatrix.h',
                                 'SparseMatrix'),

            'aims::SparseOrDenseMatrix':
            classInAimsNamespace('aims/sparsematrix/sparseordensematrix.h',
                                 'SparseOrDenseMatrix'),

            'soma::DataSource':
            classInSomaNamespace('soma-io/datasource/datasource.h',
                                  'DataSource'),

            'soma::DataSourceInfo':
            classInSomaNamespace('soma-io/datasourceinfo/datasourceinfo.h',
                                  'DataSourceInfo'),

            'carto::rc_ptr<AimsTimeSurface_3_VOID>':
            {'typecode': 'rc_ptr_AimsTimeSurface_3_VOID',
               'pyFromC': 'pyaimsConvertFrom_rc_ptr_AimsTimeSurface_3_VOID',
               'CFromPy': 'pyaimsConvertTo_rc_ptr_AimsTimeSurface_3_VOID',
               'castFromSip': '',
               'deref': '*',
               'pyderef': '*',
               'address': '&',
               'pyaddress': '&',
               'defScalar': '',
               'defNumpyBindings': '',
               'new': 'new carto::rc_ptr<carto::rc_ptr<AimsTimeSurface<Void> > >',
               'NumType': 'PyArray_OBJECT',
               'PyType': 'rc_ptr_AimsTimeSurface_3_VOID',
               'sipClass': 'rc_ptr_AimsTimeSurface_3_VOID',
               'typeinclude': '#include <aims/mesh/surface.h>\n'
                  '#include <cartobase/smart/rcptr.h>',
               'sipinclude': '#ifndef PYAIMS_RC_PTR_TIMESURFACE_3_VOID_CHECK_DEFINED\n'
               '#define PYAIMS_RC_PTR_TIMESURFACE_3_VOID_CHECK_DEFINED\n'
               '#ifndef PYAIMS_AIMSTIMESURFACE_3_VOID_DEFINED\n'
               '#define PYAIMS_AIMSTIMESURFACE_3_void_DEFINED\n'
               'typedef AimsTimeSurface<3, Void > AimsTimeSurface_3_VOID;\n'
               '#endif\n'
               'inline int pyaimsRcptr_TimeSurface_3_VOID_Check( PyObject* o )\n'
               '{ return sipCanConvertToType( o, sipFindType( "rc_ptr_AimsTimeSurface_3_VOID" ), SIP_NOT_NONE | SIP_NO_CONVERTORS ); }\n'
               '#endif\n'
               '#ifndef PYAIMS_RC_PTR_TIMESURFACE_3_VOID_CONVERT_DEFINED\n'
               '#define PYAIMS_RC_PTR_TIMESURFACE_3_VOID_CONVERT_DEFINED\n'
               'inline carto::rc_ptr<AimsTimeSurface<3, Void> >* pyaimsConvertTo_rc_ptr_AimsTimeSurface_3_VOID( PyObject* o )\n'
               '{ int iserr = 0;\n'
               '  void *ptr = sipForceConvertToType( o, sipFindType( "rc_ptr_AimsTimeSurface_3_VOID" ), 0, 0, 0, &iserr );\n'
               '  if( iserr ) return 0;\n'
               '  return reinterpret_cast<carto::rc_ptr<AimsTimeSurface<3, Void> > *>( ptr );\n}\n'
               'inline PyObject* pyaimsConvertFrom_rc_ptr_AimsTimeSurface_3_VOID( void * a )\n'
               '{ return sipConvertFromType( a, sipFindType( "rc_ptr_AimsTimeSurface_3_VOID" ), 0 ); }\n'
               '#endif\n',
               'module': 'aims',
               'testPyType': 'pyaimsRcptr_TimeSurface_3_VOID_Check',
            },


            }

# add rc_ptr<Volume<T> >
for voltype in ('bool', 'int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t',
                'uint32_t', 'float', 'double', 'AimsRGB', 'AimsRGBA',
                'AimsHSV', 'cfloat', 'cdouble', 'Point3df'):
    k, v = rc_ptr_class('carto::Volume<%s>' % voltype, typessub)
    typessub[k] = v


if target_platform == 'windows-64':
    if sip.SIP_VERSION < 0x04130d:
        typessub['size_t'] = typessub['unsigned long long']
    else:
        typessub['size_t'] = \
           {'typecode': 'SIZE_T',
               'pyFromC': 'PyLong_FromLongLong',
               'CFromPy': 'PyLong_AsUnsignedLongLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'PyArray_UINT64',
               'PyType': 'size_t',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
               'needs_typedef': '1',
            }

elif sys.platform == 'darwin' \
        or platform.processor() == 'x86_64':
    if sip.SIP_VERSION < 0x04130d:
        typessub['size_t'] = typessub['unsigned long']
    else:
        typessub['size_t'] = \
            {'typecode': 'SIZE_T',
               'pyFromC': 'PyLong_FromLong',
               'CFromPy': 'PyLong_AsUnsignedLong',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'PyArray_UINT',
               'PyType': 'size_t',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
               'needs_typedef': '1',
            }
else:
    if sip.SIP_VERSION < 0x04130d:
        typessub['size_t'] = typessub['unsigned']
    else:   
        typessub['size_t'] = \
           {'typecode': 'SIZE_T',
               'pyFromC': 'carto::PyaimsInt_FromLong',
               'CFromPy': 'carto::uint32_FromPy',
               'castFromSip': '',
               'deref': '',
               'pyderef': '',
               'address': '',
               'pyaddress': '',
               'defScalar': '#define PYAIMS_SCALAR',
               'defNumpyBindings': '',
               'new': '',
               'NumType': 'PyArray_UINT32',
               'PyType': 'size_t',
               'sipClass': '',
               'typeinclude': '',
               'sipinclude': '#include <pyaims/object/numconv.h>',
               'module': 'aims',
               'testPyType': 'carto::PyaimsInt_Check',
               'compareElement': '',
               'needs_typedef': '1',
            },

completeTypesSub(typessub)
