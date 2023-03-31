/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : mod.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __mod_h_
#define __mod_h_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <format>

#include "param.h"

namespace parser
{
  namespace obj
  {
    class mod
    {
    public:
      enum class type
      {
        eRotate,
        eTranslate,
        eScale,
      };

      static const std::map<std::string, type> Table;

      static const std::map<type, std::vector<param::type>> Types;

      static const std::map<type, std::function<std::string(std::string, std::vector<std::string>)>> ToStr;
    };
  }
}

#endif 

/* END OF 'mod.h' FILE */