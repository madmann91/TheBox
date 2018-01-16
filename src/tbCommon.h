/************************************************************************
*                                                                       *
*  File : tbCommon.h                                                    *
*  Date : 25/10/10                                                      *
*  Comments : Common include file for the whole project                 *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_COMMON_H__
#define __TB_COMMON_H__

#ifdef TB_DOUBLE
typedef double tbScalar;
#define tbCos cos
#define tbSin sin
#define tbAbs fabs
#define tbSqrt sqrt
#define tbFmod fmod
#define tbLog log
#define TB_EPSILON 2.2204460492503131e-016
#define TB_PI 3.14159265358979323846
#define TB_MINIMUM 2.2250738585072014e-308
#define TB_MAXIMUM 1.7976931348623158e+308
#else
typedef float tbScalar;
#define tbCos cosf
#define tbSin sinf
#define tbAbs fabsf
#define tbSqrt sqrtf
#define tbFmod fmodf
#define tbLog logf
#define TB_EPSILON 1.192092896e-07f
#define TB_PI 3.14159265358979f
#define TB_MINIMUM 1.175494351e-38f
#define TB_MAXIMUM 3.402823466e+38f
#endif

#endif
