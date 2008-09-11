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

#include <cartobase/object/object_d.h>
#include <cartobase/object/property.h>
#include <assert.h>
#include <iostream>
#include <cartobase/stream/sstream.h>
#include <time.h>

using namespace carto;
using namespace std;

class MyInterface : public virtual Interface
{
  virtual void myMethod() = 0;
};

class MyScalar : public ScalarInterface, public MyInterface
{
public:
  double value;

  double getScalar() const;
  void setScalar( double );
  void myMethod();
};


double MyScalar::getScalar() const
{
  return value;
}

void MyScalar::setScalar( double v )
{
  value = v;
}

void MyScalar::myMethod()
{
}

namespace carto {
DECLARE_GENERIC_OBJECT_TYPE( MyScalar )
}
INSTANTIATE_GENERIC_OBJECT_TYPE( MyScalar )

int main()
{
  try {
    Object object;
    assert( object.isNull() );
    
    // Test ValueObject creation and value access
    cout << "ValueObject test..." << endl;
    uint8_t u8 = 8;
    object = Object::value( u8 );
    assert( object->type() == DataTypeCode<uint8_t>::name() );
    assert( object->value<uint8_t>() == 8 );
    
    int8_t s8 = -8;
    object = Object::value( s8 );
    assert( object->type() == DataTypeCode<int8_t>::name() );
    assert( object->value<int8_t>() == -8 );
    
    uint16_t u16 = 16;
    object = Object::value( u16 );
    assert( object->type() == DataTypeCode<uint16_t>::name() );
    assert( object->value<uint16_t>() == 16 );
    
    int16_t s16 = -16;
    object = Object::value( s16 );
    assert( object->type() == DataTypeCode<int16_t>::name() );
    assert( object->value<int16_t>() == -16 );
    
    uint32_t u32 = 32;
    object = Object::value( u32 );
    assert( object->type() == DataTypeCode<uint32_t>::name() );
    assert( object->value<uint32_t>() == 32 );
    
    int32_t s32 = -32;
    object = Object::value( s32 );
    assert( object->type() == DataTypeCode<int32_t>::name() );
    assert( object->value<int32_t>() == -32 );
    
    float f = 1.2F;
    object = Object::value( f );
    assert( object->type() == DataTypeCode<float>::name() );
    assert( object->value<float>() == 1.2F );
    
    double d = 3.4;
    object = Object::value( d );
    assert( object->type() == DataTypeCode<double>::name() );
    assert( object->value<double>() == 3.4 );
    
    string s = "content";
    object = Object::value( s );
    assert( object->type() == DataTypeCode<string>::name() );
    assert( object->value<string>() == "content" );
    cout << "OK." << endl;

    // Test ReferenceObject creation, value access and value modification

    cout << "ReferenceObject test..." << endl;
    object = Object::reference( u8 );
    assert( object->type() == DataTypeCode<uint8_t>::name() );
    assert( object->value<uint8_t>() == 8 );
    object->setScalar( 9 );
    assert( u8 == 9 );
    
    object = Object::reference( s8 );
    assert( object->type() == DataTypeCode<int8_t>::name() );
    assert( object->value<int8_t>() == -8 );
    object->setScalar( -9 );
    assert( s8 == -9 );
    
    object = Object::reference( u16 );
    assert( object->type() == DataTypeCode<uint16_t>::name() );
    assert( object->value<uint16_t>() == 16 );
    object->setScalar( 17 );
    assert( u16 == 17 );
    
    object = Object::reference( s16 );
    assert( object->type() == DataTypeCode<int16_t>::name() );
    assert( object->value<int16_t>() == -16 );
    object->setScalar( -17 );
    assert( s16 == -17 );
    
    object = Object::reference( u32 );
    assert( object->type() == DataTypeCode<uint32_t>::name() );
    assert( object->value<uint32_t>() == 32 );
    object->setScalar( 33 );
    assert( u32 == 33 );
    
    object = Object::reference( s32 );
    assert( object->type() == DataTypeCode<int32_t>::name() );
    assert( object->value<int32_t>() == -32 );
    object->setScalar( -33 );
    assert( s32 == -33 );
    
    object = Object::reference( f );
    assert( object->type() == DataTypeCode<float>::name() );
    assert( object->value<float>() == 1.2F );
    object->setScalar( 2.2F );
    assert( f == 2.2F );
    
    object = Object::reference( d );
    assert( object->type() == DataTypeCode<double>::name() );
    assert( object->value<double>() == 3.4 );
    object->setScalar( 4.4 );
    assert( d == 4.4 );
    
    object = Object::reference( s );
    assert( object->type() == DataTypeCode<string>::name() );
    assert( object->value<string>() == "content" );
    object->setString( "new" );
    assert( s == "new" );
    cout << "OK." << endl;

    // Test PropertySet

    cout << "PropertySet test..." << endl;
    PropertySet ps;
    ps.addBuiltinProperty( "u8", u8 );
    ps.addBuiltinProperty( "s8", s8 );
    ps.addBuiltinProperty( "u16", u16 );
    ps.addBuiltinProperty( "s16", s16 );
    ps.addBuiltinProperty( "u32", u32 );
    ps.addBuiltinProperty( "s32", s32 );
    ps.addBuiltinProperty( "float", f );
    ps.addBuiltinProperty( "double", d );
    ps.addBuiltinProperty( "string", s );
    int		opt1 = 0, opt2 = 0, opt3 = 0;
    bool	opt1ok = false, opt2ok = false, opt3ok = true;
    ps.addBuiltinProperty( "optional1", opt1, opt1ok );
    ps.addBuiltinProperty( "optional2", opt2, opt2ok );
    ps.addBuiltinProperty( "optional3", opt3, opt3ok );
    
    ps.setProperty( "u8", (uint8_t) 1 );
    ps.setProperty( "s8", (int8_t) 2 );
    ps.setProperty( "u16", (uint16_t) 3 );
    ps.setProperty( "s16", (int16_t) 4 );
    ps.setProperty( "u32", (uint32_t) 5 );
    ps.setProperty( "s32", (int32_t) 6 );
    ps.setProperty( "float", (float) 7 );
    ps.setProperty( "double", (double) 8 );
    ps.setProperty( "string", string( "yop" ) );
    assert( u8 == 1 );
    assert( s8 == 2 );
    assert( u16 == 3 );
    assert( s16 == 4 );
    assert( u32 == 5 );
    assert( s32 == 6 );
    assert( f == 7 );
    assert( d == 8 );
    assert( s == "yop" );
    ps.setProperty( "optional2", 12 );
    assert( !ps.hasProperty( "optional1" ) );
    assert( ps.hasProperty( "optional2" ) );
    assert( ps.hasProperty( "optional3" ) );
    assert( opt2 == 12 );
    assert( opt2ok == true );
    ps.setProperty( "optional3", 23 );
    assert( opt3 == 23 );
    assert( opt3ok == true );
    assert( ps.removeProperty( "optional3" ) );
    assert( opt3ok == false );

    Object ops = Object::reference( ps );
    ops->setProperty( "xu8", (uint8_t) 9 );
    ops->setProperty( "xs8", (int8_t) 10 );
    ops->setProperty( "xu16", (uint16_t) 11 );
    ops->setProperty( "xs16", (int16_t) 12 );
    ops->setProperty( "xu32", (uint32_t) 13 );
    ops->setProperty( "xs32", (int32_t) 14 );
    ops->setProperty( "xfloat", (float) 15 );
    ops->setProperty( "xdouble", (double) 16 );
    ops->setProperty( "xstring", "yep" );
    assert( ops->getProperty( "xu8" )->getScalar() == 9 );
    assert( ops->getProperty( "xs8" )->getScalar() == 10 );
    assert( ops->getProperty( "xu16" )->getScalar() == 11 );
    assert( ops->getProperty( "xs16" )->getScalar() == 12 );
    assert( ops->getProperty( "xu32" )->getScalar() == 13 );
    assert( ops->getProperty( "xs32" )->getScalar() == 14 );
    assert( ops->getProperty( "xfloat" )->getScalar() == 15 );
    assert( ops->getProperty( "xdouble" )->getScalar() == 16 );
    assert( ops->getProperty( "xstring" )->getString() == "yep" );

    assert( ops->size() == 19 );

    
    assert( ops->removeProperty( "u32" ) == false );
    assert( ops->removeProperty( "xu32" ) == true );
    std::ostringstream stream;
    for( Object it = ps.objectIterator(); it->isValid(); it->next() ) {
      stream << it->key() << ": "<< it->currentValue()->getString() << endl;
    }
    assert( stream.str() == "u8: 1\ns8: 2\nu16: 3\ns16: 4\nu32: 5\ns32: 6\nfloat: 7\ndouble: 8\nstring: yop\noptional2: 12\nxu8: 9\nxs8: 10\nxu16: 11\nxs16: 12\nxs32: 14\nxfloat: 15\nxdouble: 16\nxstring: yep\n" );

    cout << "OK." << endl;

    cout << "Custom object test..." << endl;
    MyScalar ms;
    ms.value = 13.5;
    Object oms = Object::reference( ms );
    ScalarInterface *si = oms->getInterface< ScalarInterface >();
    assert( si != NULL );
    assert( si->getScalar() == ms.value );
    StringInterface *sti = oms->getInterface< StringInterface >();
    assert( sti == NULL );
    MyInterface *myi = oms->getInterface< MyInterface >();
    assert( myi != NULL );
    cout << "OK" << endl;

    cout << "Performance test..." << endl;
    ps.setProperty( "optional1", opt1 );
    PropertySet		ps2;
    ps2.addBuiltinProperty( "u8", u8 );
    ps2.addBuiltinProperty( "s8", s8 );
    ps2.addBuiltinProperty( "u16", u16 );
    ps2.addBuiltinProperty( "s16", s16 );
    ps2.addBuiltinProperty( "u32", u32 );
    ps2.addBuiltinProperty( "s32", s32 );
    ps2.addBuiltinProperty( "float", f );
    ps2.addBuiltinProperty( "double", d );
    ps2.addBuiltinProperty( "string", s );
    int		opt1b = 0, opt2b = 0, opt3b = 0;
    bool	opt1okb = false, opt2okb = false, opt3okb = true;
    ps2.addBuiltinProperty( "optional1", opt1b, opt1okb );
    ps2.addBuiltinProperty( "optional2", opt2b, opt2okb );
    ps2.addBuiltinProperty( "optional3", opt3b, opt3okb );
    unsigned	i, n = 100000;
    clock_t	ck = clock();
    for( i=0; i<n; ++i )
      ps2 = ps;
    cout << "PropertySet: " << n << " copies of an object of " << ps.size() 
         << " elements: " << float( clock() - ck ) / CLOCKS_PER_SEC  << " s." 
         << endl;
    Dictionary	dic, dic2;
    Object	dico = Object::reference( dic );
    
    dico->setProperty( "u8", (uint8_t) 1 );
    dico->setProperty( "s8", (int8_t) 2 );
    dico->setProperty( "u16", (uint16_t) 3 );
    dico->setProperty( "s16", (int16_t) 4 );
    dico->setProperty( "u32", (uint32_t) 5 );
    dico->setProperty( "s32", (int32_t) 6 );
    dico->setProperty( "float", (float) 7 );
    dico->setProperty( "double", (double) 8 );
    dico->setProperty( "string", string( "yop" ) );
    dico->setProperty( "optional1", opt1 );
    dico->setProperty( "optional2", opt2 );
    dico->setProperty( "xu8", (uint8_t) 9 );
    dico->setProperty( "xs8", (int8_t) 10 );
    dico->setProperty( "xu16", (uint16_t) 11 );
    dico->setProperty( "xs16", (int16_t) 12 );
    dico->setProperty( "xs32", (int32_t) 14 );
    dico->setProperty( "xfloat", (float) 15 );
    dico->setProperty( "xdouble", (double) 16 );
    dico->setProperty( "xstring", "yep" );
    ck = clock();
    for( i=0; i<n; ++i )
      dic2 = dic;
    cout << "Dictionary: " << n << " copies of an object of " << dic.size() 
         << " elements: " << float( clock() - ck ) / CLOCKS_PER_SEC  << " s." 
         << endl;
 }
  catch( exception &e ) {
    cerr << e.what() << endl;
    exit( 1 );
  }
  catch( ... ) {
    cerr << "Unexpected exception" << endl;
    exit( 1 );
  }
}
