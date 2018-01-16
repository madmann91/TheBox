#include "tbApplication.h"
#include "tbMemory.h"

void tbDestroyApplication(tbApplication* application)
{
	unsigned int i;

	if(application->broadphase)
		tbDestroyBroadphase(application->broadphase);
	if(application->simulation)
		tbDestroySimulation(application->simulation);
	if(application->interface)
		tbDestroyInterface(application->interface);
	if(application->window)
		tbDestroyWindow(application->window);
	if(application->renderer)
		tbDestroyRenderer(application->renderer);

	if(application->shapes)
	{
		for(i=0;i<application->numShapes;i++)
			tbDestroyShape(application->shapes[i]);
		tbFree(application->shapes);
	}
}