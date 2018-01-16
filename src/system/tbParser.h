/************************************************************************
*                                                                       *
*  File : tbParser.h                                                    *
*  Date : 26/10/10                                                      *
*  Comments : Simulation file parser                                    *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_PARSER_H__
#define __TB_PARSER_H__

#include "tbCommon.h"
#include "tbSimulation.h"
#include "tbInterface.h"
#include "tbWindow.h"
#include "tbApplication.h"

/*! \addtogroup system */
/*! @{ */

/*! \name Parser */
/*! @{ */

#define TB_PARSER_ERROR 256
#define TB_PARSER_BUFFER 512
#define TB_PARSER_COMMAND 128

/*! Parser object */
typedef struct tbParser_s
{
	const char* fileName;				/*!< The name of the file to parse */
	char errorMessage[TB_PARSER_ERROR];	/*!< The error message, if any */
	unsigned int line;					/*!< Line where the parser stopped */
	unsigned int currentObject;			/*!< Current object index while parsing */
	tbApplication* application;			/*!< The generated application, if any */
}tbParser;

/*!
  \brief Creates a parser object
  \param fileName The file to parse
  \param application The application object to modify while parsing
  \return A valid parser object, or 0 if an error occured
*/
tbParser* tbCreateParser(const char* fileName,tbApplication* application);
/*!
  \brief Parses the file
  \param parser The previously created parser object
  \return 0 if an error occured, otherwise 1
*/
int tbParse(tbParser* parser);
/*!
  \brief Destroys the given parser object
  \param parser The parser to destroy
*/
void tbDestroyParser(tbParser* parser);

/*! @} */

/*! @} */

#endif 