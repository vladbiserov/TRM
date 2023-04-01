/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : texture.h
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


#ifndef __texture_h_
#define __texture_h_

#include <vector>
#include <fstream>
#include "../../../utils/reader.h"
#include "../../../def.h"
#include "resource.h"

/* NSF name space */
namespace trm
{
    //std::map<std::string, tex_data> Textures;
  /* Material class */
  class texture
  {
    friend class texture_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class render;
    friend class material;
  private:
    UINT TexId;
    INT W, H;
  public:
    std::string Name;

    /* Structure which save information about texture */
    struct tex_data
    {
      INT n;
      texture *Tex;
    }; // End of 'tex_data' struct

    /* Constructor */
    texture( VOID ) : Name {}, TexId(0), W(0), H(0)
    {
    }

    VOID Free( VOID )
    {
    }

    UINT GetId( VOID )
    {
      return TexId;
    }

    /*
     */
    texture & LoadCube( const std::string &FileName, INT Id );
  private:
    /* Texture load from *.G24 or *.G32 file function.
     * ARGUMENTS:
     *   - texture name:
     *       const std::string &FileName;
     * RETURNS:
     *   (texture &) Self-reference.
     */
    texture & Load( const std::string &FileName, INT Id );

    /* Create texture function.
     * ARGUMENTS:
     *   - image data:
     *       VOID *Bits;
     *   - G32 or G24 tag:
     *       INT tag;
     * RETURNS:
     *   (texture &) Self-reference.
     */
    texture & Create( VOID *Bits, INT tag, INT Wi, INT He, INT Id, const std::string &ImgName );
  }; /* End of 'texture' class */

  /* Material manager class */
  class texture_manager : public resource_manager<texture, std::string>
  {
    friend class render;
    friend class model;
    friend class target;
  private:
    /* Create texture function.
     * ARGUMENTS:
     *   - shader name:
     *       const std::string &TexName;
     *   - image data:
     *       VOID *Bits;
     *   - G32 or G24 tag:
     *       INT tag;
     *   - image size:
     *       INT W, H;
     *   - texture name:
     *       const std::string &TexName;
     * RETURNS:
     *   (texture *) Pointer to texture.
     */
    texture * CreateModelTexture( VOID *Bits, INT tag, INT W, INT H, const std::string &TexName );
  public:
    /* Create texture function.
     * ARGUMENTS:
     *   - shader name:
     *       const std::string &TexFileNamePrefix;
     * RETURNS:
     *   (texture *) Pointer to texture.
     */
    texture * CreateTexture( const std::string &TexFileNamePrefix );
  }; /* End of 'texture_manager' class */
} /* end of 'trm' namespace */

#endif /* __texture_h_ */

/* END OF 'texture.h' FILE */