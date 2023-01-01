/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : topology.h
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


#ifndef __topology_h_
#define __topology_h_

#include <vector>

#include "../../../def.h"

/* Project namespace */
namespace trm
{
  class primitive;

  /* Primitive shape representation type */
  enum struct prim_type
  {
    MESH,     /* Triangle mesh - array of triangles */
    STRIP,    /* Triangle strip - array of stripped triangles */
    LINES,    /* Line segments (by 2 points) */
    POINTS,   /* Arrauy of points */
    PATH,     /* path of points */
  };
 
  /* Topology structures namespase */
  namespace topology
  {
    /* Base topology class */
    template<typename vertex_type>
      class base
      {
        friend class primitive;
        friend class primitive_manager;
      protected:
        prim_type Type = prim_type::MESH;
 
        /* Vertex array */
        std::vector<vertex_type> V;
        /* Index array */
        std::vector<INT> I;
 
      public:
        /* Class default constructor */
        base( VOID )
        {
        } /* End of 'base' function */
 
        /* Class constructor.
         * ARGUMENTS:
         *   - primitive type:
         *       prim_type NewType;
         *   - vertex array:
         *       const std::vector<vertex_type> &NewV;
         *   - index array:
         *       const std::vector<INT> &NewI;
         */
        base( prim_type NewType,
              const std::vector<vertex_type> &NewV = {},
              const std::vector<INT> &NewI = {} ) : Type(NewType), V(NewV), I(NewI)
        {
        } /* End of 'base' function */
      };
 
    /* Base topology class */
    template<typename vertex_type>
      class trimesh : public base<vertex_type>
      {
      public:
        /* Class default constructor */
        trimesh( VOID )
        {
        } /* End of 'trimesh' function */
 
        /* Class constructor.
         * ARGUMENTS:
         *   - vertex array:
         *       const std::vector<vertex> &NewV;
         *   - index array:
         *       const std::vector<INT> &NewI;
         */
        trimesh( const std::vector<vertex_type> &NewV,
                 const std::vector<INT> &NewI = {} ) : base<vertex_type>(prim_type::MESH, NewV, NewI)
        {
        } /* End of 'trimesh' function */
 
        /* Evaluate trimesh vertex normals function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID EvalNormals( VOID )
        {
          for (INT i = 0; i < base<vertex_type>::I.size(); i += 3)   // may be i = 3
          {
            vec3 N = vec3((base<vertex_type>::V[base<vertex_type>::I[i + 1]].P -
              base<vertex_type>::V[base<vertex_type>::I[i]].P) %
                (base<vertex_type>::V[base<vertex_type>::I[i + 2]].P -
                  base<vertex_type>::V[base<vertex_type>::I[i]].P)).Normalized();

            base<vertex_type>::V[base<vertex_type>::I[i + 0]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 0]].N + N).Normalized();
            base<vertex_type>::V[base<vertex_type>::I[i + 1]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 1]].N + N).Normalized();
            base<vertex_type>::V[base<vertex_type>::I[i + 2]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 2]].N + N).Normalized();
          }
        } /* End of 'EvalNormals' function */
      };

    template<typename vertex_type>
      class grid : public base<vertex_type>
      {
      protected:
        /* Grid size */
        INT W, H;
      public:
        /* Class default constructor */
        grid( VOID )
        {
        } /* End of 'grid' function */
 
        /* Class constructor.
         * ARGUMENTS:
         *   - grid size:
         *       INT NewW, NewH;
         */
        grid( INT NewW, INT NewH ) : W(NewW), H(NewH), base<vertex_type>(prim_type::STRIP)
        {
          //if ((Ind = malloc(((H - 1) * (W * 2 + 1) - 1) * sizeof(INT))) == NULL)

          for (INT i = 0; i < H - 1; i++)
          {
            for (INT j = 0; j < W; j++)
            {
              base<vertex_type>::I.emplace_back((i + 1) * W + j);
              base<vertex_type>::I.emplace_back((i + 0) * W + j);
            }
            if (i != H - 2)
              base<vertex_type>::I.emplace_back(-1);
          }
          base<vertex_type>::V.reserve(W * H);
        }
 
        /* Obtain grid width function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid width.
         */
        INT GetW( VOID ) const
        {
          return W;
        } /* End of 'GetW' function */
 
        /* Obtain grid height function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid height.
         */
        INT GetH( VOID ) const
        {
          return H;
        } /* End of 'GetH' function */
 
        /* Grid 2D array vertex access function.
         * ARGUMENTS:
         *   - row index:
         *       INT Row;
         * RETURNS:
         *   (vertex *) vertex row pointer.
         */
        vertex_type * operator[]( INT Row )
        {
          assert(Row >= 0 && Row < H);
          return &this->V[Row * W];
        } /* End of 'operator[]' function */
 
        /* Evaluate grid vertex normals function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID EvalNormals( VOID )
        {
          //. . .
        }
      };
 
  } /* end of 'topology' namespace */
 
  /* Vertex collection namespace */
  namespace vertex
  {
    /* Standard vertex representation type */
    struct std
    {
      vec3 P;  /* Vertex position */
      vec2 T;  /* Vertex texture coordinates */
      vec3 N;  /* Normal at vertex */
      vec4 C;  /* Vertex color */
 
      /* Vertex content description string */
      static const ::std::string Description;
 
      /* Class default constructor */
      std( VOID )
      {
      } /* End of 'std' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - new vertex position:
       *       const vec &NewP;
       *   - new vertex texture coordinates:
       *       const vec2 &NewT;
       *   - new vertex normal:
       *       const vec &NewN;
       *   - new vertex color:
       *       const vec4 &NewC;
       */
      std( const vec3 &NewP,
           const vec2 &NewT = vec2(0, 0),
           const vec3 &NewN = vec3(0, 1, 0),
           const vec4 &NewC = vec4(1) ) :
        P(NewP), T(NewT), N(NewN), C(NewC)
      {
      } /* End of 'std' function */
    };

    /* Standard vertex representation type */
    struct point
    {
      vec3 P;  /* Vertex position */
 
      /* Vertex content description string */
      static const ::std::string Description;
 
      /* Class default constructor */
      point( VOID )
      {
      } /* End of 'std' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - new vertex position:
       *       const vec &NewP;
       */
      point( const vec3 &NewP ) : P(NewP)
      {
      } /* End of 'point' function */
    };
 
  } /* end of 'vertex' namespace */
 
} /* end of 'trm' namespace */
 
#endif /* __topology_h_ */

/* END OF 'topology.h' FILE */