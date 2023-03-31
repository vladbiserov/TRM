/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : light.cpp
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "light.h"

std::map<std::string, std::pair<std::string, bool>>
  parser::obj::light::Point {}, parser::obj::light::Spot {}, parser::obj::light::Dir {};

int
  parser::obj::light::PointCnt = 0,
  parser::obj::light::DirCnt = 0,
  parser::obj::light::SpotCnt = 0;

const std::map<std::string, parser::obj::light::type> parser::obj::light::Table =
{
  {"point", parser::obj::light::type::ePoint},
  {"dir", parser::obj::light::type::eDir},
  {"spot", parser::obj::light::type::eSpot},
};

const std::map<parser::obj::light::type, std::vector<parser::param::type>> parser::obj::light::Types = 
{
  {parser::obj::light::type::ePoint, {param::type::eVec, param::type::eVec}},
  {parser::obj::light::type::eDir, {param::type::eVec, param::type::eVec}},
  {parser::obj::light::type::eSpot, {param::type::eVec, param::type::eVec, param::type::eVec}},
};

const std::map<parser::obj::light::type, std::function<void(std::string, std::vector<std::string>)>> parser::obj::light::Add
{
  /* Point light */
  {
    parser::obj::light::type::ePoint, std::function([](std::string Var, std::vector<std::string> P) -> void
    {
      Point.emplace(Var, std::make_pair(std::format("point_light({0}, {1}, 0.4, 0.6, 0.06)", P[0], P[1]), false));
    })
  }, /* End of 'Point Light' */
  /* Dir light */
  {
    parser::obj::light::type::eDir, std::function([](std::string Var, std::vector<std::string> P) -> void
    {
      Dir.emplace(Var, std::make_pair(std::format("dir_light({0}, {1})", P[0], P[1]), false));
    })
  }, /* End of 'Dir Light' */
  /* Spot light */
  {
    parser::obj::light::type::eSpot, std::function([](std::string Var, std::vector<std::string> P) -> void
    {
      Spot.emplace(Var, std::make_pair(std::format("spot_light({0}, {1}, {2}, cos(0.1), cos(1.1))", P[0], P[1], P[2]), false));
    })
  }, /* End of 'Spot Light' */
};

std::string parser::obj::light::GetStr( void )
{
  std::string res;

  if (PointCnt != 0)
  {
    std::string Buf;
    bool flag = false;

    for (auto &l : Point)
      if (l.second.second)
      {
        if (flag)
          Buf += ", ";
        Buf += l.second.first, flag = true;
      }
    res += std::format("bool IsPointLgt = true;\n");
    res += std::format("const int PointLgtCnt = {};\n", PointCnt);
    res += std::format("const point_light PointLgt[PointLgtCnt] = {{{}}}; \n", Buf);
  }
  else
  {
    res += std::format("bool IsPointLgt = false;\n");
    res += std::format("const int PointLgtCnt = 1;\n");
    res += std::format("const point_light PointLgt[PointLgtCnt] = {{point_light(vec3(13, 8, 8), vec3(1, 0.7, 0.47), 0.4, 0.6, 0.06)}};\n");
  }
  if (DirCnt != 0)
  {
    std::string Buf;
    bool flag = false;

    for (auto &l : Dir)
      if (l.second.second)
      {
        if (flag)
          Buf += ", ";
        Buf += l.second.first, flag = true;
      }

    res += std::format("bool IsDirLgt = true;\n");
    res += std::format("const int DirLgtCnt = {};\n", DirCnt);
    res += std::format("const dir_light DirLgt[DirLgtCnt] = {{{}}}; \n", Buf);
  }
  else
  {
    res += std::format("bool IsDirLgt = false;\n");
    res += std::format("const int DirLgtCnt = 1;\n");
    res += std::format("const dir_light DirLgt[DirLgtCnt] = {{dir_light(vec3(1, 0, 0), vec3(0.1))}};\n");
  }
  if (SpotCnt != 0)
  {
    std::string Buf;
    bool flag = false;

    for (auto &l : Spot)
      if (l.second.second)
      {
        if (flag)
          Buf += ", ";
        Buf += l.second.first, flag = true;
      }
    
    res += std::format("bool IsSpotLgt = true;\n");
    res += std::format("const int SpotLgtCnt = {};\n", SpotCnt);
    res += std::format("const spot_light SpotLgt[SpotLgtCnt] = {{{}}}; \n", Buf);
  }
  else
  {
    res += std::format("bool IsSpotLgt = false;\n");
    res += std::format("const int SpotLgtCnt = 1;\n");
    res += std::format("const spot_light SpotLgt[SpotLgtCnt] = {{spot_light(vec3(1, 2, 1), vec3(0, 1, 0), vec3(0.1), cos(0.1), cos(1.1))}};\n");
  }

  Point.clear();
  Dir.clear();
  Spot.clear();

  PointCnt = 0;
  DirCnt = 0;
  SpotCnt = 0;

  return res;
}

void parser::obj::light::Enable(const std::string &Name)
{
  if (Point.find(Name) != Point.end())
  {
    if (!Point[Name].second)
      Point[Name].second = true, PointCnt++;
  }
  else if (Spot.find(Name) != Spot.end())
  {
    if (!Spot[Name].second)
      Spot[Name].second = true, SpotCnt++;
  }
  else if (Dir.find(Name) != Dir.end())
  {
    if (!Dir[Name].second)
      Dir[Name].second = true, DirCnt++;
  }
}

/* END OF 'light.cpp' FILE */