/************************************************************************
*                                                                       *
*  File : tbInterface.h                                                 *
*  Date : 26/10/10                                                      *
*  Comments : User interface functions                                  *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_INTERFACE_H__
#define __TB_INTERFACE_H__

#include "tbCommon.h"
#include "tbSimulation.h"
#include "tbWindow.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Interface */
/*! @{ */

/*! Interface states */
enum TB_INTERFACE_STATES
{
	TB_INTERFACE_NORMAL,		/*!< Normal state (no action) */
	TB_INTERFACE_GRAB,			/*!< The user is trying to grab an object */
	TB_INTERFACE_MOVE			/*!< The user is moving the view */
};

/*! Interface object */
typedef struct tbInterface_s
{
	tbWindow* window;			/*!< The renderer window to draw in */
	tbSimulation* simulation;	/*!< The simulation object */
	int state;					/*!< The interface state */
}tbInterface;

/*!
  \brief Creates an interface object
  \param[in] simulation The simulation object to use
  \param[in] window The associated renderer window
  \return A valid interface object, or 0 if an error occured
*/
tbInterface* tbCreateInterface(tbSimulation* simulation,tbWindow* window);
/*!
  \brief Updates an interface object
  \param[in] interface The interface object to update
  \return 0 if the application must exit, otherwise 1
*/
int tbUpdateInterface(tbInterface* interface);
/*!
  \brief Destroys the given interface object
  \param[in] interface The interface object to destroy
*/
void tbDestroyInterface(tbInterface* interface);

/*! @} */

/*! @} */

#endif
