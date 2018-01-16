#include "tbConfig.h"
#include "tbSimulation.h"
#include "tbCollision.h"
#include "tbMemory.h"

tbSimulation* tbCreateSimulation(unsigned int numObjects,tbBroadphase* broadphase)
{
	tbSimulation* simulation;

	simulation=tbAlloc(sizeof(tbSimulation));
	tbMemorySet(simulation,0,sizeof(tbSimulation));
	simulation->broadphase=broadphase;

	simulation->objects=tbAlloc(sizeof(tbObject)*numObjects);
	tbMemorySet(simulation->objects,0,sizeof(tbObject)*numObjects);
	simulation->numObjects=numObjects;

	/* Allocate some contacts */
	simulation->contacts=tbAlloc(sizeof(tbContact)*16);
	simulation->maxContacts=16;

	simulation->pairs=tbCreatePairs(numObjects/4+1);

	return simulation;
}

static void tbHandlePair(tbSimulation* simulation,tbScalar* timeStep,const tbPair* pair)
{
	tbObject* object1=&simulation->objects[pair->first];
	tbObject* object2=&simulation->objects[pair->second];
	tbCollision info;
	
	if(tbTest(object1,object2,*timeStep,&info))
	{
		unsigned int i;

		/* Make sure we don't keep contacts above the time threshold */
		if(info.collisionTime>*timeStep+TB_SIMULATION_THRESHOLD)
			return;

		/* Change maximum time step if needed */
		if(info.collisionTime<*timeStep-TB_SIMULATION_THRESHOLD)
		{
			*timeStep=info.collisionTime;
			simulation->numContacts=0;
		}

		/* Reallocate contacts array */
		if(info.numPoints+simulation->numContacts>=simulation->maxContacts)
		{
			simulation->maxContacts+=info.numPoints;
			simulation->contacts=tbRealloc(simulation->contacts,sizeof(tbContact)*simulation->maxContacts);
		}

		/* Build contact points */
		for(i=0;i<info.numPoints;i++)
		{
			tbContact* contact=&simulation->contacts[simulation->numContacts];

			contact->first=object1;
			contact->second=object2;

			contact->normal[0]=info.normal[0];
			contact->normal[1]=info.normal[1];

			contact->point[0]=info.points[i][0];
			contact->point[1]=info.points[i][1];

			simulation->numContacts++;
		}
	}
}

static int tbIsSleeping(tbObject* object)
{
	if(!(object->flags&TB_OBJECT_STATIC))
	{
		if(tbSquareLength(object->state.velocity)<=TB_SIMULATION_SLEEP)
			return 1;
	}
	return 0;
}

void tbUpdateSimulation(tbSimulation* simulation,tbScalar timeStep)
{
	tbScalar timeLeft=timeStep,collisionTime;
	unsigned int i,zeroCount=0;

#ifdef TB_STATISTICS
	unsigned long ticks;
#endif

	if(simulation->flags&TB_SIMULATION_PAUSED)
		return;

#ifdef TB_STATISTICS
	simulation->frames++;
#endif

	while(timeLeft>TB_EPSILON)
	{
		collisionTime=timeLeft;
		simulation->numContacts=0;

#ifdef TB_STATISTICS
		ticks=tbGetTicks();
#endif
		/* Get potentially colliding pairs */
		tbResetPairs(simulation->pairs);
		tbSetObjects(simulation->broadphase,timeLeft,simulation->objects,simulation->numObjects);
		tbGetPairs(simulation->broadphase,simulation->pairs);

		for(i=0;i<simulation->pairs->numPairs;i++)
			tbHandlePair(simulation,&collisionTime,&simulation->pairs->pairs[i]);

#ifdef TB_STATISTICS
		simulation->broadphaseTicks+=tbGetTicks()-ticks;
#endif

		/* Integrate the objects until collision time is reached, and reset forces */
		for(i=0;i<simulation->numObjects;i++)
		{
			tbObject* object=&simulation->objects[i];
			tbIntegrate(object,&object->state,collisionTime);
			
			object->force[0]=0;
			object->force[1]=-simulation->gravity;
			object->torque=0;
			object->flags&=~TB_OBJECT_SLEEPING;
		}
	
#ifdef TB_STATISTICS
		ticks=tbGetTicks();
#endif
		/* Solve collisions */
		tbSolve(simulation->contacts,simulation->numContacts);

#ifdef TB_STATISTICS
		simulation->solverTicks+=tbGetTicks()-ticks;
#endif

		/* We prevent the simulation from freezing in some rare cases */
		timeLeft-=collisionTime;

		if(collisionTime<TB_EPSILON)
		{
			/* If the objects are interpenetrating too many times, we prevent the simulation from freezing */
			zeroCount++;
			if(zeroCount>=TB_SIMULATION_ZEROS)
			{
				/* Continue simulating even if objects are colliding */
				const tbScalar step=TB_SIMULATION_STEP*timeStep;

				/* Let them sleep a bit if they need to */
				for(i=0;i<simulation->numContacts;i++)
				{
					const tbContact* contact=&simulation->contacts[i];

					if(tbIsSleeping(contact->first))
						contact->first->flags|=TB_OBJECT_SLEEPING;
					if(tbIsSleeping(contact->second))
						contact->second->flags|=TB_OBJECT_SLEEPING;
				}

				for(i=0;i<simulation->numObjects;i++)
					tbIntegrate(&simulation->objects[i],&simulation->objects[i].state,step);

				timeLeft-=step;
				zeroCount=0;
			}
		}
		else
			zeroCount=0;
	}
}

void tbDestroySimulation(tbSimulation* simulation)
{
	tbDestroyPairs(simulation->pairs);
	tbFree(simulation->contacts);
	tbFree(simulation->objects);
	tbFree(simulation);
}
