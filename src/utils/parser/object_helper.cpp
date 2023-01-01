/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : object_helper.cpp
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "parser.h"

/* Get variable type from string function.
 * ARGUMENTS:
 *   - string:
 *       const std::string &Str;
 * RETURNS:
 *     (obj_type) object type;
 */
parser::obj_type parser::ObjectHelper_GetType(const std::string &Str)
{
  if (Objects.find(Str) == Objects.end())
  {
    ErrLog.Log(std::format("'{}' incorrect function.", Str));
    return obj_type::NONE;
  }

  return Objects[Str];
} /* End of 'parser::ObjectHelper_GetType' function */

/* Add string to shader buffer function.
 * ARGUMENTS:
 *   - string to add:
 *       const std::string& Str;
 * RETURNS: None.
 */
void parser::ObjectHelper_PrintBuf(const std::string &Str)
{
  OutBuf += Str + "\n";
} /* End of 'parser::ObjectHelper_PrintBuf' function */

/* Check parameters in operation function.
 * ARGUMENTS:
 *   - parameters array:
 *       std::vector<std::string> Params;
 * RETURNS:
 *     (bool) true or false;
 */
bool parser::ObjectHelper_CheckOperParams(std::vector<std::string> Params)
{
  for (auto p : Params)
    if (Voc.find(p) == Voc.end())
    {
      ErrLog.Log(std::format("'{}' no such variable exists.", p));
      return false;
    }

  return true;
} /* End of 'parser::ObjectHelper_CheckOperParam' function */

/* Get function parameters from string function.
 * ARGUMENTS:
 *   - string:
 *       const std::string &Str;
 * RETURNS:
 *     (std::vector<std::string>) function parameters;
 */
std::vector<std::string> parser::ObjectHelper_GetParametrs(const std::string &Str)
{
  std::vector<std::string> V;
  int Cnt = 0, IsN = 0;
  std::string Cur;

  for (int i = 0; i < (int)Str.size(); i++)
  {
    if (Str[i] == '(' && !IsN)
    {
      V.emplace_back(Cur);
      Cur = "";
      IsN = 1;
      continue;
    }

    if ((Str[i] == ',' || Str[i] == ')') && Cnt == 0)
    {
      V.emplace_back(Cur);
      Cur = "";
      continue;
    }

    if (Str[i] == '(')
      Cnt++;
    else if (Str[i] == ')')
      Cnt--;
    
    Cur += Str[i];
  }

  return V;
} /* End of 'parser::ObjectHelper_GetParameters' function */

/* Add texture function.
 * ARGUMENTS:
 *   - texture name:
 *       const std::string& TexName;
 * RETURNS:
 *     (int) texture index;
 */
int parser::ObjectHelper_AddTex(const std::string &TexName)
{
  INT PointPos = TexName.find(".");

  if (PointPos == std::string::npos)
    return -1;

  if (TexName[0] != '\"' || TexName[TexName.size() - 1] != '\"')
    return -1;

  std::string
    Name = TexName.substr(1, PointPos - 1),
    Ext = TexName.substr(PointPos, TexName.size() - PointPos - 1),
    TmpTextureName = Name + Ext,
    TextureName = Name + Ext;

  if (Ext == ".G32" || Ext == ".g32" ||
    Ext == ".G24" || Ext == ".g24")
    ;//TextureName = TmpTexName;
  else
  {
    TextureName = Name + ".G32";
    std::string fmt = std::format("utils\\ANY2ANY.exe {} {}", "bin//images//" + TmpTextureName, "bin//images//" + TextureName);
    system(fmt.c_str());
  }

  if (Textures.find(TextureName) != Textures.end())
  {
  }
  else
  {
    Textures.emplace(TextureName, CountOfTexs);
    CountOfTexs++;
  }

  INT n = Textures[TextureName].n;

  for (auto i : AddedTex)
    if (i.first == n)
      return i.second;

  AddedTex.emplace(n, TexCnt);
  return TexCnt++;
} /* End of 'parser::ObjectHelper_AddTex' function */

/* Get string by variable name and parameters function.
 * ARGUMENTS:
 *   - variable name:
 *       const std::string &Var;
 *   - parameters array:
 *       std::vector<std::string> Params;
 *   - flag is modified:
 *       bool &IsMod;
 * RETURNS:
 *     (std::string) string;
 */
