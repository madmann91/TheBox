/************************************************************************
*                                                                       *
*  File : tbSolver.h                                                    *
*  Date : 27/10/10                                                      *
*  Comments : Collision solving                                         *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_SOLVER_H__
#define __TB_SOLVER_H__

#include "tbCommon.h"
#include "tbObject.h"
#include "tbCollision.h"

/*! \addtogroup physics */
/*! @{ */

/*! \name Solver */
/*! @{ */

#define TB_SOLVER_THRESHOLD ((tbScalar)0.000001)	/*!< The threshold under which contacts are considered as non-colliding */
#define TB_SOLVER_ITERATIONS 20						/*!< The maximum solver iteration count */

/*! Collision contact structure */
typedef struct tbContact_s
{
	tbObject* first;		/*!< The first object in contact */
	tbObject* second;		/*!< The second object in contact */

	tbVector point;			/*!< The contact point */
	tbVector normal;		/*!< The contact normal (towards A) */
}tbContact;

/*!
  \brief Solve the given contacts
  \param[in] contacts The contacts array
  \param[in] numContacts The number of contacts
*/
void tbSolve(const tbContact* contacts,unsigned int numContacts);

/*! @} */

/*! @} */

#endif
