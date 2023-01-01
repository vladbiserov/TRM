/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : material.cpp
  * PURPOSE     : Ray marching project.
  *               Animation module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include <fstream>
#include "material.h"
#include "../../animation.h"

/* Creation function.
 * ARGUMENTS:
 *   - name of material:
 *       const std::string &MtlName;
 *   - pointer to shader:
 *       shader *Sh;
 *   - texture:
 *       texture *Tex;
 * RETURNS:
 *   (material &) Self-reference.
 */
trm::material &trm::material::Create( const std::string &MtlName, shader *Sh, texture *Tex )
{
  Name = MtlName;
  Shader = Sh;
  this->Tex[0] = Tex;

  return *this;
} /* End of 'trm::material::Create' function */

/* Apply material function.
 * ARGUMENTS: None.
 * RETURNS:None.
 */
VOID trm::material::Apply( VOID )
{
  /* Set shader program Id */
  BOOL p = Shader->Apply();

  for (INT i = 0; i < 8; i++)
  {
    INT loc;
    CHAR tname[] = "IsTexture0";

    tname[9] = '0' + i;
    if (this->Tex[i] != nullptr)
    {
      /* Activate sampler */
      glActiveTexture(GL_TEXTURE0 + i);
      /* Bind texture to sampler */
      glBindTexture(GL_TEXTURE_2D, this->Tex[i]->TexId);

      INT val = Tex[i]->TexId != -1;
      if ((loc = glGetUniformLocation(Shader->ProgId, tname)) != -1)
        glUniform1i(loc, val);
      return;
    }
  }
} /* End of 'trm::material::Apply' function */

/* END OF 'material.cpp' FILE */