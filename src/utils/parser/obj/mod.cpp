/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mod.cpp
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "mod.h"

#include "../variable.h"

const std::map<std::string, parser::obj::mod::type> parser::obj::mod::Table =
{
  {"rotate", parser::obj::mod::type::eRotate},
  {"translate", parser::obj::mod::type::eTranslate},
  {"scale", parser::obj::mod::type::eScale},
};

const std::map<parser::obj::mod::type, std::vector<parser::param::type>> parser::obj::mod::Types = 
{
  {parser::obj::mod::type::eRotate, {param::type::eNum, param::type::eVec, param::type::eShp}},
  {parser::obj::mod::type::eTranslate, {param::type::eVec, param::type::eShp}},
  {parser::obj::mod::type::eScale, {param::type::eVec, param::type::eShp}},
};

const std::map<parser::obj::mod::type, std::function<std::string(std::string, std::vector<std::string>)>> parser::obj::mod::ToStr
{
  { // Rotate
    parser::obj::mod::type::eRotate, std::function([](std::string Var, std::vector<std::string> P) -> std::string
    {
      return std::format("mod_{0} = Rotate({1}, {2}, mod_{0});\n", Var, P[0], P[1]) +
             variables::GetShape(Var);
    })
  },
  { // Scale
    parser::obj::mod::type::eScale, std::function([](std::string Var, std::vector<std::string> P) -> std::string
    {
      return std::format("mod_{0} = Scale({1}, mod_{0});\n", Var, P[0]) +
             variables::GetShape(Var);
    })
  },
  { // Translate
    parser::obj::mod::type::eTranslate, std::function([](std::string Var, std::vector<std::string> P) -> std::string
    {
      return std::format("mod_{0} = Translate({1}, mod_{0});\n", Var, P[0]) +
             variables::GetShape(Var);
    })
  },
};

/* END OF 'mod.cpp' FILE */