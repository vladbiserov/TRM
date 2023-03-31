/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : shape.cpp
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "shape.h"

std::map<std::string, std::string> parser::obj::shape::Texs {};

const std::map<std::string, parser::obj::shape::type> parser::obj::shape::Table =
{
  {"sphere", parser::obj::shape::type::eSphere},
  {"box", parser::obj::shape::type::eBox},
  {"cylinder", parser::obj::shape::type::eCylinder},
  {"plane", parser::obj::shape::type::ePlane},
  {"torus", parser::obj::shape::type::eTorus},
  {"ellipsoid", parser::obj::shape::type::eEllipsoid},
  {"capsule", parser::obj::shape::type::eCapsule},
};

const std::map<parser::obj::shape::type, std::vector<parser::param::type>> parser::obj::shape::Types = 
{
  {parser::obj::shape::type::ePlane, {param::type::eVec, param::type::eNum, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eBox, {param::type::eVec, param::type::eVec, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eSphere, {param::type::eVec, param::type::eNum, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eEllipsoid, {param::type::eVec, param::type::eVec, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eTorus, {param::type::eVec, param::type::eVec, param::type::eNum, param::type::eNum, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eCylinder, {param::type::eVec, param::type::eNum, param::type::eVec, param::type::eNum, param::type::eMat, param::type::eTex}},
  {parser::obj::shape::type::eCapsule, {param::type::eVec, param::type::eVec, param::type::eNum, param::type::eMat, param::type::eTex}},
};

const std::map<parser::obj::shape::type, std::function<std::string(std::string, std::vector<std::string>, bool)>> parser::obj::shape::ToStr 
{
  { // Plane
    parser::obj::shape::type::ePlane, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[3]);

      return std::format("{0} = SDFPlane(mod_{0}, plane({1}, {2}, {3}));\nmtl_{0} = {3};\n", Var, P[0], P[1], P[2]);
    })
  },
  { // Box
    parser::obj::shape::type::eBox, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[3]);

      return std::format("{0} = SDFBox(mod_{0}, box({1}, {2}, {3}));\nmtl_{0} = {3};\n", Var, P[0], P[1], P[2]);
    })
  },
  { // Ellipsoid
    parser::obj::shape::type::eEllipsoid, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[3]);

      return std::format("{0} = SDFEllipsoid(mod_{0}, ellipsoid({1}, {2}, {3}));\nmtl_{0} = {3};\n", Var, P[0], P[1], P[2]);
    })
  },
  { // Sphere
    parser::obj::shape::type::eSphere, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[3]);

      return std::format("{0} = SDFSphere(mod_{0}, sphere({1}, {2}, {3}));\nmtl_{0} = {3};\n", Var, P[0], P[1], P[2]);
    })
  },
  { // Torus
    parser::obj::shape::type::eTorus, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[5]);

      return std::format("{0} = SDFTorus(mod_{0}, torus({1}, {2}, {3}, {4}, {5}));\nmtl_{0} = {5};\n", Var, P[0], P[1], P[2], P[3], P[4]);
    })
  },
  { // Cylinder
    parser::obj::shape::type::eCylinder, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[5]);

      return std::format("{0} = SDFCylinder(mod_{0}, cylinder({1}, {2}, {3}, {4}, {5}));\nmtl_{0} = {5};\n", Var, P[0], P[1], P[2], P[3], P[4]);
    })
  },
  { // Capsule
    parser::obj::shape::type::eCapsule, std::function([](std::string Var, std::vector<std::string> P, bool IsT) -> std::string
    {
      if (IsT)
        AddTex(P[4]);

      return std::format("{0} = SDFCapsule(mod_{0}, capsule({1}, {2}, {3}, {4}));\nmtl_{0} = {4};\n", Var, P[0], P[1], P[2], P[3]);
    })
  },
};

std::string parser::obj::shape::GetTexStr(void)
{
  std::string res;

  for (int i = 0; i < (int)Texs.size(); i++)
  {
    res += std::format("layout(binding = {0}) uniform sampler2D Tex{0};\n", i);
    res += std::format("uniform bool IsTexture{};\n", i);
  }

  return res;
}

/* END OF 'shape.cpp' FILE */