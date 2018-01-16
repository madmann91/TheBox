/************************************************************************
*                                                                       *
*  File : tbTree.h                                                      *
*  Date : 27/10/10                                                      *
*  Comments : A very special kind of adapting tree... Hope it works !   *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_TREE_H__
#define __TB_TREE_H__

#include "tbCommon.h"
#include "tbVector.h"
#include "tbObject.h"

/*! \addtogroup collision */
/*! @{ */

/*! \name Tree */
/*! @{ */

/*! An object bounding box */
typedef struct tbObjectBox_s
{
	tbVector min;		/*!< Minimum dimensions of the box */
	tbVector max;		/*!< Maximum dimensions of the box */
}tbObjectBox;

/*! Heuristic data structure */
typedef struct tbHeuristic_s
{
	unsigned int minObjects;	/*!< Minimum allowed number of objects per node */
	unsigned int maxObjects;	/*!< Maximum allowed number of objects per leaf */
	unsigned int maxDepth;		/*!< Maximum tree depth */
	unsigned int minDepth;		/*!< Minimum tree depth */
	tbScalar minRatio;			/*!< Minimum allowed left/right ratio */
	tbScalar maxRatio;			/*!< Maximum allowed left/right ratio */
}tbHeuristic;

/*! A tree node object */
typedef struct tbNode_s
{
	char axis;					/*!< Splitting axis */
	tbScalar split;				/*!< Splitting value on this axis */
	unsigned int depth;			/*!< This node's depth */

	tbVector middle;			/*!< The middle point of all the bounding boxes in this node */
	tbVector variance;			/*!< The variance of all the bounding boxes  */

	unsigned int* objects;		/*!< The objects array if this node is a leaf */
	unsigned int numObjects;	/*!< The number of objects in this node */
	unsigned int maxObjects;	/*!< The size of the objects array */

	struct tbNode_s* children;	/*!< Pointer to the children, if any */
}tbNode;

/*! A tree structure */
typedef struct tbTree_s
{
	tbObject* objects;				/*!< The array of objects */
	tbObjectBox* boundingBoxes;		/*!< The array of bounding boxes */
	unsigned int numObjects;		/*!< The number of objects */
	tbNode* root;					/*!< The root node */
}tbTree;

/*!
  \brief Builds a node
  \param[in] node The parent node
  \param[in] tree The tree information
  \param[in] heuristic The heuristic information
  \return The depth of the tree
*/
unsigned int tbBuildNode(tbNode* node,const tbTree* tree,const tbHeuristic* heuristic);
/*!
  \brief Place an object in the tree
  \param[in] node The parent node
  \param[in] object The object index
  \param[in] tree The tree information
  The objects count must be set to zero before trying to place the objects in the tree.
*/
void tbPlaceObject(tbNode* node,unsigned int object,const tbTree* tree);
/*!
  \brief Updates the given node and its children
  \param[in] node The parent node
  \param[in] tree The tree information
  \param[in] heuristic Heuristic information
*/
void tbUpdateNode(tbNode* node,const tbTree* tree,const tbHeuristic* heuristic);
/*
  \brief Prepares a node and its children
  \param[in] node The parent node
  You must call this function after tbUpdateNode, otherwise, the behaviour is undetermined.
*/
void tbResetNode(tbNode* node);
/*!
  \brief Destroys a node
  \param[in] node The parent node
*/
void tbDestroyNode(tbNode* node);

/*! @} */

/*! @} */

#endif
