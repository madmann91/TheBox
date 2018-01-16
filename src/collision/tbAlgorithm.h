/************************************************************************
*                                                                       *
*  File : tbAlgorithm.h                                                 *
*  Date : 29/10/10                                                      *
*  Comments : Collision algorithms                                      *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_ALGORITHM_H__
#define __TB_ALGORITHM_H__

#include "tbCommon.h"
#include "tbCollision.h"
#include "tbObject.h"

/*! \addtogroup collision */
/*! @{ */

/*! \name Algorithms */
/*! @{ */

#define	TB_ALGORITHM_ITERATIONS 4

/*! \brief Tests a box against a box */
int tbBoxToBox(const tbShape*,const tbShape*,const tbState*,const tbState*,tbCollision*);
/*! \brief Tests a box against a circle */
int tbBoxToCircle(const tbShape*,const tbShape*,const tbState*,const tbState*,tbCollision*);
/*! \brief Tests a circle against a box */
int tbCircleToBox(const tbShape*,const tbShape*,const tbState*,const tbState*,tbCollision*);
/*! \brief Tests a circle against a circle */
int tbCircleToCircle(const tbShape*,const tbShape*,const tbState*,const tbState*,tbCollision*);

/*! @} */

/*! @} */

#endif
