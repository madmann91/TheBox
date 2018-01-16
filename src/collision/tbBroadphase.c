#include "tbConfig.h"
#include "tbBroadphase.h"
#include "tbMemory.h"

tbBroadphase* tbCreateBroadphase(unsigned int axis)
{
	tbBroadphase* broadphase;

	broadphase=tbAlloc(sizeof(tbBroadphase));
	tbMemorySet(broadphase,0,sizeof(tbBroadphase));
	broadphase->axis=axis;

	return broadphase;
}

void tbSetObjects(tbBroadphase* broadphase,tbScalar timeStep,tbObject* objects,unsigned int numObjects)
{
	const unsigned int numElements=numObjects*2;
	unsigned int i,j;

	if(numElements!=broadphase->numElements)
	{
		/* The object array has changed, we must recompute all the elements... */
		broadphase->elements=tbRealloc(broadphase->elements,sizeof(tbBroadphaseElement)*numElements);
		for(i=0;i<numObjects;i++)
		{
			tbBroadphaseElement* elements=&broadphase->elements[i*2];
					
			elements[0].data=i|TB_BROADPHASE_POINT;
			elements[1].data=i;
		}
		broadphase->numElements=numElements;
	}

	broadphase->objects=objects;

	if(broadphase->maxActive<numObjects)
	{
		/* We must resize the "active" list */
		broadphase->active=tbRealloc(broadphase->active,sizeof(unsigned int)*numObjects);
		broadphase->maxActive=numObjects;
	}
	
	if(broadphase->numObjects<numObjects)
	{
		/* We must resize the bounding box list */
		broadphase->boundingBoxes=tbRealloc(broadphase->boundingBoxes,sizeof(tbBroadphaseBox)*numObjects);
		broadphase->numObjects=numObjects;
	}

	/* Compute the bounding box of each object */
	for(i=0;i<numObjects;i++)
	{
		tbObject* object=&objects[i];
		tbBroadphaseBox* box=&broadphase->boundingBoxes[i];
		tbState state=object->state;

		/* We must integrate the objects so that we have their right bounding boxes at time t0+timeStep */
		tbIntegrate(object,&state,timeStep);
		tbBoundingBox(object->shape,state.rotation,box->min,box->max);

		/* Note that we need to translate the transformed bounding box */
		box->min[0]+=state.position[0];
		box->min[1]+=state.position[1];
		box->max[0]+=state.position[0];
		box->max[1]+=state.position[1];
	}

	/* Update the elements */
	for(i=0;i<broadphase->numElements;i++)
	{
		tbBroadphaseElement* element=&broadphase->elements[i];
		const unsigned int index=element->data&TB_BROADPHASE_INDEX;
		const tbBroadphaseBox* box=&broadphase->boundingBoxes[index];
		
		element->value=(element->data&TB_BROADPHASE_POINT)?box->min[broadphase->axis]:box->max[broadphase->axis];
	}

	/* Sort the elements using an insertion sort (cf D. Baraff's article : Rigid Body Simulation) */
	for(i=1;i<broadphase->numElements;i++)
	{
		tbBroadphaseElement current=broadphase->elements[i];
		for(j=i;j>0&&broadphase->elements[j-1].value>current.value;j--)
			broadphase->elements[j]=broadphase->elements[j-1];
		broadphase->elements[j]=current;
	}
}

void tbGetPairs(tbBroadphase* broadphase,tbPairTable* table)
{
	const unsigned int otherAxis=!broadphase->axis;
	unsigned int* active=broadphase->active;
	unsigned int numActive=0;
	unsigned int i,j;
	
	/* Prune the elements */
	for(i=0;i<broadphase->numElements;i++)
	{
		const tbBroadphaseElement* element=&broadphase->elements[i];
		const unsigned int index=element->data&TB_BROADPHASE_INDEX;

		if(element->data&TB_BROADPHASE_POINT) /* This is a minimum element */
		{
			tbObject* first=&broadphase->objects[index];
			tbBroadphaseBox* box1=&broadphase->boundingBoxes[index];
			int filter=first->flags&(TB_OBJECT_STATIC|TB_OBJECT_SLEEPING);

			/* This object might collide with all the active objects */
			for(j=0;j<numActive;j++)
			{
				tbBroadphaseBox* box2=&broadphase->boundingBoxes[active[j]];
				/* We test the other axis for intersection */
				if(box1->min[otherAxis]<box2->max[otherAxis]&&box1->max[otherAxis]>box2->min[otherAxis])
				{
					tbObject* second=&broadphase->objects[active[j]];
					tbPair pair;

					if(filter&&(second->flags&(TB_OBJECT_STATIC|TB_OBJECT_SLEEPING)))
						continue;

					if(active[j]>index)
					{
						pair.first=index;
						pair.second=active[j];
					}
					else
					{
						pair.first=active[j];
						pair.second=index;
					}
					
					tbAddPair(table,&pair);
				}
			}

			/* Set this object as active */
			active[numActive]=index;
			numActive++;
		}
		else /* This is a maximum element, remove it from the active list */
		{
			tbAssert(numActive>0);	/* This should never happen */

			for(j=0;j<numActive;j++)
			{
				if(active[j]==index)
				{
					numActive--;
					active[j]=active[numActive];	/* Move the last element into this position */
					break;
				}
			}
		}
	}
}

void tbFindBestAxis(tbBroadphase* broadphase)
{
	unsigned int i;
	tbVector variance={0,0};

	/* Compute the middle point of all the objects on both axes */
	for(i=0;i<broadphase->numObjects;i++)
	{
		tbBroadphaseBox* box=&broadphase->boundingBoxes[i];
		variance[0]+=tbAbs(box->min[0]);
		variance[0]+=tbAbs(box->max[0]);
		variance[1]+=tbAbs(box->min[1]);
		variance[1]+=tbAbs(box->max[1]);
	}

	/* Choose the axis with the greatest variance */
	if(variance[0]>=variance[1])
		broadphase->axis=0;
	else
		broadphase->axis=1;
}

void tbDestroyBroadphase(tbBroadphase* broadphase)
{
	if(broadphase->elements)
		tbFree(broadphase->elements);
	if(broadphase->active)
		tbFree(broadphase->active);
	if(broadphase->boundingBoxes)
		tbFree(broadphase->boundingBoxes);
	tbFree(broadphase);
}

