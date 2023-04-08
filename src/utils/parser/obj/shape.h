/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : shape.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 08.04.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __shape_h_
#define __shape_h_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <format>
#include <optional>

#include "param.h"
#include "../../../animation/render/resource/texture.h"

namespace parser
{
  namespace obj
  {
    class shape
    {
    private:
      static int AddTex(const std::string& Name)
      {
        int s = (int)Name.size();
        std::string
          ext = Name.substr(s - 4, 3),
          name = Name.substr(1, s - 5),
          res_name = name + ".g32",
          tmp_name = name + "." + ext;
        if (Textures.find(res_name) != Textures.end())
		{
          return Textures[res_name].n;
		}
		else
		{
          if (ext != "g32")
          {
            std::string fmt = std::format("utils\\ANY2ANY.exe {} {}", "bin//images//" + tmp_name, "bin//images//" + res_name);
            system(fmt.c_str());
          }

		  Textures.emplace(res_name, CountOfTex);
		  return CountOfTex++;
		}
      }

      static std::map<std::string, trm::texture::tex_data>& Textures;
      static int CountOfTex;

    public:
      enum class type
      {
        eSphere,
        eBox,
        eCylinder,
        eCapsule,
        ePlane,
        eTorus,
        eEllipsoid,
        eWater
      };

      static const std::map<std::string, type> Table;

      static const std::map<type, std::vector<param::type>> Types;

      static const std::map<type, std::function<std::string(std::string, std::vector<std::string>, bool)>> ToStr;

      static std::string GetTexStr(void);

      static void ClearTextures( void )
      {
        Textures.clear();
        CountOfTex = 1;
      }
    };
  }
}

#endif 

/* END OF 'shape.h' FILE */