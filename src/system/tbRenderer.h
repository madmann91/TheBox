/************************************************************************
*                                                                       *
*  File : tbRenderer.h                                                  *
*  Date : 26/10/10                                                      *
*  Comments : Simulation rendering with OpenGL                          *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_RENDERER_H__
#define __TB_RENDERER_H__

#include "tbCommon.h"
#include "tbWindow.h"
#include "tbSimulation.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Renderer */
/*! @{ */

#define TB_RENDERER_CROSS ((tbScalar)0.2)	/*!< The size of the cross symbol */
#define TB_RENDERER_ARROW ((tbScalar)0.3)	/*!< The size of the arrow symbol */

/*! The renderer object */
typedef struct tbRenderer_s
{
	tbWindow* window;			/*!< The window to render in */

#ifdef TB_STATISTICS
	unsigned int frames;		/*!< The number of frames rendered */
	unsigned long ticks;		/*!< The number of accounted ticks */
#endif
	
	tbScalar crossBuffer[8];	/*!< The cross buffer */
	tbScalar* circleBuffer;		/*!< The circle vertex buffer */
	unsigned int numSegments;	/*!< The number of segments that make up a circle */
}tbRenderer;

/*!
  \brief Creates a renderer object
  \param[in] window The window object
  \param[in] quality The quality of rendering (between 0 and 100)
*/
tbRenderer* tbCreateRenderer(tbWindow* window,unsigned int quality);
/*!
  \brief Renders a simulation
  \param[in] renderer The renderer object to use
  \param[in] simulation The simulation to render
*/
void tbRender(tbRenderer* renderer,const tbSimulation* simulation);
/*!
  \brief Destroys a renderer object
  \param[in] renderer The renderer object to destroy
*/
void tbDestroyRenderer(tbRenderer* renderer);

/*! Rendering callback */
typedef void (*tbRenderCallback)(tbRenderer*,const tbObject*);

/*! Rendering function table */
extern const tbRenderCallback tbRenderingTable[TB_SHAPE_MAX];

/*! @} */

/*! @} */

#endif
