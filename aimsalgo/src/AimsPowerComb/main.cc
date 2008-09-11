/* Copyright (c) 1995-2007 CEA
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


//	Uses 3 levels of templates, hope your compiler is strong enough...

#include <aims/data/data_g.h>
#include <aims/io/finder.h>
#include <aims/io/process.h>
#include <aims/io/reader.h>
#include <aims/io/writer.h>
#include <aims/getopt/getopt.h>
#include <aims/utility/utility_g.h>
#include <aims/mesh/texture.h>
#include <assert.h>
#include <math.h>

using namespace aims;
using namespace std;


BEGIN_USAGE(usage)
  "------------------------------------------------------------------",
  "AimsPowerComb  -i <i1>  [-a <num1>] [-b <den1>]                  ",
  "               [-j <i2>  [-c <num2>] [-d <den2>]]                 ",
  "               [-e <cst>]                                         ",
  "                -o[utput] <fileout>                               ",
  "               [-t[ype] <datatype>                                ",
  "               [-h[elp]]                                          ",
  "------------------------------------------------------------------",
  "Power combination e * I1^(a/b) * I2^(c/d)                         ",
  "                                                                  ",
  "Note : a,b,c,d,e must be real                                     ",
  "------------------------------------------------------------------",
  "     i1      : first data (volume or texture)                     ",
  "     num1    : multiplicative coefficient for the first data [1.0]",
  "     den1    : divisor coefficient for the first data [1.0]       ",
  "     i2      : second data (volume or texture)                    ",
  "               (if none, I1^(a/b) ) ",
  "     num2    : multiplicative coefficient for the second data[1.0]",
  "     den2    : divisor coefficient for the second data [1.0]      ",
  "     cst     : constant offset value [default=1.0]                ",
  "     fileout : destination data                                   ",
  "     datatype: data type of the destination volume/texture (S16, U8, ...) ",
  "               [default=type of in1]                              ",
  "               For texture, i1, i2 and fileout must have the same type",
  "------------------------------------------------------------------",
END_USAGE


void Usage( void )
{
  AimsUsage( usage );
}



template<class T>
static bool linread1( Process & p, const string & fname, Finder & f );


class LinRead1 : public Process
{
public:
  LinRead1( float ca, float cb, float cc, float cd, float ce, 
	    const string & im2, const string & out, const string & t ) 
    : Process(), a( ca ), b( cb ), c( cc ), d( cd ), e( ce ), i2( im2 ), 
      fout( out ), type( t )
  {
    registerProcessType( "Volume", "S8", &linread1<AimsData<int8_t> > );
    registerProcessType( "Volume", "U8", &linread1<AimsData<uint8_t> > );
    registerProcessType( "Volume", "S16", &linread1<AimsData<int16_t> > );
    registerProcessType( "Volume", "U16", &linread1<AimsData<uint16_t> > );
    registerProcessType( "Volume", "S32", &linread1<AimsData<int32_t> > );
    registerProcessType( "Volume", "U32", &linread1<AimsData<uint32_t> > );
    registerProcessType( "Volume", "FLOAT", &linread1<AimsData<float> > );
    registerProcessType( "Volume", "DOUBLE", &linread1<AimsData<double> > );
    registerProcessType( "Texture", "FLOAT", &lintex<float > );
    registerProcessType( "Texture", "S16", &lintex<short > );

  }

  template<class T>
  friend bool linread1( Process & p, const string & fname, Finder & f );

  template<class T>
  friend bool lintex( Process & p, const string & fname, Finder & f );


  float	a,b,c,d,e;
  string	i2;
  string	fout;
  string	type;
};



template<class T, class U>
static bool linread2( Process & p, const string & fname, Finder & f );


template<class T>
class LinRead2 : public Process
{
public:
  LinRead2( LinRead1 & l1, T & data );

  LinRead1	& lin1;
  T		in1;
};


template<class T>
LinRead2<T>::LinRead2( LinRead1 & l1, T & data ) : Process(), lin1( l1 ), in1( data )
{
  in1.setHeader( data.header()->cloneHeader() );
  registerProcessType( "Volume", "S8", &linread2<T,AimsData<int8_t> > );
  registerProcessType( "Volume", "U8", &linread2<T,AimsData<uint8_t> > );
  registerProcessType( "Volume", "S16", &linread2<T,AimsData<int16_t> > );
  registerProcessType( "Volume", "U16", &linread2<T,AimsData<uint16_t> > );
  registerProcessType( "Volume", "S32", &linread2<T,AimsData<int32_t> > );
  registerProcessType( "Volume", "U32", &linread2<T,AimsData<uint32_t> > );
  registerProcessType( "Volume", "FLOAT", &linread2<T,AimsData<float> > );
  registerProcessType( "Volume", "DOUBLE", &linread2<T,AimsData<double> > );

}


template<class T,class U,class V> static 
bool linwrite( Process & p, const string & fname, Finder & f );

template<class T,class U>
class LinWrite : public Process
{
public:
  LinWrite( LinRead2<T> & l2, U & data );

  LinRead2<T>	& lin2;
  U		in2;
};




template<class T,class U>
LinWrite<T,U>::LinWrite( LinRead2<T> & l2, U & data ) : Process(), lin2( l2 ), in2( data )
{
  in2.setHeader( data.header()->cloneHeader() );
  registerProcessType( "Volume", "S8", &linwrite<T,U,int8_t> );
  registerProcessType( "Volume", "U8", &linwrite<T,U,uint8_t> );
  registerProcessType( "Volume", "S16", &linwrite<T,U,int16_t> );
  registerProcessType( "Volume", "U16", &linwrite<T,U,uint16_t> );
  registerProcessType( "Volume", "S32", &linwrite<T,U,int32_t> );
  registerProcessType( "Volume", "U32", &linwrite<T,U,uint32_t> );
  registerProcessType( "Volume", "FLOAT", &linwrite<T,U,float> );
  registerProcessType( "Volume", "DOUBLE", &linwrite<T,U,double> );
}


template<class T,class U>
static bool linwrite2( Process & p, const string & fname, Finder & f );

template<class T>
class LinWrite2 : public Process
{
public:
  LinWrite2( LinRead1 & l1, T & data );

  LinRead1	& lin1;
  T		in1;
};


template<class T>
LinWrite2<T>::LinWrite2( LinRead1 & l1, T & data ) : Process(), lin1( l1 ), in1( data )
{
  in1.setHeader( data.header()->cloneHeader() );
  registerProcessType( "Volume", "S8", &linwrite2<T,int8_t> );
  registerProcessType( "Volume", "U8", &linwrite2<T,uint8_t> );
  registerProcessType( "Volume", "S16", &linwrite2<T,int16_t> );
  registerProcessType( "Volume", "U16", &linwrite2<T,uint16_t> );
  registerProcessType( "Volume", "S32", &linwrite2<T,int32_t> );
  registerProcessType( "Volume", "U32", &linwrite2<T,uint32_t> );
  registerProcessType( "Volume", "FLOAT", &linwrite2<T,float> );
  registerProcessType( "Volume", "DOUBLE", &linwrite2<T,double> );
}


template<class T> 
bool linread1( Process & p, const string & fname, Finder & f )
{
  LinRead1	& lp = (LinRead1 &) p;
  T		data;
  string	format = f.format();
  Reader<T>	r( fname );
  cout << "Reading image " << fname << "...\n";
  if( !r.read( data, 0, &format ) )
    return( false );

  cout << "done\n";
  if( lp.type.empty() )
    lp.type = f.dataType();

  if( lp.i2.empty() )
    {
      LinWrite2<T>	proc( lp, data );
      Finder	f2;
      f2.setObjectType( f.objectType() );
      f2.setDataType( lp.type );
      return( proc.execute( f2, lp.fout ) );
    }
  else
    {
      LinRead2<T>	proc( lp, data );
      return( proc.execute( lp.i2 ) );
    }
}

template<class T> 
bool lintex( Process & p, const string & fname, Finder & f )
{
  LinRead1	& lp = (LinRead1 &) p;
  TimeTexture<T>		tex1,tex2,texO;
  string	format = f.format();
  Reader<TimeTexture<T> >	r1( fname );
  cout << "Reading " << fname << "..." << flush;
  if( !r1.read( tex1, 0, &format ) )
    return( false );

  cout << "done\n";
  if( lp.type.empty() )
    lp.type = f.dataType();
  
  unsigned		i , n = tex1[0].nItem();  
  float	a = lp.a;
  float	b = lp.b;
  float	c = lp.c;
  float	d = lp.d;
  float	e = lp.e;
 
   
  if( lp.i2.empty() )
    {
      cout << "processing..." << endl;
      for(i = 0; i < n; ++i)
	texO.push_back( T (e *  pow( (float) tex1.item(i) , a / b )  ) );
      cout << "done" << endl;    
    }
  else
    { 
      Reader<TimeTexture<T> >	r2( lp.i2 );
      cout << "Reading " << lp.i2 << "..." << flush;
      if( !r2.read( tex2, 0, &format ) )
	return( false );
      cout << "done\n";

      for(i = 0; i < n; ++i)
	texO.push_back( T ( e *  pow( (float) tex1.item(i) , a / b) * 
                             pow((float) tex2.item(i), c / d) ) );
    }
  
  Writer<TimeTexture<T>	> w( lp.fout );
  cout << "writing result..." << endl;
  return( w.write( texO ) );
}


template<class T, class U>
bool linread2( Process & p, const string & fname, Finder & f )
{
  LinRead2<T>	& lp = (LinRead2<T> &) p;
  U		data;
  string	format = f.format();
  Reader<U>	r( fname );
  cout << "reading image " << fname << "...\n";
  if( !r.read( data, 0, &format ) )
    return( false );

  cout << "done\n";
  assert( lp.in1.dimX() == data.dimX() &&
	  lp.in1.dimY() == data.dimY() &&
	  lp.in1.dimZ() == data.dimZ() &&
	  lp.in1.dimT() == data.dimT() );

  LinWrite<T,U>	proc( lp, data );
  Finder	f2;
  f2.setObjectType( f.objectType() );
  f2.setDataType( lp.lin1.type );
  return( proc.execute( f2, lp.lin1.fout ) );
}


template<class T,class U,class V>
bool linwrite( Process & p, const string & fname, Finder & )
{
  LinWrite<T,U>	& lp = (LinWrite<T,U> &) p;
  T		& in1 = lp.lin2.in1;
  U		& in2 = lp.in2;
  AimsData<V>	data( in1.dimX(), in1.dimY(), in1.dimZ(), in1.dimT() );
  int		x, y, z, t;
  float	a = lp.lin2.lin1.a;
  float	b = lp.lin2.lin1.b;
  float	c = lp.lin2.lin1.c;
  float	d = lp.lin2.lin1.d;
  float	e = lp.lin2.lin1.e;

  data.setSizeXYZT( in1.sizeX(), in1.sizeY(), in1.sizeZ(), in1.sizeT() );
  cout << "processing..." << endl;
  ForEach4d( data , x , y , z , t)
    data( x, y, z, t ) = V( e *  pow( (float) in1( x, y, z, t ) , (float) a / b)  * 
                            pow( (float) in2( x, y, z, t ) , (float) c / d ) );
  cout << "done" << endl;

  data.setHeader( in1.header()->cloneHeader() );

  Writer<AimsData<V> >	w( fname );
  cout << "writing result..." << endl;
  return( w.write( data ) );
}


template<class T,class U>
bool linwrite2( Process & p, const string & fname, Finder & )
{
  LinWrite2<T>	& lp = (LinWrite2<T> &) p;
  T		& in1 = lp.in1;
  AimsData<U>	data( in1.dimX(), in1.dimY(), in1.dimZ(), in1.dimT() );
  int		x, y, z, t;
  float	a = lp.lin1.a;
  float	b = lp.lin1.b;
  float	e = lp.lin1.e;

  data.setSizeXYZT( in1.sizeX(), in1.sizeY(), in1.sizeZ(), in1.sizeT() );
  cout << "processing..." << endl;
  ForEach4d( data , x , y , z , t)
    data( x, y, z, t ) = U( e *  pow( (float) in1( x, y, z, t ), a / b ) );
  cout << "done" << endl;

  data.setHeader( in1.header()->cloneHeader() );
  Writer<AimsData<U> >	w( fname );
  cout << "writing result..." << endl;
  return( w.write( data ) );
}


int main( int argc, char **argv )
{
  char *i1 = NULL, *i2 = NULL, *fileout = NULL, *type = 0;
  float num1 = 1.0, num2 = 1.0, den1 = 1.0, den2 = 1.0, cst = 1.0;

  AimsOption opt[] = {
  { 'h',"help"  ,AIMS_OPT_FLAG   ,( void* )Usage       ,AIMS_OPT_CALLFUNC,0,},
  { 'i',"i"     ,AIMS_OPT_STRING ,&i1         ,0                ,1},
  { 'a',"a"     ,AIMS_OPT_FLOAT ,&num1       ,0                ,0},
  { 'b',"b"     ,AIMS_OPT_FLOAT ,&den1       ,0                ,0},
  { 'j',"j"     ,AIMS_OPT_STRING ,&i2         ,0                ,0},
  { 'c',"c"     ,AIMS_OPT_FLOAT ,&num2       ,0                ,0},
  { 'd',"d"     ,AIMS_OPT_FLOAT ,&den2       ,0                ,0},
  { 'e',"e"     ,AIMS_OPT_FLOAT ,&cst        ,0                ,0},
  { 'o',"output",AIMS_OPT_STRING ,&fileout    ,0                ,0},
  { 't',"type  ",AIMS_OPT_STRING ,&type       ,0                ,0},
  { 0  ,0       ,AIMS_OPT_END    ,0           ,0                ,0}};

  AimsParseOptions( &argc, argv, opt, usage );

  cout << "options parsed" << endl;
  string	stype;
  if( type )
    stype = type;

  cout << "i1      : " << i1 << endl;
  cout << "a       : " << num1 << endl;
  cout << "b       : " << den1 << endl;
  cout << "c       : " << num2 << endl;
  cout << "d       : " << den2 << endl;
  cout << "e       : " << cst << endl;
  if( i2 )
    cout << "i2      : " << i2 << endl;
  cout << "fileout : " << fileout << endl;
  cout << "type    : " << stype << endl;

  string	im2;
  if( i2 )
    im2 = i2;

  LinRead1	proc( num1, den1, num2, den2, cst, im2, fileout, stype );
  if( !proc.execute( i1 ) )
    {
      cerr << "Couldn't process" << endl;
      return 1;
    }
  cout << "done" << endl;
  return EXIT_SUCCESS;
}
