#include "tbConfig.h"
#include "tbAlgorithm.h"

const tbCollisionCallback tbAlgorithmTable[TB_SHAPE_MAX][TB_SHAPE_MAX]={{tbBoxToBox,tbBoxToCircle},{tbCircleToBox,tbCircleToCircle}};

#define TB_BOXEDGE_TEST(a,b,halfExtents) \
	newExtents=axis[0]*a+axis[1]*b; \
	if(distance>halfExtents+newExtents) \
		return 0;

#define TB_BOXEDGEMIN_TEST0(a,b,halfExtents) \
	newExtents=axis[0]*a+axis[1]*b; \
	axisDistance=halfExtents+newExtents-tbAbs(distance); \
	if(axisDistance<0) \
		return 0; \
	sign=(tbScalar)((distance>0)?1.0:-1.0); \
	minAxis=0; \
	minDistance=axisDistance;

#define TB_BOXEDGEMIN_TEST(i,a,b,halfExtents,left,right) \
	newExtents=axis[0]*a+axis[1]*b; \
	axisDistance=halfExtents+newExtents-tbAbs(distance); \
	if(axisDistance<0) \
		return 0; \
	if(minDistance>axisDistance) \
	{ \
		sign=(tbScalar)((distance>0)?left:right); \
		minDistance=axisDistance; \
		minAxis=i; \
	}

#define TB_BOXPOINT_TEST(halfExtents,position) \
	if(tbAbs(tbDot(boxAxes[minAxis],point))<=halfExtents[whichAxis]&& \
		tbAbs(tbDot(boxAxes[perpAxis],point))<=halfExtents[otherAxis]) \
	{ \
		info->points[info->numPoints][0]=point[0]+position[0]; \
		info->points[info->numPoints][1]=point[1]+position[1]; \
		info->numPoints++; \
	}

