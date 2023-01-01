/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : primitive.cpp
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

#include "primitive.h"

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - '*.OBJ' file name:
 *       const std::string &FileName;
 *   - pointer to material:
 *       const std::string &MtlName;
 * RETURNS:
 *   (primitive &) Self-reference.
 */
trm::primitive & trm::primitive::Load( const std::string &FileName, material *Mtl )
{
  INT
    noofv = 0,
    noofi = 0;
  FILE *F;
  CHAR Buf[1000];

  /* Open FILE */
  if ((F = fopen((std::string("bin/models/obj/") + FileName).c_str(), "r")) == NULL)
    return *this;

  /* Count vertex and index quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      noofv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      noofi++;
  }

  std::vector<INT> I(3 * noofi);
  std::vector<vertex::std> V(noofv);

  /* Read vertices and facets data */
  rewind(F);
  noofv = noofi = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      V[noofv++].P = vec3(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      /* Read one of possible facet references */
      auto z = sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      n1--;
      n2--;
      n3--;
      I[noofi++] = n1;
      I[noofi++] = n2;
      I[noofi++] = n3;
    }
  }

  trm::topology::trimesh<vertex::std> T(V, I);
  /* making an auto normalize */
  T.EvalNormals();

  trm::topology::base<vertex::std> *B = reinterpret_cast<trm::topology::base<vertex::std> *>(&T);
  /* create primitive */
  Create(*B, Mtl);

  fclose(F);
  return *this;
} /* End of 'trm::render::LoadPrim' function */

/* Free render primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::primitive::Free( VOID )
{
  if (VA != 0)
  {
    /* making an array of vertices active */
    glBindVertexArray(VA);
    /* unhooking the buffer */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBuf);
    /* making an array of vertices inactive */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VA);

    /* making an array of indexes inactive */
    glDeleteBuffers(1, &IBuf);
    VA = 0, VBuf = 0, IBuf = 0;
  }
  else
    VBuf = 0, IBuf = 0;
} /* End of 'Free' function */

/* Create primitive function.
 * ARGUMENTS:
 *   - '*.OBJ' file name:
 *       const std::string &FileName;
 *   - pointer to material:
 *       material *Mtl;
 * RETURNS:
 *   (primitive *) created primitive interface.
 */
trm::primitive * trm::primitive_manager::CreatePrim( const std::string &FileName, material *Mtl )
{
  return Add(primitive().Load(FileName, Mtl));
} /* End of 'trm::primitive_manager::CreatePrim' function */