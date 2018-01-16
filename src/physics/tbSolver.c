#include "tbConfig.h"
#include "tbSolver.h"
#include "tbMemory.h"

static void tbApplyImpulse(tbObject*,tbScalar,const tbVector,const tbVector);
static void tbApplyForce(tbObject*,tbScalar,const tbVector,const tbVector);
static void tbPointVelocity(const tbVector,const tbObject*,tbVector);
static int tbSolveContact(const tbContact*);

static void tbApplyImpulse(tbObject* object,tbScalar j,const tbVector point,const tbVector normal)
{
	const tbVector impulse={j*normal[0],j*normal[1]};
	object->state.velocity[0]+=impulse[0]*object->inverseMass;
	object->state.velocity[1]+=impulse[1]*object->inverseMass;
	object->state.angularVelocity+=(point[0]*impulse[1]-point[1]*impulse[0])*object->inverseInertia;
}

static void tbApplyForce(tbObject* object,tbScalar j,const tbVector point,const tbVector normal)
{
	const tbVector force={j*normal[0],j*normal[1]};
	object->force[0]+=force[0];
	object->force[1]+=force[1];
	object->torque+=point[0]*force[1]-point[1]*force[0];
}

static void tbPointVelocity(const tbVector point,const tbObject* object,tbVector velocity)
{
	velocity[0]=object->state.velocity[0]-point[1]*object->state.angularVelocity;
	velocity[1]=object->state.velocity[1]+point[0]*object->state.angularVelocity;
}

static int tbSolveContact(const tbContact* contact)
{
	const tbVector firstPoint={contact->point[0]-contact->first->state.position[0],
				contact->point[1]-contact->first->state.position[1]};
	const tbVector secondPoint={contact->point[0]-contact->second->state.position[0],
				contact->point[1]-contact->second->state.position[1]};
	const tbScalar restitution=contact->first->shape->restitution*contact->second->shape->restitution;
	tbScalar relativeVelocity,firstJ,secondJ,det,j;
	tbVector firstVelocity,secondVelocity,perp;
	
	/* Get the colliding points speed */
	tbPointVelocity(firstPoint,contact->first,firstVelocity);
	tbPointVelocity(secondPoint,contact->second,secondVelocity);

	relativeVelocity=contact->normal[0]*(firstVelocity[0]-secondVelocity[0])+
		contact->normal[1]*(firstVelocity[1]-secondVelocity[1]);

	/* Apply friction (directed against velocity) */
	tbPerp(contact->normal,perp);
	tbApplyForce(contact->first,-tbDot(perp,firstVelocity)*contact->first->shape->friction,firstPoint,perp);
	tbApplyForce(contact->second,-tbDot(perp,secondVelocity)*contact->second->shape->friction,secondPoint,perp);

	/* Check if the objects are going towards each other, and exit if not */
	if(relativeVelocity<-TB_SOLVER_THRESHOLD)
		return 0;

	/* Compute the impulse (see D. Baraff's article) */
	det=(firstPoint[0]*contact->normal[1]-firstPoint[1]*contact->normal[0])*contact->first->inverseInertia;
	firstJ=(firstPoint[0]*det)*contact->normal[1]-(firstPoint[1]*det)*contact->normal[0];

	det=(secondPoint[0]*contact->normal[1]-secondPoint[1]*contact->normal[0])*contact->second->inverseInertia;
	secondJ=(secondPoint[0]*det)*contact->normal[1]-(secondPoint[1]*det)*contact->normal[0];

	j=(-(1+restitution)*relativeVelocity)/
		(contact->first->inverseMass+contact->second->inverseMass+firstJ+secondJ);

	tbApplyImpulse(contact->first,j,firstPoint,contact->normal);
	tbApplyImpulse(contact->second,-j,secondPoint,contact->normal);

	return 1;
}

void tbSolve(const tbContact* contacts,unsigned int numContacts)
{
	unsigned int i,j,count;

	j=0;
	do
	{
		j++;
		count=0;
		for(i=0;i<numContacts;i++)
			count+=tbSolveContact(&contacts[i]);
	}
	while(count>0&&j<TB_SOLVER_ITERATIONS);
}
