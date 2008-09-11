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

/*
 *  Inversion and determinant of a matrix.
 */
#ifndef AIMS_MATH_GAUSSLU_H
#define AIMS_MATH_GAUSSLU_H

#include <aims/config/aimsdata_config.h>
#include <aims/def/general.h>

template <class T> class AimsData;


/**@name Resolution of linear systems.*/
//@{

/**@name LU decomposition of matrix.
    The source matrix is a. As output, a is the LU decomposition. indx is
    an output vector that records the row permutation effected by the partial
    pivoting. d is output as +1 or -1 depending wether the number of row
    interchanges was even or odd.
*/
//@{
///
AIMSDATA_API float
AimsDecompositionLU(AimsData<float> &a,
                    AimsData<int32_t>  &indx);
//@}

/**@name LU forward and backward substitution.
    Solves the set of N linear equations A.X=B. Here a is input but in LU
    form. indx is input as the permutation vector returned by LU
    decomposition. b is input as the right-hand side vector B, and returns
    with the solution vector X.
*/
//@{
///
AIMSDATA_API void AimsBackSubstitutionLU(AimsData<float> &a,
					 AimsData<int32_t>  &indx,
					 AimsData<float> &b);
//@}


/**@name Inverse of a matrix.
*/
//@{
/// Inverse of a float matrix
AIMSDATA_API AimsData<float> 
AimsInversionLU(const AimsData<float> &matrix);
/// Inverse of a complex float matrix
AIMSDATA_API AimsData< cfloat > 
AimsInversionLU(const AimsData< cfloat > &matrix);
//@}


/**@name Resolution of the linear equation A.X = B.
*/
//@{
/// Resolution in float 
AIMSDATA_API AimsData<float> 
AimsLinearResolutionLU(const AimsData<float> &matrix,
                       const AimsData<float> &b);
/// Resolution in complex float
AIMSDATA_API AimsData< cfloat >
AimsLinearResolutionLU(const AimsData< cfloat > &matrix,
                       const AimsData< cfloat > &b);
//@}


/**@name Determinant of a matrix.
*/
//@{
///
AIMSDATA_API float
AimsDeterminantLU(const AimsData<float> &matrix);
//@}

/**@name Linear resolution of Toeplitz like system.
*/
//@{
/** Resolution of system R.x = y where R is a toeplitz matrix decomposed
    in r=[r(-(N-1)), ...., r(-1), r(0), r(1), ..., r(N-1)]. The result is 
    given in x
*/
AIMSDATA_API void
AimsToeplitz( const AimsData<float>& r,
              const AimsData<float>& y,
              AimsData<float>& x );
//@}

//@}

#endif







