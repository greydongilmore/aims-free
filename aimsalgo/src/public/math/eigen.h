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


#ifndef AIMS_MATH_EIGEN_H
#define AIMS_MATH_EIGEN_H

#include <aims/def/general.h>

template <class T> class AimsData;


/** @name Eigen system resolution. */
template < class T >
class AimsEigen
{
public:

  enum EigenReturnType
  {
    MatrixOfEigenValues,
    VectorOfEigenValues
  };

  /** @name Constructor and destructor */
  //@{
  /** constructor. \\
      Here, we suppose that we mainly want to solve general eigen
      system with a non-symmetric matrix on input */
  AimsEigen( EigenReturnType rt=MatrixOfEigenValues ) : retType( rt ) { }
  /// destructor
  virtual ~AimsEigen() { }
  //@}

  void setReturnType( EigenReturnType rt ) { retType = rt; }

  /** Eigen system resolution function. \\
      The various computation function used within this one are adapted
      from the Numerical Recipes in C.\\
      This function returns the diagonal matrix of eigenvalues, and the
      input matrix is transformed into a matrix which columns are the 
      eigenvectors. \\
      @param wi return the diagonal matrix of the imaginary parts of the
      eigenvalues if *wi exists.
  */
  AimsData< T > doit( AimsData< T >&, AimsData< T > *wi = NULL );

  /// Sort the eigenvalues and eigenvectors in decreasing order.
  void sort( AimsData< T >&, AimsData< T >&, AimsData< T > *wi = NULL );

private:

  EigenReturnType retType;
};

#endif
