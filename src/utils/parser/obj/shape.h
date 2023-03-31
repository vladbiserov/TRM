/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : shape.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __shape_h_
#define __shape_h_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <format>
#include <optional>

#include "param.h"

namespace parser
{
  namespace obj
  {
    class shape
    {
    private:
      static void AddTex(const std::string& Name)
      {
        if (Texs.find(Name) != Texs.end())
          return;
        Texs.emplace(Name, "");
      }

      static std::map<std::string, std::string> Texs;

    public:
      enum class type
      {
        eSphere,
        eBox,
        eCylinder,
        eCapsule,
        ePlane,
        eTorus,
        eEllipsoid
      };

      static const std::map<std::string, type> Table;

      static const std::map<type, std::vector<param::type>> Types;

      static const std::map<type, std::function<std::string(std::string, std::vector<std::string>, bool)>> ToStr;

      static std::string GetTexStr(void);
    };
  }
}

#endif 

/* END OF 'shape.h' FILE */