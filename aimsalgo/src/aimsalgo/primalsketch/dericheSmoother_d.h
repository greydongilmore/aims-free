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


#ifndef AIMS_PRIMALSKETCH_DERICHESMOOTHER_D_H
#define AIMS_PRIMALSKETCH_DERICHESMOOTHER_D_H

#include <cstdlib>
#include <aims/primalsketch/dericheSmoother.h>
#include <stdlib.h>

template<class T> AimsData<T> DericheSmoother<T>::doSmoothing(const AimsData<T> & ima, int maxiter, bool verbose) {

  if (maxiter >= 0)
    {
			if ((ima.sizeX()==ima.sizeY()) && (ima.sizeX()==ima.sizeZ()))
			{
				AimsData<T> ima2=ima.clone(); // this is to avoid problem caused by
											// copy constructor that just makes a reference
				float sigma=sqrt(maxiter*ima.sizeX()*ima.sizeX());
		      	std::cout << "sigma=" << sigma << std::endl;
      			Gaussian3DSmoothing<T> gaussSmooth(sigma, sigma, sigma);
      			return gaussSmooth.doit(ima2);
			}
			else
			{
				std::cerr << "Anisotropic images not supported (yet) for scale-space applications..." << std::endl;
				exit(EXIT_FAILURE);
			}
    }
  else
    {
      std::cerr << "DericheRecursiveConvolution Smoother: must have tIn < tOut" << std::endl;
      exit(EXIT_FAILURE);
    }
}

#endif
