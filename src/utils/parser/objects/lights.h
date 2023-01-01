/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : lights.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __lights_h_
#define __lights_h_

#include "objects.h"

/* Point light class */
class point_lgt : public lgt
{
public:
  /* Class constructor */
  point_lgt(std::vector<std::string> Params) : lgt(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Pos = Params[0];
    Color = Params[1];
  } /* End of 'point_lgt' function */

  /* Class destructor */
  ~point_lgt(void) override
  {
  } /* End of '~point_lgt' function */

  /* Get light description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("point_light({0},{1}, 0.4, 0.6, 0.06)", Pos, Color);
  } /* End of 'GetStr' function */

  /* Get light type function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (lgt_type) light type.
   */
  lgt_type GetType(void) override
  {
    return lgt::POINT;
  } /* End of 'GetType' function */
}; /* End of 'point_lgt' class */

/* Dir light class */
class dir_lgt : public lgt
{
public:
  /* Class constructor */
  dir_lgt(std::vector<std::string> Params) : lgt(2)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Dir = Params[0];
    Color = Params[1];
  } /* End of 'dir_lgt' function */
  
  /* Class destructor */
  ~dir_lgt(void) override
  {
  } /* End of '~dir_lgt' function */

  /* Get light description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("dir_light({}, {})", Dir, Color);
  } /* End of 'GetStr' function */

  /* Get light type function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (lgt_type) light type.
   */
  lgt_type GetType(void) override
  {
    return lgt::DIR;
  } /* End of 'GetType' function */
}; /* End of 'dir_lgt' class */

/* Spot light class */
class spot_lgt : public lgt
{
public:
  /* Class constructor */
  spot_lgt(std::vector<std::string> Params) : lgt(3)
  {
    if ((int)Params.size() != ParamCount)
      return;
    IsInit = true;
    Pos = Params[0];
    Dir = Params[1];
    Color = Params[2];
  } /* End of 'spot_lgt' function */

  /* Class destructor */
  ~spot_lgt(void) override
  {
  } /* End of '~spot_lgt' function */

  /* Get light description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("spot_light({}, {}, {}, cos(0.1), cos(1.1))", Pos, Dir, Color);
  } /* End of 'GetStr' function */

  /* Get light type function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (lgt_type) light type.
   */
  lgt_type GetType(void) override
  {
    return lgt::SPOT;
  } /* End of 'GetType' function */
}; /* End of 'spot_lgt' class */

#endif /* __lights_h_ */

/* END OF 'lights.h' FILE */