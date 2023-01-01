/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : buffers.cpp
  * PURPOSE     : Ray marching project.
  *               Animation module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include "buffers.h"
#include "../../../def.h"

/* Apply buffers 
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID trm::buffer::Apply( VOID )
{
  if (BindPoint != 0)
    glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, Id);
} // end of 'trm::buffer::Apply' function

/* Free buffers 
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID trm::buffer::Free( VOID )
{
  if (Id != 0)
  {
    glDeleteBuffers(1, &Id);
    Id = 0;
    BindPoint = 0;
    NumOfQuads = 0;
  }
} // end of 'trm::buffer::Free' function

/* END OF 'buffers.cpp' FILE */