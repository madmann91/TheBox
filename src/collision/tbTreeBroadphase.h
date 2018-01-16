/************************************************************************
*                                                                       *
*  File : tbBroadphase.h                                                *
*  Date : 27/10/10                                                      *
*  Comments : My implementation of the Sweep And Prune algorithm        *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_BROADPHASE_H__
#define __TB_BROADPHASE_H__

#include "tbCommon.h"
#include "tbObject.h"
#include "tbTree.h"

/*! \addtogroup collision */
/*! @{ */

/*! \name Broadphase */
/*! @{ */

/*! Broadphase pair collision callback */
typedef void (*tbBroadphaseCallback)(void*,tbObject*,tbObject*);

/*! Broadphase object */
typedef struct tbBroadphase_s
{			
	tbTree* tree;
	tbHeuristic heuristic;
	int updatesCount;
}tbBroadphase;

/*!
  \brief Creates a broadphase
  \return A valid broadphase object, or 0 if an error occured
*/
tbBroadphase* tbCreateBroadphase(void);
/*!
  \brief Updates the broadphase
  \param[in] broadphase The broadphase to update
  \param[in] timeStep The elapsed time
  \param[in] objects The object array
  \param[in] numObjects The number of objects
*/
void tbSetObjects(tbBroadphase* broadphase,tbScalar timeStep,tbObject* objects,unsigned int numObjects);
/*!
  \brief Tests all the objects for collision
  \param[in] broadphase The broadphase to test
  \param[in] callback The function to call when a collision is found
  \param[in] user The user data to pass to the callback function
  This function should be called after tbSetObjects. Otherwise, the behaviour is undetermined.
*/
void tbGetPairs(tbBroadphase* broadphase,tbBroadphaseCallback callback,void* user);
/*!
  \brief Destroys the broadphase
  \param[in] broadphase The broadphase to be destroyed
*/
void tbDestroyBroadphase(tbBroadphase* broadphase);

/*! @} */

/*! @} */

#endif
