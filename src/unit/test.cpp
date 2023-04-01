/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : test.cpp
  * PURPOSE     : Ray marching project.
  *               Test module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include "../animation/animation.h"
#include "../def.h"

namespace trm
{ 
  class test_unit : public unit
  {
  private:
    primitive* FullScreen;
    texture *Skybox;
  public:
    test_unit( animation* Ani )
    {
      material* M = Ani->AddMaterial("RT", "RT");
      FullScreen = Ani->CreatePrim(topology::base<vertex::point>(prim_type::POINTS, { vertex::point(vec3(0)) }), M);
      Skybox = new texture();
      Skybox->LoadCube("skyboxes/SkyLight", 0);
    }
    ~test_unit( VOID )
    {
      delete Skybox;
      trm::animation::GetPtr()->Free(FullScreen);
    }
    VOID Response( animation *Ani ) override
    {
      for (auto tex : Ani->Textures)
      {
        INT loc;
        CHAR tname[] = "IsTexture0";

        tname[9] = '0' + tex.second.n;
        if (tex.second.Tex != nullptr)
        {
          /* Activate sampler */
          glActiveTexture(GL_TEXTURE0 + tex.second.n);
          /* Bind texture to sampler */
          glBindTexture(GL_TEXTURE_2D, tex.second.Tex->GetId());

          INT val = tex.second.Tex->GetId() != -1;
          if ((loc = glGetUniformLocation(FullScreen->Material->Shader->GetPrgId(), tname)) != -1)
            glUniform1i(loc, val);
        }
      }

      if (Skybox != nullptr)
      {
        /* Activate sampler */
        glActiveTexture(GL_TEXTURE0);
        /* Bind texture to sampler */
        glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox->GetId());
      }
    }
    VOID Render( animation *Ani ) override
    {
      Ani->Draw(FullScreen);
    }
  };
  static animation::unit_register<test_unit> _("Test");
}

/* END OF 'test.cpp' FILE */