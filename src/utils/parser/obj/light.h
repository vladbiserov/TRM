/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : light.h
 * PURPOSE     : Ray marching project.
 *               Parser module.
 * PROGRAMMER  : Vladislav Biserov.
 * LAST UPDATE : 30.03.2023
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __light_h_
#define __light_h_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <format>

#include "param.h"

namespace parser
{
  /* Object namespace */
  namespace obj
  {
    /* Light object */
    class light
    {
    private:
      static std::map<std::string, std::pair<std::string, bool>> Point, Spot, Dir;
      static int 
        PointCnt,
        SpotCnt,
        DirCnt;

    public:
      /* Light type */
      enum class type
      {
        ePoint,  // Point light
        eDir,    // Dir light
        eSpot,   // Spot light
      }; /* End of 'type' enum */

      /* Table of light's names */
      static const std::map<std::string, type> Table;

      static const std::map<type, std::vector<param::type>> Types;

      static const std::map<type, std::function<void(std::string, std::vector<std::string>)>> Add;

      static void Enable(const std::string &Name);

      static std::string GetStr(void);
    }; /* End of 'light' class */
  } /* end of 'obj' namespace */
}

#endif 

/* END OF 'light.h' FILE */