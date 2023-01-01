/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : target.h
  * PURPOSE     : Animation project.
  *               Animation module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : macg.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __target_h_
#define __target_h_

#include "resource.h"
#include "shader.h"
#include "texture.h"

// Project namespace
namespace trm
{
  /* Max count of FBO */
  constexpr auto MAX_FBO = 3;

  // Frame buffer object class
  class target
  {
    friend class render;
  private:
    GLuint FBO;             // Framebuffer object
    GLuint FBOTex[MAX_FBO]; // Array of textures
    GLenum FBOBuf;          // Framebuffer enum
    shader *Shd;            // Target shader

  public:

    /* Get position from framebuffer function
     * ARGUMENTS:
     *   - x coord:
     *       size_t X;
     *   - y coord:
     *       size_t Y; 
     * RETURNS:
     *   (vec2) position on xz plane;
     */
    vec2 GetPos( size_t X, size_t Y )
    {
      vec4 P;
      glGetTextureSubImage(FBOTex[1],
        0,
        X,
        Y,
        0,
        1,
        1,
        1,
        GL_RGBA,
        GL_FLOAT,
        sizeof vec4,
        &P);
      return vec2(P.X, P.Z);
    } /* End of 'GetPos' function */

    /* Get ID from framebuffer function
     * ARGUMENTS:
     *   - x coord:
     *       size_t X;
     *   - y coord:
     *       size_t Y;
     * RETURNS:
     *   (int32_t) instance ID;
     */
    int32_t GetId( size_t X, size_t Y )
    {
      int32_t I;
      glGetTextureSubImage(FBOTex[2], 0, X, Y, 0, 1, 1, 1, GL_RED_INTEGER, GL_INT, sizeof int32_t, &I);
      return I;
    } /* End of 'GetId' function */

    /* Get ID from framebuffer function
     * ARGUMENTS:
     *   - shader for binding:
     *       shader *Shd;
     * RETURNS: None.
     */
    VOID BindShader( shader *Shd )
    {
      this->Shd = Shd;
    } /* End of 'BindShader' function */

    /* Create framebuffer function
     * ARGUMENTS:
     *   - weight:
     *       int32_t W;
     *   - height:
     *       int32_t H;
     * RETURNS: None.
     */
    VOID Create( int32_t W, int32_t H )
    {
      GLenum DrawBuf[MAX_FBO];

      glGenFramebuffers(1, &FBO);
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);

      for (GLenum i = 0; i < MAX_FBO - 1; i++)
      {
        DrawBuf[i] = GL_COLOR_ATTACHMENT0 + i;

        glGenTextures(1, &FBOTex[i]);
        glBindTexture(GL_TEXTURE_2D, FBOTex[i]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, W, H);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, FBOTex[i], 0);
      }
      DrawBuf[MAX_FBO - 1] = GL_COLOR_ATTACHMENT0 + MAX_FBO - 1;

      glGenTextures(1, &FBOTex[MAX_FBO - 1]);
      glBindTexture(GL_TEXTURE_2D, FBOTex[MAX_FBO - 1]);
      glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32I, W, H);
      glBindTexture(GL_TEXTURE_2D, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + MAX_FBO - 1, GL_TEXTURE_2D, FBOTex[MAX_FBO - 1], 0);

      glDrawBuffers(MAX_FBO, DrawBuf);

      glGenRenderbuffers(1, &FBOBuf);
      glBindRenderbuffer(GL_RENDERBUFFER, FBOBuf);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, W, H);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBOBuf);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } /* End of 'Create' funciton */

    /* Start frame function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Start( VOID )
    {
      FLT Color[MAX_FBO - 1][4] = { {0.30, 0.50, 0.8, 1}, {0.50, 0.30, 0.8, 1} }, Ind = 0.5, Depth = 1;

      glBindFramebuffer(GL_FRAMEBUFFER, FBO);
      for (GLint i = 0; i < MAX_FBO - 1; i++)
        glClearBufferfv(GL_COLOR, i, Color[i]);
      glClearBufferfv(GL_COLOR, MAX_FBO - 1, &Ind);

      glClearBufferfv(GL_DEPTH, 0, &Depth);
    } /* End of 'Start' function */

    /* End frame function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID End( VOID )
    {
      FLT Color[4] = { 1, 0, 0, 1 }, Depth = 1;

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glFinish();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClearBufferfv(GL_COLOR, 0, Color);
      glClearBufferfv(GL_DEPTH, 0, &Depth);

      if (Shd->Apply())
      {
        for (GLenum i = 0; i < MAX_FBO; i++)
        {
          glActiveTexture(GL_TEXTURE0 + i);
          glBindTexture(GL_TEXTURE_2D, FBOTex[i]);
        }

        glDrawArrays(GL_POINTS, 0, 1);
      }

      glFinish();
    } /* End of 'End' function */

    /* Free framebuffer function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      glBindFramebuffer(GL_FRAMEBUFFER, FBO);

      for (GLenum i = 0; i < MAX_FBO; i++)
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
        glDeleteTextures(1, &FBOTex[i]);
      }
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
      glDeleteRenderbuffers(1, &FBOBuf);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDeleteFramebuffers(1, &FBO);
    } /* End of 'Free' function */

    /* Resize framebuffer function
     * ARGUMENTS:
     *   - weight:
     *       int32_t W;
     *   - height:
     *       int32_t H;
     * RETURNS: None.
     */
    VOID Resize( int32_t W, int32_t H )
    {
      Free();
      Create(W, H);
      glViewport(0, 0, W, H);
    } /* End of 'Resize' function */

    // Destructor
    ~target( VOID )
    {
      Free();
    } /* End of destructor */

  }; /* End of 'target' class */
} /* end of 'macg' namespace */

#endif // __target_h_

/* END OF 'target.h' FILE */
