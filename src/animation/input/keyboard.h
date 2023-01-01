/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : keyboard.h
  * PURPOSE     : Ray marching project.
  *               Keyboard module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __keyboard_h_
#define __ketboard_h_

#include <cstring>
#include "../../def.h"

/* NSF name space */
namespace trm
{
  /* Keyboard handler class */
  class keyboard
  {
  private:
    BYTE KeysOld[256]; /* state of keys on the current frame */
  public:
    /* keyboard responsing */
    BYTE Keys[256];      /* state of keys on the current frame */
    BYTE KeysClick[256]; /* signs of a single click of the keyboard */
    /* Constructor */
    keyboard( VOID )
    {
      memset(Keys, 0, 256);
      memset(KeysOld, 0, 256);
      memset(KeysClick, 0, 256);
    } /* End of constructor */

    /* Animation ketboard keyboard response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      INT i;

      BOOL p = GetKeyboardState(Keys);
      for (i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];
      }
      memcpy(KeysOld, Keys, 256);
    } /* End of 'Response' function */
  }; /* End of 'keyboard' class */
} /* end of 'trm' name space */

#endif /* __ketboard_h_ */

/* END OF 'ketboard.h' FILE */