int tbBoxToBox(const tbShape* shape1,const tbShape* shape2,const tbState* state1,const tbState* state2,tbCollision* info)
{
	/* We use the SAT (Separating axis theorem) to determine if a collision occured */
	const tbBox* box1=(tbBox*)(shape1+1);
	const tbBox* box2=(tbBox*)(shape2+1);
	/* Compute relative position and orientation */
	const tbVector position={state2->position[0]-state1->position[0],
			state2->position[1]-state1->position[1]};
	const tbScalar cosBox1=state1->rotation[0],sinBox1=state1->rotation[1];
	const tbScalar cosBox2=state2->rotation[0],sinBox2=state2->rotation[1];
	/* Just a trick to compute the cosinus and sinus of state2->angle-state1->angle */
	const tbVector axis={tbAbs(cosBox1*cosBox2+sinBox1*sinBox2),tbAbs(sinBox2*cosBox1-sinBox1*cosBox2)};
	tbScalar distance,newExtents;

	/* Do faster tests when no information is needed */
	if(!info)
	{
		/* Test the four axes */
		distance=tbAbs(position[0]*cosBox1+position[1]*sinBox1);
		TB_BOXEDGE_TEST(box2->halfExtents[0],box2->halfExtents[1],box1->halfExtents[0]);

		distance=tbAbs(-position[0]*sinBox1+position[1]*cosBox1);
		TB_BOXEDGE_TEST(box2->halfExtents[1],box2->halfExtents[0],box1->halfExtents[1]);

		distance=tbAbs(-position[0]*cosBox2-position[1]*sinBox2);
		TB_BOXEDGE_TEST(box1->halfExtents[0],box1->halfExtents[1],box2->halfExtents[0]);

		distance=tbAbs(position[0]*sinBox2-position[1]*cosBox2);
		TB_BOXEDGE_TEST(box1->halfExtents[1],box1->halfExtents[0],box2->halfExtents[1]);

		return 1;
	}
	else
	{
		const tbVector boxAxes[4]={{cosBox1,sinBox1},{-sinBox1,cosBox1},
					{cosBox2,sinBox2},{-sinBox2,cosBox2}};
		char minAxis,perpAxis,whichAxis,otherAxis;
		tbScalar axisDistance,minDistance,sign;
		tbScalar sinAndCos[4];
		tbVector point;

		/* Do each test and compute minimum distance */
		distance=position[0]*cosBox1+position[1]*sinBox1;
		TB_BOXEDGEMIN_TEST0(box2->halfExtents[0],box2->halfExtents[1],box1->halfExtents[0]);

		distance=-position[0]*sinBox1+position[1]*cosBox1;
		TB_BOXEDGEMIN_TEST(1,box2->halfExtents[1],box2->halfExtents[0],box1->halfExtents[1],(tbScalar)1.0,(tbScalar)-1.0);

		distance=-position[0]*cosBox2-position[1]*sinBox2;
		TB_BOXEDGEMIN_TEST(2,box1->halfExtents[0],box1->halfExtents[1],box2->halfExtents[0],(tbScalar)-1.0,(tbScalar)1.0);

		distance=position[0]*sinBox2-position[1]*cosBox2;
		TB_BOXEDGEMIN_TEST(3,box1->halfExtents[1],box1->halfExtents[0],box2->halfExtents[1],(tbScalar)-1.0,(tbScalar)1.0);

		/* Find which box and which axis are to be considered */
		whichAxis=minAxis%2;
		perpAxis=(!(whichAxis))+2*(minAxis/2);

		info->normal[0]=sign*boxAxes[minAxis][0];
		info->normal[1]=sign*boxAxes[minAxis][1];

		/* Find collision point(s) */
		info->numPoints=0;
		otherAxis=!whichAxis;

		/* Test the first box points */
		sinAndCos[0]=cosBox1*box1->halfExtents[0];
		sinAndCos[1]=sinBox1*box1->halfExtents[1];
		sinAndCos[2]=sinBox1*box1->halfExtents[0];
		sinAndCos[3]=cosBox1*box1->halfExtents[1];

		/* Top right */
		point[0]=-position[0]+sinAndCos[0]-sinAndCos[1];
		point[1]=-position[1]+sinAndCos[2]+sinAndCos[3];
		TB_BOXPOINT_TEST(box2->halfExtents,state2->position);

		/* Top left */
		point[0]=-position[0]-sinAndCos[0]-sinAndCos[1];
		point[1]=-position[1]-sinAndCos[2]+sinAndCos[3];
		TB_BOXPOINT_TEST(box2->halfExtents,state2->position);

		/* Bottom left */
		point[0]=-position[0]-sinAndCos[0]+sinAndCos[1];
		point[1]=-position[1]-sinAndCos[2]-sinAndCos[3];
		TB_BOXPOINT_TEST(box2->halfExtents,state2->position);

		/* Bottom right */
		point[0]=-position[0]+sinAndCos[0]+sinAndCos[1];
		point[1]=-position[1]+sinAndCos[2]-sinAndCos[3];
		TB_BOXPOINT_TEST(box2->halfExtents,state2->position);
		
		/* Second box points */
		sinAndCos[0]=cosBox2*box2->halfExtents[0];
		sinAndCos[1]=sinBox2*box2->halfExtents[1];
		sinAndCos[2]=sinBox2*box2->halfExtents[0];
		sinAndCos[3]=cosBox2*box2->halfExtents[1];

		/* Top right */
		point[0]=position[0]+sinAndCos[0]-sinAndCos[1];
		point[1]=position[1]+sinAndCos[2]+sinAndCos[3];
		TB_BOXPOINT_TEST(box1->halfExtents,state1->position);

		/* Top left */
		point[0]=position[0]-sinAndCos[0]-sinAndCos[1];
		point[1]=position[1]-sinAndCos[2]+sinAndCos[3];
		TB_BOXPOINT_TEST(box1->halfExtents,state1->position);

		/* We only need to find 2 points at most for this box */
		
		/* Bottom left */
		point[0]=position[0]-sinAndCos[0]+sinAndCos[1];
		point[1]=position[1]-sinAndCos[2]-sinAndCos[3];
		TB_BOXPOINT_TEST(box1->halfExtents,state1->position);

		/* Bottom right */
		point[0]=position[0]+sinAndCos[0]+sinAndCos[1];
		point[1]=position[1]+sinAndCos[2]-sinAndCos[3];
		TB_BOXPOINT_TEST(box1->halfExtents,state1->position);

		return 1;
	}
}

