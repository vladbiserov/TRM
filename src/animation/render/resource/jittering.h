/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : jittering.h
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

#ifndef __jittering_h_
#define __jittering_h_

#include "resource.h"
#include "shader.h"
#include "primitive.h"
#include "texture.h"
#include <functional>

// Project namespace
namespace trm
{
  /* Jittering class */
  class jittering
  {
  private:
    shader *Shd; // shader for postproccesing draw
  public:
    /* Default constructor */
    jittering( VOID ) : Shd(nullptr)
    {
    } // End of constructor

    /* Constructor function.
     * ARGUMENTS:
     *   - fullscreen primitive:
     *       primitive *Pr;
     */
    jittering( shader *Shd ) : Shd(Shd)
    {
    } // End of constructor

    /* Default destructor */
    ~jittering( VOID )
    {
    } // End of destructor

    /* Free jittering function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      Shd->Free();
    } // End of 'Free' function

    /* Update texture in primitive function.
     * ARGUMENTS:
     *   - texture for updating:
     *       GLuint FBOTex;
     * RETURNS: None.
     */
    VOID Update( GLuint FBOTex )
    {
      FLT Color[4] = { 1, 0, 0, 1 }, Depth = 1;
      
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      
      glFinish();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClearBufferfv(GL_COLOR, 0, Color);
      glClearBufferfv(GL_DEPTH, 0, &Depth);

      if (Shd->Apply())
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FBOTex);
        glDrawArrays(GL_POINTS, 0, 1);
      }

      glFinish();
    } // End of 'Update' function
  }; /* End of 'jittering' class */
} /* end of 'trm' namespace */

#endif // __jittering_h_

/* END OF 'jittering.h' FILE */
