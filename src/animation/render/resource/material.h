/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : material.h
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


#ifndef __material_h_
#define __material_h_

#include <initializer_list>
#include "resource.h"
#include "shader.h"
#include "texture.h"
//#include "material_pattern.h"
#include "buffers.h"
#include "../../../def.h"

namespace trm
{
  class material
  {
    friend class material_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class render;
  private:
    /* Free material function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      for (INT i = 0; i < 8; i++)
        if (Tex[i] != nullptr)
          Tex[i]->Free();
    } // End of 'Free' function

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
    material & Create( const std::string &MtlName, shader *Sh, texture *Tex = nullptr );

  public:
    shader *Shader;   /* Shader */
    texture* Tex[8];  /* Textures */
    std::string Name; /* Material name */

    /* Constructor */
    material( VOID ) :
      Shader {}
    {
      ZeroMemory(Tex, sizeof(Tex));
    } /* End of constructor */

    /* Apply material function.
     * ARGUMENTS: None.
     * RETURNS:None.
     */
    VOID Apply( VOID );

    /* Denstructor */
    ~material( VOID )
    {
    }
  }; /* End of 'material' class */

  /* Material manager class */
  class material_manager : public resource_manager<material, std::string>
  {
    friend class model;
  protected:
    /* Create material function.
     * ARGUMENTS:
     *   - name of material:
     *       const std::string &MtlName;
     *   - pointer to shader:
     *       shader *Sh;
     *   - texture:
     *       texture *Tex;
     * RETURNS: 
     *   (material *) Pointer to material.
     */
    material * CreateMaterial( const std::string &MtlName, shader *Sh, texture *Tex = nullptr )
    {
      return Add(material().Create(MtlName, Sh, Tex));
    } /* End of 'CreateMaterial' function */
  };
} /* end of 'trm' namespace */

#endif // __material_h_

/* END OF 'material.h' FILE */
