#include "tbConfig.h"
#include "tbWindow.h"
#include "tbMemory.h"
#include "tbResources.h"

#include <math.h>

tbWindow* tbCreateWindow(unsigned int width,unsigned int height,unsigned int bitsPerPixel)
{
	const tbIcon* icon;
	tbWindow* window;
	unsigned int i;

	if(!(SDL_WasInit(SDL_INIT_VIDEO)&SDL_INIT_VIDEO))
	{
		if(SDL_Init(SDL_INIT_VIDEO)<0)
			return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,bitsPerPixel);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_WM_SetCaption("TheBox2",NULL);

	window=tbAlloc(sizeof(tbWindow));

	icon=tbGetIcon();
	if(icon)
	{
		/* An icon was found, build the SDL_Surface object from it */
		SDL_Surface* image;
		image=SDL_CreateRGBSurface(SDL_SWSURFACE,32,32,8,0,0,0,0);
		SDL_LockSurface(image);
		memcpy(image->pixels,icon->image,sizeof(unsigned char)*32*32);
		memcpy(image->format->palette->colors,icon->colors,sizeof(unsigned char)*16*4);
		SDL_UnlockSurface(image);
		SDL_SetColorKey(image,SDL_SRCCOLORKEY,1);
		SDL_WM_SetIcon(image,0);

		window->icon=image;
	}

	for(i=0;i<TB_CURSOR_MAX;i++)
	{
		/* SDL_CreateCursor doesn't use "const" arguments, that's why I made a crappy cast */
		tbCursor* cursor=(tbCursor*)tbGetCursor(i);
		window->cursors[i]=SDL_CreateCursor(cursor->image,cursor->mask,16,16,cursor->x,cursor->y);
	}

	SDL_SetVideoMode(width,height,bitsPerPixel,SDL_OPENGL);

	window->width=width;
	window->height=height;
	window->bitsPerPixel=bitsPerPixel;

	return window;
}

void tbSetCursor(tbWindow* window,unsigned int type)
{
	if(type<TB_CURSOR_MAX)
		SDL_SetCursor(window->cursors[type]);
}

void tbSetView(tbWindow* window,const tbVector center,tbScalar scale)
{
	const tbVector size={window->width*scale,window->height*scale};

	/* Adjust viewport and view matrix */
	glViewport(0,0,window->width,window->height);
	window->scale=scale;
	window->center[0]=center[0];
	window->center[1]=center[1];
	glLoadIdentity();
	glOrtho(center[0]-size[0],center[0]+size[0],center[1]-size[1],center[1]+size[1],-1.0,1.0);

	/* Adjust line and point size values */
	glLineWidth(fmaxf(1.0f,fminf(0.02f/(float)scale,1.8f)));
	glPointSize(fmaxf(3.0f,fminf(0.08f/(float)scale,6.0f)));
}

void tbDestroyWindow(tbWindow* window)
{
	unsigned int i;

	for(i=0;i<TB_CURSOR_MAX;i++)
		SDL_FreeCursor(window->cursors[i]);
	SDL_FreeSurface(window->icon);

	tbFree(window);
	SDL_Quit();
}