std::string parser::ObjectHelper_GetStr(const std::string &Var, std::vector<std::string> Params, bool &IsMod)
{
  if (Params.size() == 0)
    return "";

  IsMod = false;

  obj_type Type = ObjectHelper_GetType(Params[0]);

  if (Type == obj_type::NONE)
    return "";

  Params.erase(Params.begin());

  lgt* L = nullptr;
  shape* Sh = nullptr;
  oper* Op = nullptr;
  mod* M = nullptr;

  switch (Type)
  {
  /* Lights */
  case parser::obj_type::POINT:
    L = new point_lgt(Params);
    break;
  case parser::obj_type::DIR:
    L = new dir_lgt(Params);
    break;
  case parser::obj_type::SPOT:
    L = new spot_lgt(Params);
    break;
  /* Shapes */
  case parser::obj_type::SPHERE:
    Sh = new sphere(Params);
    break;
  case parser::obj_type::BOX:
    Sh = new box(Params);
    break;
  case parser::obj_type::PLANE:
    Sh = new plane(Params);
    break;
  case parser::obj_type::ELLIPSOID:
    Sh = new ellipsoid(Params);
    break;
  case parser::obj_type::TORUS:
    Sh = new torus(Params);
    break;
  case parser::obj_type::CYLINDER:
    Sh = new cylinder(Params);
    break;
  case parser::obj_type::CAPSULE:
    Sh = new capsule(Params);
    break;
  case parser::obj_type::HOLLOW_SPHREE:
    Sh = new hollow_sphere(Params);
    break;
  /* Modifications */
  case parser::obj_type::ROTATE:
    M = new rotate(Params);
    break;
  case parser::obj_type::TRANSLATE:
    M = new translate(Params);
    break;
  case parser::obj_type::SCALE:
    M = new scale(Params);
    break;
  /* Operations */
  case parser::obj_type::INTER:
    Op = new inter(Params);
    break;
  case parser::obj_type::DIF:
    Op = new dif(Params);
    break;
  case parser::obj_type::UNION:
    Op = new uni(Params);
    break;
  case parser::obj_type::SMTH_INTER:
    Op = new smth_inter(Params);
    break;
  case parser::obj_type::SMTH_DIF:
    Op = new smth_dif(Params);
    break;
  case parser::obj_type::SMTH_UNION:
    Op = new smth_uni(Params);
    break;
  default:
    return "";
  }

  if (L != nullptr)
  {
    if (L->IsInited())
    {
      if (Lgts.find(Var) == Lgts.end())
        Lgts.emplace(Var, L);
      else
      {
        delete Lgts[Var];
        Lgts[Var] = L;
      }
      return "light";
    }
    delete L;
    return "";
  }

  if (Sh != nullptr)
  {
    std::string s;
    if (Sh->IsInited())
    {
      s = Sh->GetStr(Var);
      s += Sh->GetMtl(Var);
      int p = ObjectHelper_AddTex(Sh->Tex);

      if (p != -1)
      {
        s += Sh->GetTex(Var);
        s += Sh->ApplyTex(Var, p);
      }
    }
    delete Sh;
    return s;
  }

  if (Op != nullptr)
  {
    if (Params.size() < 2 || !ObjectHelper_CheckOperParams({ Params[0], Params[1] }))
      IsFailed = true;
    std::string s;
    if (!IsFailed && Op->IsInited())
    {
      s = Op->CheckParamsMtl(Var);
      s += Op->GetStr(Var);
    }
    delete Op;
    return s;
  }

  if (M != nullptr)
  {
    std::string s;
    if (Params.size() == 0 || !ObjectHelper_CheckOperParams({ Params[0] }))
      IsFailed = true;
    if (!IsFailed && M->IsInited())
    {
      //std::vector<std::string> P = ObjectHelper_GetParametrs(Voc[Params[0]]);
    
      s = M->GetStr(Var);
      // s += ObjectHelper_GetStr(Var, P, IsMod);
      IsMod = true;
    }
    delete M;
    return s;
  }

  return "";
} /* End of 'parser::ObjectHelper_GetStr' function */

/* Get result string by variable name function.
 * ARGUMENTS:
 *   - variable:
 *       const std::string& Var;
 *   - flag is modifiers:
 *       bool &IsMod;
 * RETURNS:
 *  (std::string) result string.
 */
std::string parser::GetStrByVar(const std::string &Var, bool &IsMod)
{
  std::vector<std::string> V = ObjectHelper_GetParametrs(Voc[Var]);

  return ObjectHelper_GetStr(Var, V, IsMod);
} /* End of 'parser::ObjectHelper_GetStrByVar' function */

/* Get result string by variable name and description function.
 * ARGUMENTS:
 *   - variable and description:
 *       const std::string& Var, Desc;
 *   - flag is modifiers:
 *       bool &IsMod;
 * RETURNS:
 *  (std::string) result string.
 */
std::string parser::GetStrByVarDesc(const std::string& Var, const std::string& Desc, bool &IsMod)
{
  std::vector<std::string> V = ObjectHelper_GetParametrs(Desc);

  return ObjectHelper_GetStr(Var, V, IsMod);
} /* End of 'parser::ObjectHelper_GetStrByVarDesc' function */