/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : param.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __param_h_
#define __param_h_

#include <map>
#include <functional>
#include <string>

namespace parser
{
  class param
  {
  public:
    enum class type
    {
      eNum,
      eVec,
      eMat,
      eTex,
      eShp
    };
  };
}

#endif

/* END OF 'param.h' FILE */