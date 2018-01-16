#include "tbConfig.h"
#include "tbShapes.h"
#include "tbMemory.h"

static void tbBoxBoundingBox(const tbShape*,const tbVector,tbVector,tbVector);
static void tbCircleBoundingBox(const tbShape*,const tbVector,tbVector,tbVector);

typedef void (*tbBoundingBoxCallback)(const tbShape*,const tbVector,tbVector,tbVector);
static const tbBoundingBoxCallback tbBoundingBoxTable[TB_SHAPE_MAX]={tbBoxBoundingBox,tbCircleBoundingBox};

tbShape* tbCreateBox(tbVector halfExtents)
{
	tbShape* shape;
	tbBox* box;

	shape=tbAlloc(sizeof(tbShape)+sizeof(tbBox));
	box=(tbBox*)(shape+1);
	shape->type=TB_SHAPE_BOX;
	shape->inertia=(tbScalar)(1.0/3.0)*(halfExtents[0]*halfExtents[0]+halfExtents[1]*halfExtents[1]);
	shape->friction=(tbScalar)1.0;
	shape->restitution=(tbScalar)1.0;
	
	box->halfExtents[0]=halfExtents[0];
	box->halfExtents[1]=halfExtents[1];

	return shape;
}

tbShape* tbCreateCircle(tbScalar radius)
{
	tbShape* shape;
	tbCircle* circle;

	shape=tbAlloc(sizeof(tbShape)+sizeof(tbCircle));
	circle=(tbCircle*)(shape+1);
	shape->type=TB_SHAPE_CIRCLE;
	shape->inertia=(tbScalar)(0.5)*radius*radius;
	shape->friction=(tbScalar)1.0;
	shape->restitution=(tbScalar)1.0;
	
	circle->radius=radius;

	return shape;
}

tbScalar tbGetRadius(const tbShape* shape)
{
	const tbCircle* circle=(tbCircle*)(shape+1);
	tbAssert(shape->type==TB_SHAPE_CIRCLE);
	return circle->radius;
}

void tbGetHalfExtents(const tbShape* shape,tbVector halfExtents)
{
	const tbBox* box=(tbBox*)(shape+1);
	tbAssert(shape->type==TB_SHAPE_BOX);
	halfExtents[0]=box->halfExtents[0];
	halfExtents[1]=box->halfExtents[1];
}

static void tbBoxBoundingBox(const tbShape* shape,const tbVector rotation,tbVector min,tbVector max)
{
	const tbBox* box=(tbBox*)(shape+1);
	const tbVector absRotation={tbAbs(rotation[0]),tbAbs(rotation[1])};

	max[0]=absRotation[0]*box->halfExtents[0]+absRotation[1]*box->halfExtents[1];
	max[1]=absRotation[0]*box->halfExtents[1]+absRotation[1]*box->halfExtents[0];
	min[0]=-max[0];
	min[1]=-max[1];
}

static void tbCircleBoundingBox(const tbShape* shape,const tbVector rotation,tbVector min,tbVector max)
{
	const tbCircle* circle=(tbCircle*)(shape+1);
	min[0]=-circle->radius;
	min[1]=-circle->radius;
	max[0]=circle->radius;
	max[1]=circle->radius;
}

void tbBoundingBox(const tbShape* shape,const tbVector rotation,tbVector min,tbVector max)
{
	tbBoundingBoxTable[shape->type](shape,rotation,min,max);
}

void tbDestroyShape(tbShape* shape)
{
	tbFree(shape);
}
