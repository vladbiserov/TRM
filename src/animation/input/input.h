/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : input.h
  * PURPOSE     : Ray marching project.
  *               Input module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __input_h_
#define __input_h_

#include "keyboard.h"
#include "mouse.h"

/* NSF name space */
namespace trm
{
  /* Input system class */
  class input : public keyboard, public mouse
  { 
  protected:
    /* Constructor */
    input( INT &WheelRef, HWND hWnd ) : keyboard(), mouse(WheelRef, hWnd)
    {
    } /* End of constructor */

    /* Input inter frame events handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response(VOID)
    {
      keyboard::Response();
      mouse::Response();
    } /* End of 'Response' function */
  }; /* End of 'input' class */
} /* end of 'trm' name space */

#endif /* __input_h_ */

/* END OF 'input.h' FILE */