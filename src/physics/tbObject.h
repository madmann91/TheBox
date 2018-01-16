/************************************************************************
*                                                                       *
*  File : tbObject.h                                                    *
*  Date : 27/10/10                                                      *
*  Comments : Simulation objects structures and definitions             *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_OBJECT_H__
#define __TB_OBJECT_H__

#include "tbCommon.h"
#include "tbShapes.h"
#include "tbVector.h"

/*! \addtogroup physics */
/*! @{ */

/*! \name Object */
/*! @{ */

/*! Object flags */
enum TB_OBJECT_FLAGS
{
	TB_OBJECT_STATIC=0x01,			/*!< The object movement is not affected by a collision  */
	TB_OBJECT_SLEEPING=0x02			/*!< The object is sleeping (not affected by integration) */
};

/*!< The state of an object (ie the parameters modified by the integrator) */
typedef struct tbState_s
{
	tbVector position;			/*!< Object position */
	tbVector rotation;			/*!< Object rotation vector (cos(angle),sin(angle)) */
	tbScalar angle;				/*!< Object angle */

	tbVector velocity;			/*!< Object speed */
	tbScalar angularVelocity;	/*!< Angular velocity */
}tbState;

/*! Object structure */
typedef struct tbObject_s
{
	int flags;					/*!< Object flags, see TB_OBJECT_FLAGS */

	tbVector force;				/*!< Forces to apply to the object */
	tbScalar torque;			/*!< The torque to apply to the object */

	tbScalar inverseMass;		/*!< Object inverse mass */
	tbScalar inverseInertia;	/*!< The inverse inertia of the object */

	tbState state;				/*!< Object state */
	tbShape* shape;				/*!< Object shape */
}tbObject;

/*!
  \brief Sets the angle of an object state
  \param[in] state The state to modify
  \param[in] angle The new angle of the state, in radians
*/
void tbSetAngle(tbState* state,tbScalar angle);
/*!
  \brief Sets the mass of an object
  \param[in] object The object to modify
  \param[in] mass The new mass of the object, in kg
*/
void tbSetMass(tbObject* object,tbScalar mass);
/*!
  \brief Integrates an object state
  \param[in] object The object containing mass, force, and shape information
  \param[in,out] state The state to integrate
  \param[in] timeStep The elapsed time
*/
void tbIntegrate(const tbObject* object,tbState* state,tbScalar timeStep);

/*! @} */

/*! @} */

#endif
