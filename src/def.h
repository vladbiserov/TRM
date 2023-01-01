/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : def.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __def_h_
#define __def_h_

#include <cassert>
#include <string>

#define GLEW_STATIC
#include <glew.h>

#include <wglew.h>
#include <gl/wglext.h>
 
#pragma comment(lib, "opengl32")

#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#undef WIN32
#endif

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

#include "math/mth.h"

namespace trm
{
  typedef mth::vec2<FLT> vec2;
  typedef mth::vec3<FLT> vec3;
  typedef mth::vec4<FLT> vec4;
  typedef mth::matr<FLT> matr;
  typedef mth::matr_data<FLT> matr_data;
  typedef mth::camera<FLT> camera;
  typedef mth::ray<FLT> ray;
  typedef mth::noise<FLT> noise;
}

#endif /* __def_h_ */

/* END OF 'def.h' FILE */