/************************************************************************
*                                                                       *
*  File : tbResources.h                                                 *
*  Date : 27/10/10                                                      *
*  Comments : Cursors and program icon                                  *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_RESOURCES_H__
#define __TB_RESOURCES_H__

#include "tbCommon.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Resources */
/*! @{ */

/*! The cursor types */
enum TB_CURSOR_TYPE
{
	TB_CURSOR_ARROW,		/*!< An arrow cursor */
	TB_CURSOR_HAND,			/*!< A hand-like cursor */
	TB_CURSOR_MOVE,			/*!< A cross-like cursor */
	TB_CURSOR_MAX
};

/*! The cursor object structure */
typedef struct tbCursor_s
{
	unsigned char image[32];		/*!< The cursor image */
	unsigned char mask[32];			/*!< The cursor mask (transparency) */
	char x,y;						/*!< Hot point position */
}tbCursor;

/*! The icon object structure */
typedef struct tbIcon_s
{
	unsigned char colors[4*16];		/*!< The icon palette */
	unsigned char image[32*32];		/*!< The icon image */
}tbIcon;

/*!
  \brief Gets a cursor object
  \param[in] type The type of cursor
  \return A valid cursor object or 0 if an error occured
  \sa TB_CURSOR_TYPES
*/
const tbCursor* tbGetCursor(unsigned int type);
/*!
  \brief Gets the program icon
  \return A valid icon object or 0 if an error occured
*/
const tbIcon* tbGetIcon(void);

/*! @} */

/*! @} */

#endif
