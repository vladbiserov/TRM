/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : mods.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __mods_h_
#define __mods_h_

#include "objects.h"

/* Rotation modification */
class rotate : public mod
{
private:
  std::string N, A; // Normal and angle
public:
  /* Class constructor */
  rotate(std::vector<std::string> Params) : mod(3)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Var = Params[0];
    N = Params[1];
    A = Params[2];
  } /* End of 'rotate' function */

  /* Class destructor */
  ~rotate(void) override
  {
    N.clear();
    A.clear();
  } /* End of '~rotate' function */

  /* Get modification description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("mod_{0} = Rotate({1}, {2}, mod_{0});\n", Val, A, N);
  } /* End of 'GetStr' function */
}; /* End of 'rotate' calss */

/* Translation modification */
class translate : public mod
{
private:
  std::string P; // New position
public:
  /* Class constructor */
  translate(std::vector<std::string> Params) : mod(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Var = Params[0];
    P = Params[1];
  } /* End of 'translate' function */

  /* Class destructor */
  ~translate(void) override
  {
    P.clear();
  } /* End of '~translate' function */

  /* Get modification description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("mod_{0} = Translate({1}, mod_{0});\n", Val, P);
  } /* End of 'GetStr' function */
}; /* End of 'translate' calss */

/* Scale modification */
class scale : public mod
{
private:
  std::string S; // New size
public:
  /* Class constructor */
  scale(std::vector<std::string> Params) : mod(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Var = Params[0];
    S = Params[1];
  } /* End of 'scale' function */

  /* Class destructor */
  ~scale(void) override
  {
    S.clear();
  } /* End of '~scale' function */

  /* Get modification description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("mod_{0} = Scale({1}, mod_{0});\n", Val, S);
  } /* End of 'GetStr' function */
}; /* End of 'scale' calss */

#endif /* __mods_h_ */

/* END OF 'mods.h' FILE */