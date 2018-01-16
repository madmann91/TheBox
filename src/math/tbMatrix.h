/************************************************************************
*                                                                       *
*  File : tbMatrix.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Matrix operations                                         *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_MATRIX_H__
#define __TB_MATRIX_H__

#include "tbCommon.h"

/*! \addtogroup math */
/*! @{ */

/*! \name Matrix */
/*! @{ */

typedef tbScalar tbMatrix[4];

/*! \brief Sets the matrix to the identity matrix */
void tbIdentity(tbMatrix);
/*! \brief Multiplies two matrices */
void tbMultiply(const tbMatrix,const tbMatrix,tbMatrix);
/*! \brief Transposes a matrix */
void tbTranspose(const tbMatrix,tbMatrix);
/*! \brief Computes the inverse of a matrix */
void tbInverse(const tbMatrix,tbMatrix);
/*! \brief Computes the determinant of a matrix */
tbScalar tbDeterminant(const tbMatrix);

/*! @} */

/*! @} */

#endif
