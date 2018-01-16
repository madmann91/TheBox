/************************************************************************
*                                                                       *
*  File : tbCollision.h                                                 *
*  Date : 29/10/10                                                      *
*  Comments : Collision detection routines                              *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_COLLISION_H__
#define __TB_COLLISION_H__

#include "tbCommon.h"
#include "tbShapes.h"
#include "tbObject.h"
#include "tbVector.h"

/*!
  \addtogroup collision The collision detection module
  This module is designed to help detecting collision between objects.
  It also contains a broadphase system, which makes collision detection a lot faster.
*/
/*! @{ */

#define TB_COLLISION_POINTS 8

/*! Collision information */
typedef struct tbCollision_s
{
	tbScalar collisionTime;					/*!< Time of intersection */
	tbVector points[TB_COLLISION_POINTS];	/*!< Collision points */
	unsigned int numPoints;					/*!< Number of points */
	tbVector normal;						/*!< Collision normal (towards second object) */
}tbCollision;

/*! Collision callback (parameters : shape1,shape2,state1,state2,collisionInfo) */
typedef int (*tbCollisionCallback)(const tbShape*,const tbShape*,const tbState*,const tbState*,tbCollision*);

/*! Collision algorithm table */
extern const tbCollisionCallback tbAlgorithmTable[TB_SHAPE_MAX][TB_SHAPE_MAX];

/*!
  \brief Tests a couple of objects for collision
  \param[in] object1 The first object to test
  \param[in] object2 The second object
  \param[in] timeStep The elapsed time
  \param[in,out] info The collision information, if any
  \return 1 if a collision occured, otherwise 0
*/
int tbTest(const tbObject* object1,const tbObject* object2,tbScalar timeStep,tbCollision* info);

/*! @} */

#endif
