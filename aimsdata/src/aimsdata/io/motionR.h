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

#ifndef AIMS_IO_MOTIONR_H
#define AIMS_IO_MOTIONR_H

#include <aims/config/aimsdata_config.h>
#include <fstream>
#include <string>

#include <aims/resampling/motion.h> 

class MotionReader;

AIMSDATA_API MotionReader& operator >> ( MotionReader&, Motion& );


/** Class for Motion reading operation.
*/
class AIMSDATA_API MotionReader
{ 
public:
  /**@name Constructor and Destructor*/
  //@{
  /** The programmer should give the file name to the reader.
      @param name referes to the file name
 
  */
  MotionReader( const std::string& name );
  /// Destructor does nothing special
  virtual ~MotionReader();
  //@}

  /**@name Methods*/
  //@{
  /// set input file name
  inline void setFileName( const std::string &fileName );

  /** Read the Motion with "name" file name from disk

  */
  void read( Motion& thing );

  friend MotionReader& operator >> ( MotionReader&, Motion& );

private:

  /**@name Data*/
  //@{
  /// File name
  std::string   _name;
  //@}
};


inline void MotionReader::setFileName( const std::string &fileName )
{
  _name = fileName;
}


#endif
