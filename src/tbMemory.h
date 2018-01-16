/************************************************************************
*                                                                       *
*  File : tbMemory.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Memory handling                                           *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_MEMORY_H__
#define __TB_MEMORY_H__

/*!
  \addtogroup memory Memory handling
  This module offers a low-level access to memory.
  If you want to use something else than malloc/free,
  you can change the routines that allocates and releases memory blocks.
*/
/*! @{ */

/*!
  \brief Allocates memory
  \param[in] size The memory block size, in bytes
  \return A valid memory block, or 0 if there's not enough memory
*/
void* tbAlloc(unsigned int size);
/*!
  \brief Reallocates memory
  \param[in] ptr The initial memory block to resize
  \param[in] size The memory size, in bytes
  \return A valid memory block, or 0 if there's not enough memory
*/
void* tbRealloc(void* ptr,unsigned int size);
/*!
  \brief Releases memory
  \param[in] ptr The memory block to release
*/
void tbFree(void* ptr);
/*!
  \brief Sets a memory block to the specified value
  \param[in] ptr The memory block to set
  \param[in] value The value to use
  \param[in] size The size of the memory block
*/
void tbMemorySet(void* ptr,int value,unsigned int size);

#ifdef DEBUG
extern unsigned long tbMemoryUsed;
extern unsigned long tbMemoryFreed;
#endif

/*! @} */

#endif 