/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : object.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __object_h_
#define __object_h_

#include <vector>
#include <string>
#include <format>

#pragma once

/* Base object class */
class object
{
public:
  int ParamCount; // Count of object function parameters
  bool IsInit;    // Is object inited

  /* Object constructor */
  object(int ParCount) : ParamCount(ParCount), IsInit(false)
  {
  } /* End of 'object' function */

  /* Virtual object destructor */
  virtual ~object(void)
  {
  } /* End of '~object' function */

  /* Get object description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  virtual std::string GetStr(const std::string &Val) = 0;

  /* Check is object inited function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (BOOL) is inited.
   */
  bool IsInited(void)
  {
    return IsInit;
  } /* End of 'IsInited' function */
}; /* End of 'object' class */

/* Base shape class */
class shape : public object
{
public:
  std::string Mtl, Tex;  // Shape's material and texture

  /* Shape constructor */
  shape(int ParCount) : object(ParCount)
  {
  } /* End of 'shape' constructor */

  /* Get shape material function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) material.
   */
  std::string GetMtl(const std::string& Val)
  {
    return std::format("mtl_{0} = {1};\n", Val, Mtl);
  } /* End of 'GetMtl' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  virtual std::string GetTex(const std::string& Val) = 0;

  /* Apply shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   *   - texture index:
   *       const std::string &TexInd;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string ApplyTex(const std::string& Val, const int &TexInd)
  {
    return std::format("mtl_{0}.Kd = texture(Tex{1}, uv_{0}).bgr;\n", Val, TexInd);
  } /* End of 'ApplyTex' function */
}; /* End of 'shape' class */

/* Base light class */
class lgt : public object
{
public:
  /* Light type */
  enum lgt_type
  {
    POINT, DIR, SPOT
  }; /* End of 'lgt_type' enum */
  std::string Color, Pos, Dir; // Light's color, position and directioin

  /* Light constructor */
  lgt(int ParCount) : object(ParCount)
  {
  } /* End of 'lgt' function */

  /* Get light type function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (lgt_type) light type.
   */
  virtual lgt_type GetType(void) = 0;
}; /* End of 'lgt' class */

/* Base modification class */
class mod : public object
{
public:
  std::string Var; // Variable to apply modification

  /* Modification constructor */
  mod(int ParCount) : object(ParCount)
  {
  } /* End of 'mod' function */
}; /* End of 'mod' class */

/* Base operation class */
class oper : public object
{
public:
  std::string P1, P2, k; // Operation parameters and coeeficient

  /* Operation constructor */
  oper(int ParCount) : object(ParCount)
  {
  } /* End of 'oper' function */

  /* Check parameters material function.
   * ARGUMENTS:
   *   - result value:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) material.
   */
  std::string CheckParamsMtl(const std::string& Val)
  {
    if (Val == P1 || Val == P2)
      return std::format(
        "tmp = {0};\n"
        "tmp_mtl = mtl_{0};\n", Val);
    else
      return "";
  } /* End of 'CheckParamsMtl' function */

  /* Get result material function.
   * ARGUMENTS:
   *   - result value:
   *       const std::string &Val;
   *   - parameters:
   *       const std::string &P1, P2;
   * RETURNS:
   *  (std::string) material.
   */
  std::string GetMtl(const std::string& Val, const std::string& P1, const std::string& P2)
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
  } /* End of 'GetMtl' function */
}; /* End of 'oper' class */

#endif /* __object_h_ */

/* END OF 'object.h' FILE */