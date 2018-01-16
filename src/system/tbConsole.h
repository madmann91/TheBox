/************************************************************************
*                                                                       *
*  File : tbConsole.h                                                   *
*  Date : 26/10/10                                                      *
*  Comments : Console handling functions                                *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_CONSOLE_H__
#define __TB_CONSOLE_H__

#include "tbCommon.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Console */
/*! @{ */

/*!
  \brief Prints a line in the console prefixed by the current date and time
  \param format The formatting string (same as printf)
*/
void tbMessage(const char* format,...);
/*!
  \brief Prints a raw string in the console
  \param format The formatting string (same as printf)
*/
void tbPrint(const char* format,...);

/*! @} */

/*! @} */

#endif
