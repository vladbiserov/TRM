/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : render.cpp
  * PURPOSE     : Ray marching project.
  *               Render module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include <cstdio> 
#include "render.h"
#include "../input/timer.h"

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *      UINT Source;
 *   - error type:
 *      UINT Type;
 *   - error message id:
 *      UINT Id, 
 *   - message severity:
 *      UINT severity, 
 *   - message text length:
 *      INT Length, 
 *   - message text:
 *      CHAR *Message, 
 *   - user addon parameters pointer:
 *      VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY trm::render::glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                           INT Length, const CHAR *Message, const VOID *UserParam )
{
  INT len = 0;
  static CHAR Buf[10000];
 
  /* Ignore non-significant error/warning codes */
  if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
    return;

  len += sprintf(Buf + len, "Debug message (%i) %s\n", Id, Message);
  switch (Source)
  {
  case GL_DEBUG_SOURCE_API:
    len += sprintf(Buf + len, "%s", "Source: API%s");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    len += sprintf(Buf + len, "%s", "Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    len += sprintf(Buf + len, "%s", "Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    len += sprintf(Buf + len, "%s", "Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    len += sprintf(Buf + len, "%s", "Source: Application");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    len += sprintf(Buf + len, "%s", "Source: Other");
    break;
  }
  len += sprintf(Buf + len, "%s", "\n");
 
  switch (Type)
  {
  case GL_DEBUG_TYPE_ERROR:
    len += sprintf(Buf + len, "%s", "Type: Error");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    len += sprintf(Buf + len, "%s", "Type: Deprecated Behaviour");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    len += sprintf(Buf + len, "%s", "Type: Undefined Behaviour");
    break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    len += sprintf(Buf + len, "%s", "Type: Portability");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    len += sprintf(Buf + len, "%s", "Type: Performance");
    break;
  case GL_DEBUG_TYPE_MARKER:
    len += sprintf(Buf + len, "%s", "Type: Marker");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    len += sprintf(Buf + len, "%s", "Type: Push Group");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    len += sprintf(Buf + len, "%s", "Type: Pop Group");
    break;
  case GL_DEBUG_TYPE_OTHER:
    len += sprintf(Buf + len, "%s", "Type: Other");
    break;
  }
  len += sprintf(Buf + len, "%s", "\n");
 
  switch (Severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    len += sprintf(Buf + len, "%s", "Severity: high");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    len += sprintf(Buf + len, "%s", "Severity: medium");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    len += sprintf(Buf + len, "%s", "Severity: low");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    len += sprintf(Buf + len, "%s", "Severity: notification");
    break;
  }
  len += sprintf(Buf + len, "%s", "\n\n");
 
  OutputDebugString(Buf);
} /* End of 'glDebugOutput' function */

/* Render system type constructor.
 * ARGUMENTS:
 *   - window handle ref:
 *       HWND &hWnd;
 *   - window size ref:
 *       INT &W, &H;
 */
trm::render::render( HWND &hWnd, INT &W, INT &H ) :
  hWnd(hWnd), hDC(GetDC(hWnd)), FrameW(W), FrameH(H)
{
  INT i;
  UINT num;
  HGLRC hRC;
  PIXELFORMATDESCRIPTOR pfd = {0};
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  //hWnd = hWnd;

  /* Prepare window device contesxt */
  //hDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);

  /* OpenGL init: initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(hWnd, "Error extensions initializing", "Error", MB_OK | MB_ICONERROR);
    exit(0);
  }

  /* Modern OpenGL init */
  wglChoosePixelFormatARB(hDC, PixelAttribs, NULL, 1, &i, &num);
  hRC = wglCreateContextAttribsARB(hDC, NULL, ContextAttribs);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hGLRC);

  hGLRC = hRC;
  wglMakeCurrent(hDC, hGLRC);

  /* Render parameters setup */
  glClearColor(0.30, 0.47, 0.8, 1);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Trg.Create(W, H);
  Trg.BindShader(LoadShader("target"));

  Trg1.Create(W, H);
  Trg1.BindShader(LoadShader("target"));

  material *Mtl = AddMaterial("Jit", "jittering");
  Jit = jittering(CreateShader("jittering"));
  //Jit = jittering(CreatePrim(topology::base<vertex::point>(prim_type::POINTS, { vertex::point(vec3(0)) }), Mtl));

  // Set default render parameters
  //glClearColor(0.3, 0.5, 0.7, 1);

  UBO_CAMERA *UC = nullptr;
  UboCamera = CreateBuffer(2, UC);
} // End of 'render' constructor

/* Render system type destructor.
 * ARGUMENTS: None.
 */
trm::render::~render( VOID )
{
  primitive_manager::Close();
  shader_manager::Close();
  material_manager::Close();
  wglMakeCurrent(nullptr, nullptr);
  wglDeleteContext(hGLRC);
  ReleaseDC(hWnd, hDC);
  Trg.Free();
  Trg1.Free();
  Jit.Free();
} // End of 'render' destructor

/* Rendring resizing function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID trm::render::Resize( VOID )
{
  //glViewport(0, 0, FrameW, FrameH);
  Trg.Resize(FrameW, FrameH);
  Trg1.Resize(FrameW, FrameH);

  Camera.Resize(FrameW, FrameH);
} // End of 'trm::render::Resize' function

/* Rendring copy frame function.
  * ARGUMENTS:
      - WINDOW HANDLE:
          HWND hWnd;
  * RETURNS: None.
  */
VOID trm::render::CopyFrame( VOID )
{
  wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
} // End of 'trm::render::CopyFrame' function

/* Rendring start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::render::Start( VOID )
{
  UBO_CAMERA UC =
  {
    Camera.View,
    Camera.Proj,
    Camera.VP,
    vec4(Camera.Loc, Camera.FrameH),
    vec4(Camera.Dir, Camera.ProjDist),
    vec4(Camera.Right, Camera.Wp),
    vec4(Camera.Up, Camera.Hp),
    vec4(Camera.At, Camera.FrameW),
    vec4(0, 0, 0, Camera.Size),
  };
  UboCamera->Update(&UC);

  /* Clear frame */
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Trg.Start();
  //Jit.Update(GetTexture());
  //
  //Jit.Draw([&]( primitive *Pr )
  //  {
  //    Draw(Pr, matr::Identity());
  //  });
} // End of 'trm::render::Start' function

