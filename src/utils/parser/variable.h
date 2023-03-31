/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : variable.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __variable_h_
#define __variable_h_

#include <map>
#include <string>

#include "token.h"

namespace parser
{
  /* Variable class */
  class variables
  {
  private:
    /* Variable daata structure */
    struct data
    {
      var_type Type;    // Variable type
      double Val;       // Returning value
      std::string Text; // After '='
    }; /* End of 'data' structure */

    static std::map<std::string, data> Table;
    static std::map<std::string, std::string> Shapes;

    variables(void)
    {
    }
  public:
    static bool IsFirst;

    static void Clear( void )
    {
      Shapes.clear();
      IsFirst = true;
      Table.clear();

      Table["PI"] = { var_type::eFloat, 3.1415 };
      Table["Time"] = { var_type::eFloat, 0 };
    }

    static data Get(std::string Name)
    {
      if (Table.find(Name) != Table.end())
        return Table[Name];
      throw std::exception("no such variable exists");
    } /* End of 'Get' function */

    static bool GetType(std::string Name, var_type* Type)
    {
      auto search = Table.find(Name);

      if (search == Table.end())
        return false;
      *Type = search->second.Type;
      return true;
    } /* End of 'GetType' function */

    static bool IsExists(std::string Name)
    {
      return Table.find(Name) != Table.end();
    } /* End of 'IsExists' function */

    /* Set variable function.
     * ARGUMENTS:
     *   - Key:
     *       const std::string Name;
     *   - Value:
     *       data val;
     * RETURNS: None.
     */
    static void Set(std::string Key, data Val)
    {
      Table[Key] = Val;
    } /* End of 'Set' function */

    /* Check is shape exists function.
     * ARGUMENTS:
     *   - shape name:
     *       const std::string Name;
     * RETURNS: (bool) is exists.
     */
    static bool IsShapeExists(const std::string& Name)
    {
      return Shapes.find(Name) != Shapes.end();
    } /* End of 'IsShapeExists' function */

    /* Get shape function.
     * ARGUMENTS:
     *   - shape name:
     *       const std::string Name;
     * RETURNS: (std::string) Name.
     */
    static std::string GetShape(const std::string& Name)
    {
      if (Shapes.find(Name) != Shapes.end())
        return Shapes[Name];
      throw std::exception("no such shape exists");
    } /* End of 'GetShape' function */

    /* Set shape function.
     * ARGUMENTS:
     *   - Key, value:
     *       const std::string Key, Val;
     * RETURNS: None.
     */
    static void SetShape(const std::string& Key, const std::string& Val)
    {
      Shapes[Key] = Val;
    } /* End of 'SetShape' function */
  }; /* End of 'variable' class */
}

#endif

/* END OF 'variable.h' FILE */