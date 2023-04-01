/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : variable.cpp
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "variable.h"

std::map<std::string, std::string> parser::variables::Shapes;
bool parser::variables::IsFirst = true;

std::map<std::string, parser::variables::data> parser::variables::Table =
{
  {"PI", {var_type::eFloat, 3.1415}},
  {"Time", {var_type::eFloat, 0}}
};

std::map<std::string, parser::var_type> parser::TYPES = 
{
  {"int", parser::var_type::eInt},
  {"double", parser::var_type::eFloat},
  {"vec3", parser::var_type::eVec},
  {"shape", parser::var_type::eShape},
  {"light", parser::var_type::eLight},
  {"mtl", parser::var_type::eMtl},
};

std::map<std::string, parser::state_type> parser::STATES = 
{
  {"rm_ao",      parser::state_type::eAO},
  {"rm_reflect", parser::state_type::eReflect},
  {"rm_shadow",  parser::state_type::eShadow},
  {"rm_skybox",  parser::state_type::eSky},
};

std::map<parser::state_type, bool> parser::variables::Flags = 
{
  {parser::state_type::eAO, false},
  {parser::state_type::eReflect, true},
  {parser::state_type::eShadow, true},
  {parser::state_type::eSky, true},
};

/* END OF 'variable.cpp' FILE */