#include "tbConfig.h"
#include "tbMemory.h"

#ifndef DEBUG
void* tbAlloc(unsigned int size)
{
	return malloc(size);
}

void* tbRealloc(void* ptr,unsigned int size)
{
	return realloc(ptr,size);
}

void tbFree(void* ptr)
{
	free(ptr);
}
#else
unsigned long tbMemoryUsed=0;
unsigned long tbMemoryFreed=0;

void* tbAlloc(unsigned int size)
{
	void* ptr=malloc(size);
	tbMemoryUsed+=_msize(ptr);
	return ptr;
}

void* tbRealloc(void* ptr,unsigned int size)
{
	void* nptr;
	if(ptr)
		tbMemoryUsed-=_msize(ptr);
	nptr=realloc(ptr,size);
	tbMemoryUsed+=_msize(nptr);
	return nptr;
}

void tbFree(void* ptr)
{
	tbMemoryFreed+=_msize(ptr);
	free(ptr);
}
#endif

void tbMemorySet(void* ptr,int value,unsigned int size)
{
	memset(ptr,value,size);
}

