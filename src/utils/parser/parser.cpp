/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : parser.cpp
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
#include <iostream>
#include <format>
#include <sstream>

#include "parser.h"

/* Get expressions from file function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void parser::GetExprs(void)
{
  int Pos = 0;

  ClearTextures();

  while (1)
  {
    int StPos = Pos, TmpPos = 0;

    TmpPos = InBuf.find(';', Pos);
    if (TmpPos == std::string::npos)
      break;

    Pos = TmpPos;

    std::string Expr = InBuf.substr(StPos, Pos - StPos);

    int p = Expr.find("//");
    if (p != std::string::npos)
      Expr = Expr.substr(0, p);
    
    ParseVars(Expr);
    ParseAdd(Expr);
    Pos++;

    if (IsFailed)
      return;
  }
} /* End of 'parser::GetExpr' function */

/* Parse variables function.
 * ARGUMENTS:
 *   - expression to parse:
 *       const std::string& Expr;
 * RETURNS:
 *  (bool) if parse or not.
 */
bool parser::ParseVars(const std::string& Expr)
{
  int Pos = Expr.find('=');

  if (Pos == std::string::npos)
    return false;

  std::string
    VarName = Expr.substr(0, Pos),
    VarDescp = Expr.substr(Pos + 1, Expr.size() - Pos);

  if (Voc.find(VarName) == Voc.end())
  {
    bool IsMod = false;
    std::string res = GetStrByVarDesc(VarName, VarDescp, IsMod);
    if (res == "")
    {
      ErrLog.Log(std::format("error in '{}' declaration.", VarName));
      IsFailed = true;
      return false;
    }

    std::string mod;

    if (IsMod)
    {
      std::vector<std::string> P = ObjectHelper_GetParametrs(VarDescp);

      if (P.size() < 2) return false;
      if (Voc.find(P[1]) == Voc.end()) return false;

      Voc.emplace(VarName, Voc[P[1]]);

      std::string r = GetStrByVarDesc(VarName, Voc[VarName], IsMod);

      mod = res;
      res = r;
    }
    else
      Voc.emplace(VarName, VarDescp);

    ObjectHelper_PrintBuf(std::format("// create '{}' var", VarName));
    ObjectHelper_PrintBuf(std::format("surface mtl_{};", VarName));
    ObjectHelper_PrintBuf(std::format("vec2 uv_{};", VarName));
    ObjectHelper_PrintBuf(std::format("vec3 N2_{0}, N3_{0}, mod_{0} = point;", VarName));
    ObjectHelper_PrintBuf(std::format("float {};\n", VarName));

    if (res == "light")
      return true;

    if (mod != "")
      ObjectHelper_PrintBuf(mod);

    ObjectHelper_PrintBuf(res);
    ObjectHelper_PrintBuf("");
  }
  else
  {
    bool IsMod = false;
    std::string res = GetStrByVarDesc(VarName, VarDescp, IsMod);
    if (res == "")
    {
      ErrLog.Log(std::format("error in '{}' declaration.", VarName));
      IsFailed = true;
      return false;
    }

    if (!IsMod)
      Voc[VarName] = VarDescp;
    else
    {
      std::vector<std::string> P = ObjectHelper_GetParametrs(VarDescp);

      if (P.size() < 2) return false;
      if (Voc.find(P[1]) == Voc.end()) return false;

      std::string r = GetStrByVarDesc(VarName, Voc[VarName], IsMod);
      res += r;
    }

    if (res == "light")
      return true;

    ObjectHelper_PrintBuf(std::format("// update '{}' var", VarName));
    ObjectHelper_PrintBuf(res);
  }

  return true;
} /* End of 'parser::ParsVars' function */

/* Parse scene add objects function.
 * ARGUMENTS:
 *   - expression to parse:
 *       const std::string& Expr;
 * RETURNS:
 *  (bool) if parse or not.
 */
bool parser::ParseAdd(const std::string& Expr)
{
  int Pos = Expr.find("Scene<<");

  if (Pos == std::string::npos)
    return false;
  std::stringstream S(Expr);
  std::string Var;

  while (getline(S, Var, '<'))
  {
    if (Var == "Scene" || Var == "")
      continue;
    if (Voc.find(Var) == Voc.end())
      continue;
    
    std::string Type = Voc[Var];
    int p = Type.find('(');
    if (p > 0)
      Type = Type.substr(0, p);

    obj_type T = ObjectHelper_GetType(Type);
    if ((int)T >= 100)
    {
      std::string s = Lgts[Var]->GetStr("");
      lgt::lgt_type t = Lgts[Var]->GetType();

      if (t == lgt::POINT)
        PointLgtBuf += s + ",", PointLgtCnt++;
      else if (t == lgt::DIR)
        DirLgtBuf += s + ",", DirLgtCnt++;
      else if (t == lgt::SPOT)
        SpotLgtBuf += s + ",", SpotLgtCnt++;

      continue;
    }

    if (!IsFileInit)
    {
      ObjectHelper_PrintBuf(std::format("// add to scene '{}' var", Var));
      ObjectHelper_PrintBuf(std::format("res = {};", Var));
      ObjectHelper_PrintBuf(std::format("mtl = mtl_{};", Var));
      ObjectHelper_PrintBuf("");

      IsFileInit = true;
    }
    else
    {
      ObjectHelper_PrintBuf(std::format("// add to scene '{}' var", Var));
      ObjectHelper_PrintBuf(std::format("tmp = res;"));
      ObjectHelper_PrintBuf(std::format("tmp_mtl = mtl;"));
      ObjectHelper_PrintBuf(std::format("res = SDFUnion(tmp, {});", Var));
      ObjectHelper_PrintBuf(std::format(
      "if (res == {0})\n"
      "  mtl = mtl_{0};\n"
      "else if (res == tmp)\n"
      "  mtl = tmp_mtl;\n"
      "else\n"
      "  mtl = SDFSurfaceSmoothUnion(tmp, tmp_mtl, {0}, mtl_{0}, 0.5);\n", Var));
      ObjectHelper_PrintBuf("");
    }
  }

  return true;
} /* End of 'parser::ParseAdd' function */