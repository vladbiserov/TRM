/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : texture.cpp
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


#include "texture.h"

/* Texture load from *.G24 or *.G32 file function.
 * ARGUMENTS:
 *   - texture name:
 *       const std::string &FileName;
 *   (texture &) Self-reference.
 */
trm::texture & trm::texture::Load( const std::string &FileName, INT Id )
{
  WORD w = 0, h = 0;
  reader rd("bin/images/" + FileName);
  if (!rd.is_open())
  {
    TexId = 0;
    return *this;
  }
  Name = FileName;
  
  rd(&w);
  rd(&h);
  W = w;
  H = h;

  INT C = 0;
  if (rd.length - 4 == W * H * 3)
    C = 3;
  else if (rd.length - 4 == W * H * 4)
    C = 4;
  else
    C = 1;
  BYTE *Bits = new BYTE[W * H * C];

  rd(Bits, W * H * C);

  Create(Bits, C, W, H, Id, FileName);

  delete[] Bits;
  return *this;
} /* End of 'trm::texture::Load' function */


/*
 */
trm::texture & trm::texture::LoadCube( const std::string &FileName, INT Id )
{
  INT i = 0;
  WIN32_FIND_DATAA Files[6] {};

  for (auto &Elm : {"/PosX.*", "/NegX.*", "/PosY.*", "/NegY.*", "/PosZ.*", "/NegZ.*"})
  {
    if (FindFirstFileA(("bin/images/" + FileName + Elm).c_str(), Files + i) == INVALID_HANDLE_VALUE)
      return *this;

    i++;
  }

  std::string Paths[6];

  for (INT i = 0; i < 6; i++)
    Paths[i] = "bin/images/" + FileName + "/" + Files[i].cFileName;

  /* Allocate texture space */
  glGenTextures(1, &TexId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, TexId);

  /* Upload texture */
  ///gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);

  /* First side */
  std::vector<DWORD> Bulk {};

  {
    reader rd(Paths[0]);
    if (!rd.is_open())
    {
      TexId = 0;
      return *this;
    }

    WORD w = 0, h = 0;
    rd(&w);
    rd(&h);
    W = w;
    H = h;

    Bulk.resize(W * H);

    if (rd.length - 4 == W * H * 3)
      for (INT i = 0; i < W * H; i++)
        rd((BYTE *)(Bulk.data() + i), 3), Bulk[i] |= 0xFF'00'00'00;
    else if (rd.length - 4 == W * H * 4)
      rd(Bulk.data(), W * H);
    else
      for (INT i = 0; i < W * H; i++)
        rd((BYTE *)(Bulk.data() + i), 1), Bulk[i] |= 0xFF'00'00'00 | (Bulk[i] << 16) | (Bulk[i] << 8);
  }

  FLOAT mips;

  mips = W > H ? H : W;
  mips = log(mips) / log(2);
  if (mips < 1)
    mips = 1;
  else
    mips = ceilf(mips);

  INT MipsI = (INT)mips;

  INT Sides[6]
  {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };

  glTexStorage2D(GL_TEXTURE_CUBE_MAP, MipsI, GL_RGBA8, W, H);
  glTexSubImage2D(Sides[0], 0, 0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Bulk.data());

  /* Other sides */
  for (INT i = 1; i < 6; i++)
  {
    reader rd(Paths[i]);

    if (rd.is_open())
    {
      WORD w = 0, h = 0;
      rd(&w);
      rd(&h);

      if (rd.length - 4 == (INT)w * (INT)h * 3)
        for (INT i = 0; i < Bulk.size(); i++)
          rd((BYTE *)(Bulk.data() + i), 3), Bulk[i] |= 0xFF'00'00'00;
      else if (rd.length - 4 == (INT)w * (INT)h * 4)
        rd(Bulk.data(), Bulk.size());
      else
        for (INT i = 0; i < Bulk.size(); i++)
          rd((BYTE *)(Bulk.data() + i), 1), Bulk[i] |= 0xFF'00'00'00 | (Bulk[i] << 16) | (Bulk[i] << 8);

      glTexSubImage2D(Sides[i], 0, 0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Bulk.data());
    }
  }

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  return *this;
} /*  */

/* Create texture function.
 * ARGUMENTS:
 *   - image data:
 *       VOID *Bits;
 *   - G32 or G24 tag:
 *       INT tag;
 * RETURNS:
 *   (texture &) Self-reference.
 */
trm::texture & trm::texture::Create( VOID *Bits, INT tag, INT Wi, INT He, INT Id, const std::string &ImgName )
{
  W = Wi;
  H = He;
  TexId = Id;
  Name = ImgName;

  FLOAT mips;

  mips = W > H ? H : W;
  mips = log(mips) / log(2);
  if (mips < 1)
    mips = 1;

  /* Allocate texture space */
  glGenTextures(1, &TexId);
  glBindTexture(GL_TEXTURE_2D, TexId);

  /* Upload texture */
  ///gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glTexStorage2D(GL_TEXTURE_2D, mips, tag == 3 ? GL_RGB8 : tag == 4 ? GL_RGBA8 : GL_R8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return *this;
} /* End of 'trm::texture::Create' function */

/* Create texture function.
 * ARGUMENTS:
 *   - shader name:
 *       const std::string &TexFileNamePrefix;
 * RETURNS:
 *   (texture *) Pointer to texture.
 */
trm::texture * trm::texture_manager::CreateTexture( const std::string &TexFileNamePrefix )
{
  texture *find = {};

  if ((find = Find(TexFileNamePrefix)) != nullptr)
    return find;

  return Add(texture().Load(TexFileNamePrefix, static_cast<INT>(Stock.size() + 1)));
} /* End of 'trm::texture_manager::CreateTexture' function */

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
trm::texture * trm::texture_manager::CreateModelTexture( VOID *Bits, INT tag, INT W, INT H, const std::string &ImgName )
{
  texture *find = {};
  
  if ((find = Find(ImgName)) != nullptr)
    return find;

  return Add(texture().Create(Bits, tag, W, H, static_cast<INT>(Stock.size() + 1), ImgName));
} /* End of 'trm::texture_manager::CreateModelTexture' function */

/* END OF 'texture.cpp' FILE */