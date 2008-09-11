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


#ifndef AIMS_COMPRESSION_LZHL_H
#define AIMS_COMPRESSION_LZHL_H

#define ROTL( x, y ) ( ( (x) << (y) ) | ( (x) >> ( 32 - (y) ) ) )

#define LZMIN 4

//
// user tunable parameters
//

// Affect format
// LZBUFBITS is a log2(LZBUFSIZE) and must be in range 10 - 16
#define LZBUFBITS 16

// NOT affect format
#define LZMATCH 5

#define LZSLOWHASH
// LZTABLEBITS is a log2(LZTABLESIZE) and should be in range 9 - 17
#define LZTABLEBITS 15

#define LZOVERLAP
#define LZBACKWARDMATCH
#define LZLAZYMATCH
#define LZSKIPHASH 1024

// HUFFRECALCLEN should be <= 16384
#define HUFFRECALCLEN 4096

#define LZTABLESIZE (1<<(LZTABLEBITS))
#define LZBUFSIZE (1<<(LZBUFBITS))


#endif