/* Rendring end function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID trm::render::End( VOID )
{
  //glFinish();

  Trg.End();

  //Trg1.Start();
  //
  Jit.Update(Trg.FBOTex[0]);
  //
  //Jit.Draw([&]( primitive *Pr )
  //  {
  //    Draw(Pr, matr::Identity());
  //  });
  //
  //Trg1.End();
} // End of 'trm::render::End' function

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - '*.OBJ' file name:
 *       const std::string &FileName;
 *   - pointer to material:
 *       material *Mtl;
 * RETURNS:
 *   (primitive *) loaded primitive.
 */
trm::primitive * trm::render::LoadPrim( const std::string &FileName, material *Mtl )
{
  return CreatePrim(FileName, Mtl);
} /* End of 'trm::render::LoadPrim' function */

/* Draw render primitive function.
 * ARGUMENTS:
 *   - primitive:
 *      primitive *Pr;
 *   - matrix:
 *      const matr &World;
 * RETURNS: None.
 */
VOID trm::render::Draw( primitive *Pr, const matr &World, INT InstanceCount )
{
  matr
    w = Pr->IsTrans ? Pr->Transform * World : World,
    wvp = w * Camera.VP;
  INT gl_prim_type = Pr->Type == prim_type::LINES ? GL_LINES :
                     Pr->Type == prim_type::MESH ? GL_TRIANGLES :
                     Pr->Type == prim_type::STRIP ? GL_TRIANGLE_STRIP :
                     Pr->Type == prim_type::PATH ? GL_PATCHES :
                     GL_POINTS;

  Pr->Material->Apply();
  UboCamera->Apply();

  /* making an array of vertices active */
  if (InstanceCount < 1)
    if (Pr->IBuf != 0)
    {
      glBindVertexArray(Pr->VA);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
    else
    {
      glBindVertexArray(Pr->VA);
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
      glBindVertexArray(0);
    }
  else
    if (Pr->IBuf != 0)
    {
      glBindVertexArray(Pr->VA);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElementsInstanced(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL,
       InstanceCount);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
    else
    {
      glBindVertexArray(Pr->VA);
      glDrawArraysInstanced(gl_prim_type, 0, Pr->NumOfElements, InstanceCount);
      glBindVertexArray(0);
    }
} /* End of 'trm::render::Draw' function */

/* Get texture from target function
 * ARGUMENTS: None.
 * RETURNS:
 *   (texture*) texture from target
 */
trm::texture *trm::render::GetTexture( VOID )
{
  vec4 *T = new vec4[Camera.FrameW * Camera.FrameH];
  
  glGetTextureSubImage(Trg.FBOTex[0], 0, 0, 0, 0, Camera.FrameW, Camera.FrameH, 1, GL_RGBA, GL_FLOAT, sizeof(vec4) * Camera.FrameW * Camera.FrameH, T);
  
  texture *Tex = CreateModelTexture(T, 4, Camera.FrameW, Camera.FrameH, "Mama");
  
  delete[] T;
  return Tex;
  //vec4 *T = new vec4[Camera.FrameW * Camera.FrameH];
  //
  //for (INT i = 0; i < Camera.FrameW; i++)
  //  for (INT j = 0; j < Camera.FrameH; j++)
  //    glGetTextureSubImage(Trg.FBOTex[0], 0, i, j, 0, 1, 1, 1, GL_RGBA, GL_FLOAT, sizeof vec4, &T[j + i * Camera.FrameH]);
  //
  //texture *Tex = CreateModelTexture(T, 4, Camera.FrameW, Camera.FrameH, "Mama");
  //
  //delete[] T;
  //return Tex;
} /* End of 'trm::render::GetTexture' function */

/* Draw render primitive function.
 * ARGUMENTS:
 *   - primitive:
 *      primitive *Pr;
 * RETURNS: None.
 */
VOID trm::render::Draw( primitive *Pr, INT InstanceCount )
{
  matr wvp = Pr->IsTrans ? Pr->Transform * Camera.VP : Camera.VP;
  INT gl_prim_type = Pr->Type == prim_type::LINES ? GL_LINES : 
                     Pr->Type == prim_type::MESH ? GL_TRIANGLES :
                     Pr->Type == prim_type::STRIP ? GL_TRIANGLE_STRIP :
                     Pr->Type == prim_type::PATH ? GL_PATCHES :
                     GL_POINTS;

  Pr->Material->Apply();

  if (InstanceCount < 1)
    if (Pr->IBuf != 0)    /* . . . старый код */
    {
      glBindVertexArray(Pr->VA);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
    else
    {
      glBindVertexArray(Pr->VA);
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
      glBindVertexArray(0);
    }
  else
    if (Pr->IBuf != 0)
    {
      glBindVertexArray(Pr->VA);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElementsInstanced(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL,
       InstanceCount);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
    else
    {
      glBindVertexArray(Pr->VA);
      glDrawArraysInstanced(gl_prim_type, 0, Pr->NumOfElements, InstanceCount);
      glBindVertexArray(0);
    }
} /* End of 'trm::render::Draw' function */

/* Free primitive resource function.
 * ARGUMENTS:
 *   - pointer to resource:
 *      primitive *Pr;
 * RETURNS: None.
 */
VOID trm::render::Free( primitive *Pr )
{
  primitive_manager::Delete(Pr);
} /* End of 'trm::render::Free' function */


/* Create shader function.
 * ARGUMENTS:
 *   - shader name:
 *       const std::string ShdFileNamePrefix;
 * RETURNS: 
 *   (shader *) Pointer to shader.
 */
trm::shader * trm::render::LoadShader( const std::string &ShdFileNamePrefix )
{
  return CreateShader(ShdFileNamePrefix);
} /* End of 'trm::render::LoadShader' function */

/* Add material function.
 * ARGUMENTS:
 *   - name of material:
 *       const std::string &MtlName;
 *   - shader name:
 *       const std::string &ShName;
 *   - buffer structure:
 *       ill_coefs &Ptr;
 *   - textures name:
 *       const std::string &Tex;
 * RETURNS: 
 *   (material *) Pointer to material.
 */
trm::material * trm::render::AddMaterial( const std::string &MtlName, const std::string &ShName )
{
  shader *Sh = LoadShader(ShName);
  if (Sh == nullptr)
    Sh = LoadShader("default");

  return CreateMaterial(MtlName, Sh);
} /* End of 'trm::render::AddMaterial' function */

/* END OF 'render.cpp' FILE */