#define TB_BOXCORNER_TEST(distance1,distance2,a,b) \
	if(distance1>0&&distance2>0) \
	{ \
		if(distance1*distance1+distance2*distance2<=squareRadius) \
		{ \
			if(info) \
			{ \
				info->numPoints=1; \
				\
				info->points[0][0]=state1->position[0]+a*cosBox-(b*sinBox); \
				info->points[0][1]=state1->position[1]+a*sinBox+b*cosBox; \
				\
				info->normal[0]=state2->position[0]-info->points[0][0]; \
				info->normal[1]=state2->position[1]-info->points[0][1]; \
				\
				tbScale(info->normal,(tbScalar)1.0/tbLength(info->normal)); \
			} \
			return 1; \
		} \
		else \
			return 0; \
	}

int tbBoxToCircle(const tbShape* shape1,const tbShape* shape2,const tbState* state1,const tbState* state2,tbCollision* info)
{
	const tbBox* box=(tbBox*)(shape1+1);
	const tbCircle* circle=(tbCircle*)(shape2+1);
	const tbScalar cosBox=state1->rotation[0],sinBox=state1->rotation[1];
	/* Find the circle coordinates along new box axes */
	const tbVector center={state2->position[0]-state1->position[0],
					state2->position[1]-state1->position[1]};
	const tbVector circlePosition={center[0]*cosBox+center[1]*sinBox,
					-center[0]*sinBox+center[1]*cosBox};
	/* Computes box-circle distances */
	const tbScalar distances[4]={circlePosition[0]-box->halfExtents[0],
								-circlePosition[0]-box->halfExtents[0],
								circlePosition[1]-box->halfExtents[1],
								-circlePosition[1]-box->halfExtents[1]};
	tbScalar squareRadius;

	/* Two tests... better than four, isn't it ? */
	if((distances[0]-circle->radius)*(distances[1]-circle->radius)<0)
		return 0;

	if((distances[2]-circle->radius)*(distances[3]-circle->radius)<0)
		return 0;

	/* Test each box corner */
	squareRadius=circle->radius*circle->radius;
	TB_BOXCORNER_TEST(distances[0],distances[2],box->halfExtents[0],box->halfExtents[1]);
	TB_BOXCORNER_TEST(distances[0],distances[3],box->halfExtents[0],-box->halfExtents[1]);
	TB_BOXCORNER_TEST(distances[1],distances[2],-box->halfExtents[0],box->halfExtents[1]);
	TB_BOXCORNER_TEST(distances[1],distances[3],-box->halfExtents[0],-box->halfExtents[1]);
	
	if(info)
	{
		/* Here we are sure there must be a collision against one of the box edges */
		const tbVector axes[2]={{cosBox,sinBox},{-sinBox,cosBox}};
		tbScalar sign,min,value;
		char dir,i;

		info->numPoints=1;

		/* Find minimum distance */
		i=0;
		min=tbAbs(distances[0]-circle->radius);
		value=tbAbs(distances[1]-circle->radius);
		if(min>value)
		{
			i=1;
			min=value;
		}
		value=tbAbs(distances[2]-circle->radius);
		if(min>value)
		{
			i=2;
			min=value;
		}
		value=tbAbs(distances[3]-circle->radius);
		if(min>value)
		{
			i=3;
			min=value;
		}

		/* Just a trick to avoid a test (when i is 0 or 2, sign is 1, otherwise, sign is -1) */
		sign=(tbScalar)(-2*(i%2)+1);
		dir=i/2;

		info->normal[0]=sign*axes[dir][0];
		info->normal[1]=sign*axes[dir][1];

		info->points[0][0]=state2->position[0]-info->normal[0]*circle->radius;
		info->points[0][1]=state2->position[1]-info->normal[1]*circle->radius;	
	}

	return 1;
}

