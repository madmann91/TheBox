/************************************************************************
*                                                                       *
*  File : tbApplication.h                                               *
*  Date : 26/10/10                                                      *
*  Comments : Structure containing all system objects                   *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_APPLICATION_H__
#define __TB_APPLICATION_H__

#include "tbCommon.h"
#include "tbBroadphase.h"
#include "tbSimulation.h"
#include "tbInterface.h"
#include "tbWindow.h"
#include "tbRenderer.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Application */
/*! @{ */

/*! Application object */
typedef struct tbApplication_s
{
	tbBroadphase* broadphase;			/*!< The broadphase object */
	tbSimulation* simulation;			/*!< The simulation object */
	tbInterface* interface;				/*!< The interface object */
	tbWindow* window;					/*!< The window object */
	tbRenderer* renderer;				/*!< The renderer object */

	tbScalar timeStep;					/*!< Simulation time step */
	tbShape** shapes;					/*!< Shape array */
	unsigned int numShapes;				/*!< The number of shapes in the array */
}tbApplication;

/*!
  \brief Destroys the given application object
  \param[in] application The application to destroy
*/
void tbDestroyApplication(tbApplication* application);

/*! @} */

/*! @} */

#endif 