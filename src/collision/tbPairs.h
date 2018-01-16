/************************************************************************
*                                                                       *
*  File : tbPairs.h                                                     *
*  Date : 27/10/10                                                      *
*  Comments : The pair manager for broadphase algorithms                *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_PAIRS_H__
#define __TB_PAIRS_H__

#include "tbCommon.h"

/*!
  \addtogroup collision 
  @{
  \name Pairs

  The pair table contains a hash table of indices, and the pair list. It works in a fairly simple way :
  first, a hash value is built from the pair, then the pair is added to the pair list, and the corresponding
  hash table index is set to the position of the pair in the list.

  @{
*/

/*! The growing factor for the pair table (must be greater than 1.0) */
#define TB_PAIRS_GROW ((tbScalar)1.2)

/*! A pair of objects (defined by their indices) */
typedef struct tbPair_s
{
	unsigned int first;			/*!< First object index */
	unsigned int second;		/*!< Second object index */
}tbPair;

/*! A chained list containing the indices in the table */
typedef struct tbIndexList_s
{
	int next;					/*!< The index of the next element in the list (or -1)*/
	unsigned int index;			/*!< The associated index value */
}tbIndexList;

/*! A pair hash table */
typedef struct tbPairTable_s
{
	int* indexTable;			/*!< The indices hash table */
	unsigned int tableSize;		/*!< The size of the index table */

	tbPair* pairs;				/*!< The pair list */
	tbIndexList* indices;		/*!< Preallocated indices (faster "add" operation) */
	unsigned int numPairs;		/*!< The number of pairs in the list */
	unsigned int maxPairs;		/*!< The size of the pair list */
}tbPairTable;

/*!
  \brief Sets up a table of pairs with the given number of entries
  \param[in] numEntries The number of entries in the table

  The parameter numEntries defines how many lists will be created to hold the hash table.
  When this number is too big, the "reset" operation is very slow. And when this number
  is too small, there is too many collisions. You must choose the right amount of entries
  according to the expected number of pairs (depends on the number of objects).
*/
tbPairTable* tbCreatePairs(unsigned int numEntries);
/*!
  \brief Adds a pair in the table, if it has not been added
  \param[in] table The pair table in which the pair is to be added
  \param[in] pair The pair to add in the table
*/
void tbAddPair(tbPairTable* table,const tbPair* pair);
/*!
  \brief Retreives a pair from the table, given its value
  \param[in] table The pair table in which the pair is supposed to be
  \param[in] pair The pair to look for in the table
  \return The index of the pair in the pair list, or -1 if it cannot be found.
*/
int tbGetPair(const tbPairTable* table,const tbPair* pair);
/*!
  \brief Resets the pair table, removing each existing pair from it
  \param[in] table The pair table to reset
*/
void tbResetPairs(tbPairTable* table);
/*!
  \brief Destroys the given pair table object
  \param[in] table The pair table to destroy
*/
void tbDestroyPairs(tbPairTable* table);

/*!
  @}
  @}
*/

#endif
