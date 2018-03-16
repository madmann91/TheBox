/************************************************************************
*                                                                       *
*  File : tbConfig.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Link with external librairies                             *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_CONFIG_H__
#define __TB_CONFIG_H__

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <SDL/SDL.h>
#include <GL/gl.h>

#ifdef DEBUG
#include <assert.h>
#define tbAssert(x) assert(x);
#else
#define tbAssert(x) ;
#endif

#ifdef TB_STATISTICS
#include <time.h>
#define TB_STATISTICS_TICKS 5000
#define tbGetTicks() ((unsigned long)clock())
#endif

#endif
