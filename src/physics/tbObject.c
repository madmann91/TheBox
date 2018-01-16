#include "tbConfig.h"
#include "tbObject.h"

void tbSetAngle(tbState* state,tbScalar angle)
{
	state->angle=angle;
	state->rotation[0]=tbCos(state->angle);
	state->rotation[1]=tbSin(state->angle);
}

void tbSetMass(tbObject* object,tbScalar mass)
{
	object->inverseMass=(tbScalar)1.0/mass;
	object->inverseInertia=object->inverseMass/object->shape->inertia;
}

void tbIntegrate(const tbObject* object,tbState* state,tbScalar timeStep)
{
	/* Simple euler integration : value=value+dvalue_dt*dt */
	if(!(object->flags&TB_OBJECT_SLEEPING))
	{
		state->position[0]+=state->velocity[0]*timeStep;
		state->position[1]+=state->velocity[1]*timeStep;
		state->angle+=state->angularVelocity*timeStep;

		/* Compute rotation vector (used by collision detection routines) */
		state->rotation[0]=tbCos(state->angle);
		state->rotation[1]=tbSin(state->angle);

		if(!(object->flags&TB_OBJECT_STATIC))
		{
			const tbScalar factor=timeStep*object->inverseMass;

			state->velocity[0]+=object->force[0]*factor;
			state->velocity[1]+=object->force[1]*factor;	
			state->angularVelocity+=object->torque*object->inverseInertia*timeStep;
		}
	}	
}
