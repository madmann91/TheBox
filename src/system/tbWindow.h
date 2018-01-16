/************************************************************************
*                                                                       *
*  File : tbWindow.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Window handling                                           *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_WINDOW_H__
#define __TB_WINDOW_H__

#include "tbConfig.h"
#include "tbCommon.h"
#include "tbResources.h"
#include "tbMath.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Window */
/*! @{ */

#define TB_WINDOW_MAXWIDTH "2048"
#define TB_WINDOW_MAXHEIGHT "1024"

/*! Renderer window object */
typedef struct tbWindow_s
{
	unsigned int width;					/*!< The width of the wndow */
	unsigned int height;				/*!< The height of the window */
	unsigned int bitsPerPixel;			/*!< The number of bits per pixel */

	tbVector center;					/*!< Center of view */
	tbScalar scale;						/*!< Scaling factor */

	SDL_Cursor* cursors[TB_CURSOR_MAX];	/*!< SDL cursor objects */
	SDL_Surface* icon;					/*!< SDL icon object */
}tbWindow;

/*!
  \brief Creates a render window
  \param[in] width The window width
  \param[in] height The window height
  \param[in] bitsPerPixel The number of bits per pixel
  \return A valid window object, or 0 if an error occured
*/
tbWindow* tbCreateWindow(unsigned int width,unsigned int height,unsigned int bitsPerPixel);
/*!
  \brief Sets the current cursor
  \param[in] window The window object
  \param[in] type The type of cursor
  \sa TB_CURSOR_TYPES
*/
void tbSetCursor(tbWindow* window,unsigned int type);
/*!
  \brief Sets the current viewport
  \param[in] window The window object
  \param[in] center The point which will be in the center of the view
  \param[in] scale The scaling factor
*/
void tbSetView(tbWindow* window,const tbVector center,tbScalar scale);
/*!
  \brief Destroys a window object
  \param[in] window The window to destroy
*/
void tbDestroyWindow(tbWindow* window);

/*! @} */

/*! @} */

#endif
