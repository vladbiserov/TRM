/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : primitive.h
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


#ifndef __primitive_h_
#define __primitive_h_

#include "resource.h"
#include "material.h"
#include "topology.h"
#include "../../../def.h"

/* NSF name space */
namespace trm
{
  /* Primitive handle class */
  class primitive
  {
    friend class primitive_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class render;
    friend class jittering;
    friend class test_unit;
  private:
    prim_type Type;     /* Primitive type */
    UINT VA;            /* Primitive vertex array */
    UINT VBuf;          /* Vertex buffer */
    UINT IBuf;          /* Index buffer */
    INT NumOfElements;  /* Number of elements for OpenGL */
    matr Transform;     /* Primitive transformation matrix */
    BOOL IsTrans;       /* Is matrix not identity */
    material *Material; /* Pointer to material */
 
  public:
    vec3
      Min, /* Minimal primitive position */
      Max; /* Maximal primitive position */
 
    /* Class constructor */
    primitive( VOID ) :
      VA(0), VBuf(0), IBuf(0),
      NumOfElements(0), Transform(), Min(0), Max(0),
      Type(trm::prim_type::MESH),
      Material {}, IsTrans(FALSE)
    {
    } /* End of 'primitive' function */

    VOID SetTrans( const matr &Tr )
    {
      Transform = Tr;
      IsTrans = TRUE;
    }
 
    /* Class destructor */
    ~primitive( VOID )
    {
    } /* End of '~primitive' function */
  private:
    /* Free render primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
 
    /* Primitive creation function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     *   - pointer to material:
     *       material *Mtl;
     * RETURNS:
     *   (primitive &) self reference.
     */
    template<typename vertex>
      primitive & Create( const topology::base<vertex> &T, material *Mtl )
      {
        Free();
        //if (Mtl == nullptr)
        //  Material = material::GetDefault;
        Material = Mtl;
        Type = T.Type;
        // Setup data order due to vertex description string
        const std::string dsc = vertex::Description;

        if (!T.V.empty() && T.V.size() != 0)
        {
          glGenBuffers(1, &VBuf);
          glGenVertexArrays(1, &VA);
 
          glBindVertexArray(VA);
          glBindBuffer(GL_ARRAY_BUFFER, VBuf);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * T.V.size(), T.V.data(), GL_STATIC_DRAW);
 
          INT attr = 0, offset = 0;
          for (auto c = dsc.begin(); c != dsc.end(); c++)
            if (*c == 'f' || *c == 'i')
            {
              CHAR type = *c++;
              INT n = *c - '0';
 
              if (n < 0 || n > 9)
                break;
              // Enable used attributes
              glEnableVertexAttribArray(attr);
              glVertexAttribPointer(attr, n, type == 'f' ? GL_FLOAT : GL_INT, FALSE,
                sizeof(vertex), reinterpret_cast<VOID *>(offset));
              offset += n * 4;
              attr++;
            }
            else
              break;
          glBindVertexArray(0);
        }
        if (!T.I.empty() && T.I.size() != 0)
        {
          glGenBuffers(1, &IBuf);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuf);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * T.I.size(), T.I.data(), GL_STATIC_DRAW);
          NumOfElements = static_cast<INT>(T.I.size());
        }
        else
          NumOfElements = static_cast<INT>(T.V.size());
        return *this;
      } /* End of 'Create' function */

      /* Load primitive from '*.OBJ' file function.
       * ARGUMENTS:
       *   - '*.OBJ' file name:
       *       const std::string &FileName;
       *   - pointer to material:
       *       material *Mtl;
       * RETURNS:
       *   (primitive &) Self-reference
       */
      primitive & Load( const std::string &FileName, material *Mtl );
  }; /* End of 'primitive' class */

  /* Primitive manager */
  class primitive_manager : public resource_manager<primitive, INT>
  {
  public:
    /* Create primitive function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     *   - poiinter to material:
     *       material *Mtl;
     * RETURNS:
     *   (primitive *) created primitive interface.
     */
    template<typename vertex>
      primitive * CreatePrim( const topology::base<vertex> &T, material *Mtl )
      {
        return Add(primitive().Create(T, Mtl));
      } /* End of 'CreatePrim' function */
  protected:
    /* Create primitive function.
     * ARGUMENTS:
     *   - '*.OBJ' file name:
     *       const std::string &FileName;
     *   - pointer to material:
     *       material *Mtl;
     * RETURNS:
     *   (primitive *) created primitive interface.
     */
    primitive * CreatePrim( const std::string &FileName, material *Mtl );
  }; /* End of 'primitive_manager' class */
} /* end of 'trm' namespace */

#endif /* __primitive_h_ */

/* END OF 'primitive.h' FILE */