/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : file.cpp
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <fstream>

#include "parser.h"

/* Read scene file function.
 * ARGUMENTS: None.
 * RETURNS:
 *  (bool) if read or not.
 */
bool parser::ReadFile(void)
{
  std::ifstream File(SceneIn);

  if (File.is_open())
  {
    std::string Tmp;
    while (File.good())
    {
      File >> Tmp;
      InBuf += Tmp;
    }
    File.close();
    return true;
  }
  return false;
} /* End of 'parser::ReadFile' function */

/* Update shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *  (bool) if wrote or not.
 */
bool parser::WriteFile(void)
{
  if (IsFailed)
  {
    ErrLog.Print();
    Clear();
    return false;
  }

  std::ofstream Out(ShaderOut);
  std::ifstream In(ShaderIn);
  std::string line;

  if (!Out.is_open() || !In.is_open())
    return false;

  while (getline(In, line))
  {
    if (line == "SCENE")
      Out << OutBuf;
    else if (line == "TEXTURE")
      for (int i = 0; i < TexCnt; i++)
      {
        Out << std::format("layout(binding = {0}) uniform sampler2D Tex{0};\n", i);
        Out << std::format("uniform bool IsTexture{};\n", i);
      }
    else if (line == "LIGHT")
    {
      if (PointLgtCnt != 0)
      {
        Out << std::format("bool IsPointLgt = true;\n");
        Out << std::format("const int PointLgtCnt = {};\n", PointLgtCnt);
        Out << std::format("const point_light PointLgt[PointLgtCnt] = {{{}}}; \n", PointLgtBuf);
      }
      else
      {
        Out << std::format("bool IsPointLgt = false;\n");
        Out << std::format("const int PointLgtCnt = 1;\n");
        Out << std::format("const point_light PointLgt[PointLgtCnt] = {{point_light(vec3(13, 8, 8), vec3(1, 0.7, 0.47), 0.4, 0.6, 0.06)}};\n");
      }
      if (DirLgtCnt != 0)
      {
        Out << std::format("bool IsDirLgt = true;\n");
        Out << std::format("const int DirLgtCnt = {};\n", DirLgtCnt);
        Out << std::format("const dir_light DirLgt[DirLgtCnt] = {{{}}}; \n", DirLgtBuf);
      }
      else
      {
        Out << std::format("bool IsDirLgt = false;\n");
        Out << std::format("const int DirLgtCnt = 1;\n");
        Out << std::format("const dir_light DirLgt[DirLgtCnt] = {{dir_light(vec3(1, 0, 0), vec3(0.1))}};\n");
      }
      if (SpotLgtCnt != 0)
      {
        Out << std::format("bool IsSpotLgt = true;\n");
        Out << std::format("const int SpotLgtCnt = {};\n", SpotLgtCnt);
        Out << std::format("const spot_light SpotLgt[SpotLgtCnt] = {{{}}}; \n", SpotLgtBuf);
      }
      else
      {
        Out << std::format("bool IsSpotLgt = false;\n");
        Out << std::format("const int SpotLgtCnt = 1;\n");
        Out << std::format("const spot_light SpotLgt[SpotLgtCnt] = {{spot_light(vec3(1, 2, 1), vec3(0, 1, 0), vec3(0.1), cos(0.1), cos(1.1))}};\n");
      }
    }
    else
      Out << line << std::endl;
  }

  Clear();

  Out.close();
  In.close();

  return true;
} /* End of 'parser::WriteFile' function */