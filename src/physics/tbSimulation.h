/************************************************************************
*                                                                       *
*  File : tbSimulation.h                                                *
*  Date : 26/10/10                                                      *
*  Comments : Physic simulation handling                                *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_SIMULATION_H__
#define __TB_SIMULATION_H__

#include "tbCommon.h"
#include "tbBroadphase.h"
#include "tbSolver.h"
#include "tbObject.h"

/*! \addtogroup physics */
/*! @{ */

/*! \name Simulation */
/*! @{ */

#define TB_SIMULATION_THRESHOLD ((tbScalar)0.0001)		/*!< The time threshold under which contacts are considered as happening simultaneously */
#define TB_SIMULATION_ZEROS 2							/*!< The maximum number of consecutive zero-timeStep iterations */
#define TB_SIMULATION_STEP ((tbScalar)0.1)				/*!< The integration step ratio when too many zero-timeSteps occured */
#define TB_SIMULATION_SLEEP ((tbScalar)0.000001)		/*!< The velocity threshold under whiwh objects are put to sleep */

/*! The simulation flags */
enum TB_SIMULATION_FLAGS
{
	TB_SIMULATION_PAUSED=0x01		/*!< The simulation is paused */
};

/*! Simulation object */
typedef struct tbSimulation_s
{
	tbBroadphase* broadphase;		/*!< The associated broadphase object */
	int flags;						/*!< The simulation flags */

#ifdef TB_STATISTICS
	unsigned long solverTicks;		/*!< The solver ticks count */
	unsigned long broadphaseTicks;	/*!< The broadphase ticks number */
	unsigned int frames;			/*!< The frame counter */
#endif

	tbObject* objects;				/*!< The objects array */
	unsigned int numObjects;		/*!< The number of objects */

	tbContact* contacts;			/*!< The collision contacts array */
	unsigned int numContacts;		/*!< The number of contacts in the array */
	unsigned int maxContacts;		/*!< The maximum number of contacts */

	tbPairTable* pairs;				/*!< The pair table */

	tbScalar gravity;				/*!< The world gravity */
}tbSimulation;

/*!
  \brief Creates a simulation object
  \param[in] numObjects The number of objects in the simulation
  \param[in] broadphase The broadphase to use to speed up collision detection
  \return A simulation object, or 0 if an error occured
*/
tbSimulation* tbCreateSimulation(unsigned int numObjects,tbBroadphase* broadphase);
/*!
  \brief Updates a simulation object
  \param[in] simulation The simulation to be updated
  \param[in] timeStep The elapsed time, in seconds
*/
void tbUpdateSimulation(tbSimulation* simulation,tbScalar timeStep);
/*!
  \brief Destroys a simulation object
  \param[in] simulation The simulation to destroy
*/
void tbDestroySimulation(tbSimulation* simulation);

/*! @} */

/*! @} */

#endif 