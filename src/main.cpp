/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : main.cpp
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <cstdio>
#include "animation/animation.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  trm::animation *myw = trm::animation::GetPtr();

  *myw  << "Test" << "Control";

  return myw->Run();
} /* End of 'WinMain' function */