int tbCircleToBox(const tbShape* shape1,const tbShape* shape2,const tbState* state1,const tbState* state2,tbCollision* info)
{
	int result=tbBoxToCircle(shape2,shape1,state2,state1,info);
	if(info&&result)
	{
		info->normal[0]=-info->normal[0];
		info->normal[1]=-info->normal[1];
	}
	return result;
}

int tbCircleToCircle(const tbShape* shape1,const tbShape* shape2,const tbState* state1,const tbState* state2,tbCollision* info)
{
	const tbCircle* circle1=(tbCircle*)(shape1+1);
	const tbCircle* circle2=(tbCircle*)(shape2+1);
	/* Compute the distance between the two circles */
	const tbVector position={state2->position[0]-state1->position[0],
			state2->position[1]-state1->position[1]};
	const tbScalar distance=tbLength(position);

	if(distance<=circle1->radius+circle2->radius)
	{
		if(info)
		{
			info->numPoints=1;

			/* The relative position vector should be normalized */
			info->normal[0]=position[0];
			info->normal[1]=position[1];
			tbScale(info->normal,(tbScalar)1.0/distance);

			info->points[0][0]=state2->position[0]-info->normal[0]*circle2->radius;
			info->points[0][1]=state2->position[1]-info->normal[1]*circle2->radius;
		}
		return 1;
	}
	return 0;
}

int tbTest(const tbObject* object1,const tbObject* object2,tbScalar timeStep,tbCollision* info)
{
	const tbCollisionCallback callback=tbAlgorithmTable[object1->shape->type][object2->shape->type];
	const tbShape* shape1=object1->shape,*shape2=object2->shape;
	tbState state1=object1->state,state2=object2->state;

	tbIntegrate(object1,&state1,timeStep);
	tbIntegrate(object2,&state2,timeStep);

	/* We test the two objects for interpenetration at t0+timeStep */
	if(callback(shape1,shape2,&state1,&state2,0))
	{
		/* We need to do a bisection to determine the time of collision */
		tbScalar bounds[2]={0,timeStep};
		tbScalar middle;
		unsigned int i=0;

		/* We set bounds to [t0,timeStep] */
		while(i<TB_ALGORITHM_ITERATIONS)
		{
			middle=(bounds[0]+bounds[1])*(tbScalar)0.5;

			state1=object1->state;
			state2=object2->state;

			tbIntegrate(object1,&state1,middle);
			tbIntegrate(object2,&state2,middle);

			if(callback(shape1,shape2,&state1,&state2,0))
				bounds[1]=middle;
			else
				bounds[0]=middle;

			i++;
		}

		/* We decide to stop now, when there's no collision */
		info->collisionTime=bounds[0];

		/* We must determine precisely the collision circumstances */
		state1=object1->state;
		state2=object2->state;

		tbIntegrate(object1,&state1,bounds[1]);
		tbIntegrate(object2,&state2,bounds[1]);

		/* This happens sometimes (but this a very rare phenomenon) because of floating point errors */
		if(!callback(shape1,shape2,&state1,&state2,info))
		{
			state1=object1->state;
			state2=object2->state;

			tbIntegrate(object1,&state1,timeStep);
			tbIntegrate(object2,&state2,timeStep);

			/* We get less precise collision info... or nothing */
			if(!callback(shape1,shape2,&state1,&state2,info))
				return 0;
		}

		return 1;
	}
	/* No collision occured (I hope so, if not, just decrease the timeStep value) */
	return 0;
}
