#include "tbConfig.h"
#include "tbRenderer.h"
#include "tbMemory.h"

static void tbRenderBox(tbRenderer*,const tbObject*);
static void tbRenderCircle(tbRenderer*,const tbObject*);

const tbRenderCallback tbRenderingTable[TB_SHAPE_MAX]={tbRenderBox,tbRenderCircle};

tbRenderer* tbCreateRenderer(tbWindow* window,unsigned int quality)
{
	tbRenderer* renderer;
	unsigned int numSegments;
	unsigned int i;
	tbScalar lastAngle=(tbScalar)0.0;
	tbScalar* buffer;

	renderer=tbAlloc(sizeof(tbRenderer));
	tbMemorySet(renderer,0,sizeof(tbRenderer));
	renderer->window=window;

	glEnableClientState(GL_VERTEX_ARRAY);
	if(quality>=25)
	{
		glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		if(quality>=50)
		{
			glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
			glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
			glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		}
		else
		{
			glHint(GL_POINT_SMOOTH_HINT,GL_DONT_CARE);
			glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
			glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
		}
	}

	if(quality>100)
		quality=100;

	numSegments=(quality*150)/100;
	renderer->circleBuffer=tbAlloc(sizeof(tbScalar)*4*numSegments);
	renderer->numSegments=numSegments;
	buffer=renderer->circleBuffer;
	for(i=1;i<=numSegments;i++)
	{
		tbScalar angle=(tbScalar)i*(2*TB_PI)/(tbScalar)numSegments;
		buffer[0]=tbCos(lastAngle);
		buffer[1]=tbSin(lastAngle);

		buffer[2]=tbCos(angle);
		buffer[3]=tbSin(angle);

		lastAngle=angle;
		buffer+=4;
	}

	renderer->crossBuffer[0]=-TB_RENDERER_CROSS;
	renderer->crossBuffer[2]=TB_RENDERER_CROSS;
	renderer->crossBuffer[5]=-TB_RENDERER_CROSS;
	renderer->crossBuffer[7]=TB_RENDERER_CROSS;

	return renderer;
}

static void tbRenderBox(tbRenderer* renderer,const tbObject* object)
{
	tbShape* shape=object->shape;
	tbBox* box=(tbBox*)(shape+1);
	tbScalar vertices[16]={-box->halfExtents[0],-box->halfExtents[1],
				-box->halfExtents[0],box->halfExtents[1],
				-box->halfExtents[0],box->halfExtents[1],
				box->halfExtents[0],box->halfExtents[1],
				box->halfExtents[0],box->halfExtents[1],
				box->halfExtents[0],-box->halfExtents[1],
				box->halfExtents[0],-box->halfExtents[1],
				-box->halfExtents[0],-box->halfExtents[1]};

	glPushMatrix();
#ifndef TB_DOUBLE
	glTranslatef(object->state.position[0],object->state.position[1],0.0f);
	glRotatef(object->state.angle*(tbScalar)180.0/TB_PI,0.0f,0.0f,1.0f);
	glVertexPointer(2,GL_FLOAT,0,vertices);
#else
	glTranslated(object->state.position[0],object->state.position[1],0.0);
	glRotated(object->state.angle*(tbScalar)180.0/TB_PI,0.0,0.0,1.0);
	glVertexPointer(2,GL_DOUBLE,0,vertices);
#endif
	
	glDrawArrays(GL_LINES,0,8);
#ifndef TB_DOUBLE
	glVertexPointer(2,GL_FLOAT,0,renderer->crossBuffer);
#else
	glVertexPointer(2,GL_DOUBLE,0,renderer->crossBuffer);
#endif
	glDrawArrays(GL_LINES,0,4);
	glPopMatrix();
}

static void tbRenderCircle(tbRenderer* renderer,const tbObject* object)
{
	tbShape* shape=object->shape;
	tbCircle* circle=(tbCircle*)(shape+1);

	glPushMatrix();
#ifndef TB_DOUBLE
	glTranslatef(object->state.position[0],object->state.position[1],0.0f);
	glRotatef(object->state.angle*(tbScalar)180.0/TB_PI,0.0f,0.0f,1.0f);
	glVertexPointer(2,GL_FLOAT,0,renderer->crossBuffer);
#else
	glTranslated(object->state.position[0],object->state.position[1],0.0);
	glRotated(object->state.angle*(tbScalar)180.0/TB_PI,0.0,0.0,1.0);
	glVertexPointer(2,GL_DOUBLE,0,renderer->crossBuffer);
#endif

	glDrawArrays(GL_LINES,0,4);
	
#ifndef TB_DOUBLE
	glScalef(circle->radius,circle->radius,1.0f);
#else
	glScaled(circle->radius,circle->radius,1.0);
#endif

#ifndef TB_DOUBLE
	glVertexPointer(2,GL_FLOAT,0,renderer->circleBuffer);
#else
	glVertexPointer(2,GL_DOUBLE,0,renderer->circleBuffer);
#endif
	glDrawArrays(GL_LINES,0,renderer->numSegments*2);

	glPopMatrix();
}

void tbRender(tbRenderer* renderer,const tbSimulation* simulation)
{
	unsigned int i;

#ifdef TB_STATISTICS
	unsigned long ticks;
	renderer->frames++;
	ticks=tbGetTicks();
#endif

	glClear(GL_COLOR_BUFFER_BIT);

	for(i=0;i<simulation->numObjects;i++)
	{
		const tbObject* object=&simulation->objects[i];

		tbRenderingTable[object->shape->type](renderer,object);
	}

	for(i=0;i<simulation->numContacts;i++)
	{
		tbContact* contact=&simulation->contacts[i];

		tbVector perp,arrow;

		tbPerp(contact->normal,perp);
		tbScale(perp,TB_RENDERER_ARROW*(tbScalar)0.5);

		arrow[0]=contact->point[0]+((tbScalar)1.0-TB_RENDERER_ARROW)*contact->normal[0];
		arrow[1]=contact->point[1]+((tbScalar)1.0-TB_RENDERER_ARROW)*contact->normal[1];

		glColor3f(0.0f,1.0f,0.0f);
		glBegin(GL_LINES);
#ifndef TB_DOUBLE
		glVertex2fv(contact->point);
		glVertex2fv(arrow);
#else
		glVertex2dv(contact->point);
		glVertex2dv(arrow);
#endif
		glEnd();

		glBegin(GL_TRIANGLES);
#ifndef TB_DOUBLE
		glVertex2f(contact->point[0]+contact->normal[0],contact->point[1]+contact->normal[1]);
		glVertex2f(arrow[0]+perp[0],arrow[1]+perp[1]);
		glVertex2f(arrow[0]-perp[0],arrow[1]-perp[1]);
#else
		glVertex2d(contact->point[0]+contact->normal[0],contact->point[1]+contact->normal[1]);
		glVertex2d(arrow[0]+perp[0],arrow[1]+perp[1]);
		glVertex2d(arrow[0]-perp[0],arrow[1]-perp[1]);
#endif
		glEnd();

		glColor3f(1.0f,0.0f,0.0f);

		glBegin(GL_POINTS);
#ifndef TB_DOUBLE
		glVertex2fv(contact->point);
#else
		glVertex2dv(contact->point);
#endif
		glEnd();
	}

	glColor3f(1.0f,1.0f,1.0f);

	SDL_GL_SwapBuffers();

#ifdef TB_STATISTICS
	renderer->ticks+=tbGetTicks()-ticks;
#endif
}

void tbDestroyRenderer(tbRenderer* renderer)
{
	tbFree(renderer->circleBuffer);
	tbFree(renderer);
}
