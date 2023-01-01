/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : opers.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __opers_h_
#define __opers_h_

#include "objects.h"

/* Union operation class */
class uni : public oper
{
public:
  /* Class constructor */
  uni(std::vector<std::string> Params) : oper(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
  } /* End of 'uni' function */

  /* Class destructor */
  ~uni(void) override
  {
  } /* End of '~uni' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFUnion({}, {});\n", Val, P1, P2) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'uni' class */

/* Intersection operation class */
class inter : public oper
{
public:
  /* Class constructor */
  inter(std::vector<std::string> Params) : oper(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
  } /* End of 'inter' function */

  /* Class destructor */
  ~inter(void) override
  {
  } /* End of '~inter' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFInter({}, {});\n", Val, P1, P2) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'inter' class */

/* Difference operation class */
class dif : public oper
{
public:
  /* Class constructor */
  dif(std::vector<std::string> Params) : oper(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
  } /* End of 'dif' function */

  /* Class destructor */
  ~dif(void) override
  {
  } /* End of '~dif' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFDifer({}, {});\n", Val, P1, P2) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'dif' class */

/* Smooth union operation class */
class smth_uni : public oper
{
public:
  /* Class constructor */
  smth_uni(std::vector<std::string> Params) : oper(3)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
    k = Params[2];
  } /* End of 'smth_uni' function */

  /* Class destructor */
  ~smth_uni(void) override
  {
  } /* End of '~smth_uni' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFUnionSmooth({}, {}, {});\n", Val, P1, P2, k) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'smth_uni' class */

/* Smooth intersection operation class */
class smth_inter : public oper
{
public:
  /* Class constructor */
  smth_inter(std::vector<std::string> Params) : oper(3)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
    k = Params[2];
  } /* End of 'smth_inter' function */

  /* Class destructor */
  ~smth_inter(void) override
  {
  } /* End of '~smth_inter' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFInterSmooth({}, {}, {});\n", Val, P1, P2, k) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'smth_inter' class */

/* Smooth difference operation class */
class smth_dif : public oper
{
public:
  /* Class constructor */
  smth_dif(std::vector<std::string> Params) : oper(3)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
    k = Params[2];
  } /* End of 'smth_dif' function */

  /* Class destructor */
  ~smth_dif(void) override
  {
  } /* End of '~smth_dif' function */

  /* Get operation description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{} = SDFDiferSmooth({}, {}, {});\n", Val, P1, P2, k) + GetMtl(Val, P1, P2);
  } /* End of 'GetStr' function */
}; /* End of 'smth_dif' class */

#endif /* __opers_h_ */

/* END OF 'opers.h' FILE */