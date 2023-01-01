/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : mouse.h
  * PURPOSE     : Ray marching project.
  *               Mouse module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __mouse_h_
#define __mouse_h_

#include <cstring>
#include "../../def.h"

/* NSF name space */
namespace trm
{
  /* Mouse handler class */
  class mouse
  {
  private:
    INT &Wheel;
    HWND hWnd;
  public: 
    /* mouse responding */
    INT MouseX, MouseY, MouseZ;
    INT MouseDx, MouseDy, MouseDz;
    /* Constructor */
    mouse( INT &WheelRef, HWND hWnd ) :
      Wheel(WheelRef), hWnd(hWnd), 
      MouseX(0), MouseY(0), MouseZ(0), MouseDx(0), MouseDy(0), MouseDz(0)
    {
    } /* End of constructor */

    /* Animation mouse input response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);

      /* transform coordinates on the screen */
      MouseDx = pt.x - MouseX;
      MouseDy = pt.y - MouseY;
      MouseDz = Wheel;
      /* absolute val */
      MouseX = pt.x;
      MouseY = pt.y;
      MouseZ += Wheel;

      Wheel = 0;
    } /* End of 'Response' function */
  }; /* End of 'mouse' class */
} /* end of 'trm' name space */

#endif /* __mouse_h_ */

/* END OF 'mouse.h' FILE */