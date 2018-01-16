/************************************************************************
*                                                                       *
*  File : tbVector.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Vector operations                                         *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_VECTOR_H__
#define __TB_VECTOR_H__

#include "tbCommon.h"

/*! \addtogroup math */
/*! @{ */

/*! \name Vector */
/*! @{ */

typedef tbScalar tbVector[2];

/*! \brief Sets the vector to zero */
void tbZero(tbVector);
/*! \brief Computes the dot product between two vectors */
tbScalar tbDot(const tbVector,const tbVector);
/*! \brief Finds a perpendicular vector */
void tbPerp(const tbVector,tbVector);
/*! \brief Returns the vector length */
tbScalar tbLength(const tbVector);
/*! \brief Returns the square vector length */
tbScalar tbSquareLength(const tbVector);
/*! \brief Computes the sum of two vectors */
void tbSum(const tbVector,const tbVector,tbVector);
/*! \brief Computes the difference of two vectors */
void tbDiff(const tbVector,const tbVector,tbVector);
/*! \brief Scales a vector by a scalar value */
void tbScale(tbVector,tbScalar);
/*! \brief Computes the minimum of two vectors */
void tbMin(const tbVector,const tbVector,tbVector);
/*! \brief Computes the maximum of two vectors */
void tbMax(const tbVector,const tbVector,tbVector);

/*! @} */

/*! @} */

#endif
