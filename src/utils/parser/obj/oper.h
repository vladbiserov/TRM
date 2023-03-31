/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : oper.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __oper_h_
#define __oper_h_

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
    class oper
    {
    public:
      enum class type
      {
        eUnion,
        eUnionSmth,
        eDiff,
        eDiffSmth,
        eInter,
        eInterSmth,
      };

      static const std::map<std::string, type> Table;

      static const std::map<type, std::vector<param::type>> Types;

      static const std::map<type, std::function<std::string(std::string, std::string, std::string, std::optional<std::string>)>> ToStr;

    private:
      static std::string CheckParamsMtl(const std::string& Val, const std::string& P1, const std::string& P2)
      {
        if (Val == P1 || Val == P2)
          return std::format(
            "tmp = {0};\n"
            "tmp_mtl = mtl_{0};\n", Val);
        else
          return "";

      }

      static std::string GetMtl(const std::string& Val, const std::string& P1, const std::string& P2)
      {
        std::string tmp = "tmp", par;

        if (Val == P1)
          par = P2;
        else if (Val == P2)
          par = P1;

        if (par == "")
          return std::format(
            "if ({0} == {1})\n"
            "  mtl_{0} = mtl_{1};\n"
            "else if ({0} == {2})\n"
            "  mtl_{0} = mtl_{2};\n"
            "else\n"
            "  mtl_{0} = SDFSurfaceSmoothUnion({1}, mtl_{1}, {2}, mtl_{2}, 0.5);\n", Val, P1, P2);
        else
          return std::format(
            "if ({0} == {1})\n"
            "  mtl_{0} = {1}_mtl;\n"
            "else if ({0} == {2})\n"
            "  mtl_{0} = mtl_{2};\n"
            "else\n"
            "  mtl_{0} = SDFSurfaceSmoothUnion({1}, {1}_mtl, {2}, mtl_{2}, 0.5);\n", Val, tmp, par);
      }
    };
  }
}

#endif

/* END OF 'oper.h' FILE */