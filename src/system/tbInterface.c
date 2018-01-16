#include "tbConfig.h"
#include "tbInterface.h"
#include "tbMemory.h"

tbInterface* tbCreateInterface(tbSimulation* simulation,tbWindow* window)
{
	tbInterface* interface;

	interface=tbAlloc(sizeof(tbInterface));
	interface->simulation=simulation;
	interface->window=window;

	return interface;
}

int tbUpdateInterface(tbInterface* interface)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						{
							const tbWindow* window=interface->window;
							const tbVector point={window->center[0]+((tbScalar)event.button.x-(tbScalar)window->width/2)*(2*window->scale),
												window->center[1]+((tbScalar)window->height/2-(tbScalar)event.button.y)*(2*window->scale)};
							const tbVector min={point[0]-0.2f,point[1]-0.2f},max={point[0]+0.2f,point[1]+0.2f};
							tbBroadphase* broadphase=interface->simulation->broadphase;

							/* TODO : select object to be grabbed */
							
							tbSetCursor(interface->window,TB_CURSOR_HAND);
							interface->state=TB_INTERFACE_GRAB;
						}
						break;
					case SDL_BUTTON_RIGHT:
						tbSetCursor(interface->window,TB_CURSOR_MOVE);
						interface->state=TB_INTERFACE_MOVE;
						break;
					default:
						tbSetCursor(interface->window,TB_CURSOR_ARROW);
						interface->state=0;
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				switch(interface->state)
				{
					case TB_INTERFACE_GRAB:
						/* TODO : grab object */
						break;
					case TB_INTERFACE_MOVE:
						{
							tbWindow* window=interface->window;
							tbVector center={window->center[0]-event.motion.xrel*window->scale*2,
								window->center[1]+event.motion.yrel*window->scale*2};
							tbSetView(window,center,window->scale);
						}
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:	
				tbSetCursor(interface->window,TB_CURSOR_ARROW);
				interface->state=0;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym==SDLK_SPACE)
					interface->simulation->flags^=TB_SIMULATION_PAUSED;
				if(event.key.keysym.sym==SDLK_a)
					tbSetView(interface->window,interface->window->center,interface->window->scale*(tbScalar)0.8);
				if(event.key.keysym.sym==SDLK_b)
					tbSetView(interface->window,interface->window->center,interface->window->scale*(tbScalar)1.25);
				break;
			case SDL_KEYUP:	
				if(interface->state)
					interface->state=0;
				break;
			case SDL_QUIT:
				return 0;
		}
	}
	return 1;
}

void tbDestroyInterface(tbInterface* interface)
{
	tbFree(interface);
}
