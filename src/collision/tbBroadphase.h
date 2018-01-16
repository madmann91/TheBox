/************************************************************************
*                                                                       *
*  File : tbBroadphase.h                                                *
*  Date : 27/10/10                                                      *
*  Comments : My implementation of the Sweep And Prune algorithm        *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_BROADPHASE_H__
#define __TB_BROADPHASE_H__

#include "tbCommon.h"
#include "tbObject.h"
#include "tbPairs.h"

/*! \addtogroup collision */
/*! @{ */

/*! \name Broadphase */
/*! @{ */

#define TB_BROADPHASE_AXIS 0x00000001
#define TB_BROADPHASE_INDEX 0x7FFFFFFF
#define TB_BROADPHASE_POINT 0x80000000

/*! The broadphase element structure */
typedef struct tbBroadphaseElement_s
{
	int data;			/*!< Object index + Start/End flag */
	tbScalar value;		/*!< Sorting value */
}tbBroadphaseElement;

/*! The broadphase bounding box structure */
typedef struct tbBroadphaseBox_s
{
	tbVector min;		/*!< Minimum box coordinates */
	tbVector max;		/*!< Maximum box coordinates */
}tbBroadphaseBox;

/*! Broadphase object */
typedef struct tbBroadphase_s
{			
	unsigned int axis;					/*!< The sorting axis */

	tbBroadphaseElement* elements;		/*!< The elements array */
	unsigned int numElements;			/*!< The number of elements */

	tbBroadphaseBox* boundingBoxes;		/*!< The bounding boxes of the objects */
	tbObject* objects;					/*!< The objects array (not maintained by the broadphase) */
	unsigned int numObjects;			/*!< The number of objects in the array */
	
	unsigned int* active;				/*!< The active object buffer */
	unsigned int maxActive;				/*!< The size of the active buffer */
}tbBroadphase;

/*!
  \brief Creates a broadphase
  \param[in] axis The sorting axis
  \return A valid broadphase object, or 0 if an error occured
*/
tbBroadphase* tbCreateBroadphase(unsigned int axis);
/*!
  \brief Updates the broadphase elements and bounding boxes, and sort the elements.
  \param[in] broadphase The broadphase to update
  \param[in] timeStep The elapsed time
  \param[in] objects The object array
  \param[in] numObjects The number of objects
*/
void tbSetObjects(tbBroadphase* broadphase,tbScalar timeStep,tbObject* objects,unsigned int numObjects);
/*!
  \brief Tests all the objects for collision
  \param[in] broadphase The broadphase to test
  \param[in] table The pair table which is to receive pairs
  This function should be called after tbSetObjects. Otherwise, the behaviour is undetermined.
*/
void tbGetPairs(tbBroadphase* broadphase,tbPairTable* table);
/*!
  \brief Finds the best axis for the broadphase
  \param[in] broadphase The broadphase whose axis is to be changed
  This function should be called after tbSetObjects. Otherwise, the behaviour is undetermined.
  You shouldn't call this between tbSetObjects and tbGetPairs, otherwise tbGetPairs may use the wrong axis.
  This function may not be used every simulation frame, since it slows down computations (Even if it runs in O(numObjects) time).
*/
void tbFindBestAxis(tbBroadphase* broadphase);
/*!
  \brief Destroys the broadphase
  \param[in] broadphase The broadphase to be destroyed
*/
void tbDestroyBroadphase(tbBroadphase* broadphase);

/*! @} */

/*! @} */

#endif
