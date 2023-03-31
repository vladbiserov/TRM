/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : oper.cpp
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "oper.h"

#include "../variable.h"

const std::map<std::string, parser::obj::oper::type> parser::obj::oper::Table =
{
  {"union", parser::obj::oper::type::eUnion},
  {"smth_union", parser::obj::oper::type::eUnionSmth},
  {"diff", parser::obj::oper::type::eDiff},
  {"smth_diff", parser::obj::oper::type::eDiffSmth},
  {"inter", parser::obj::oper::type::eInter},
  {"smth_inter", parser::obj::oper::type::eInterSmth},
};

const std::map<parser::obj::oper::type, std::vector<parser::param::type>> parser::obj::oper::Types = 
{
  {parser::obj::oper::type::eUnion, {param::type::eShp, param::type::eShp}},
  {parser::obj::oper::type::eDiff, {param::type::eShp, param::type::eShp}},
  {parser::obj::oper::type::eInter, {param::type::eShp, param::type::eShp}},
  {parser::obj::oper::type::eUnionSmth, {param::type::eShp, param::type::eShp, param::type::eNum}},
  {parser::obj::oper::type::eDiffSmth, {param::type::eShp, param::type::eShp, param::type::eNum}},
  {parser::obj::oper::type::eInterSmth, {param::type::eShp, param::type::eShp, param::type::eNum}},
};

const std::map<parser::obj::oper::type, std::function<std::string(std::string, std::string, std::string, std::optional<std::string>)>> parser::obj::oper::ToStr
{
  { // Union
    parser::obj::oper::type::eUnion, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) + 
             std::format("{0} = SDFUnion({1}, {2});\n", Var, P1, P2) +
             GetMtl(Var, P1, P2); 
    })
  },
  { // Smooth union
    parser::obj::oper::type::eUnionSmth, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) +
              std::format("{0} = SDFUnionSmooth({1}, {2}, {3});\n", Var, P1, P2, K.value()) +
              GetMtl(Var, P1, P2); 
    })
  },
  { // Difference
    parser::obj::oper::type::eDiff, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) + 
             std::format("{0} = SDFDifer({1}, {2});\n", Var, P1, P2) +
             GetMtl(Var, P1, P2); 
    })
  },
  { // Smooth difference
    parser::obj::oper::type::eDiffSmth, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) + 
             std::format("{0} = SDFDiferSmooth({1}, {2}, {3});\n", Var, P1, P2, K.value()) +
             GetMtl(Var, P1, P2); 
    })
  },
  { // Intersection
    parser::obj::oper::type::eInter, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) + 
             std::format("{0} = SDFInter({1}, {2});\n", Var, P1, P2) +
             GetMtl(Var, P1, P2); 
    })
  },
  { // Smooth intersectioin
    parser::obj::oper::type::eInterSmth, std::function([](std::string Var, std::string P1, std::string P2, std::optional<std::string> K) -> std::string
    { 
      return CheckParamsMtl(Var, P1, P2) + 
             std::format("{0} = SDFInterSmooth({1}, {2}, {3});\n", Var, P1, P2, K.value()) +
             GetMtl(Var, P1, P2); 
    })
  },
};

/* END OF 'oper.cpp' FILE */