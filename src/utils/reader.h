/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : render.h
  * PURPOSE     : Ray marching project.
  *               Reader module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "../def.h"

namespace trm
{
  /* Unstructured data file reader class */
  class reader
  {
  private:
    // Memory pointer reference
    BYTE *Mem, *Ptr;
    std::fstream f;
    BOOL IsOpen;
  public:
    INT length;
    /* Class constructor.
     * ARGUMENTS:
     *   - texture name:
     *       const std::string &FileName;
     */
    reader( const std::string &FileName ) :
      f(FileName, std::fstream::in | std::fstream::binary),
      Mem{}, Ptr{}, IsOpen(f.is_open()), length(0)
    {
      if (IsOpen)
      {
        f.seekg(0, std::fstream::end);
        length = (INT)f.tellg();
        f.seekg(0, std::fstream::beg);
        Mem = new BYTE[length];
        f.read((CHAR *)Mem, length);
        f.close();
        Ptr = Mem;
      }
    } /* End of 'rdr' function */

    BOOL is_open( VOID )
    {
      return IsOpen;
    }

    ~reader( VOID )
    {
      if (Mem != nullptr)
        delete[] Mem;
    }
 
    /* Read data function.
     * ARGUMENT:
     *   - read data pointer:
     *       type *Data;
     *   - read data count:
     *       INT Count;
     * RETURNS: None.
     */
    template<typename type>
     VOID operator()( type *Data, const INT Count = 1 )
        {
          if (Count == 1)
            *Data = *(type *)Ptr, Ptr += sizeof(type);
          else
            memcpy(Data, Ptr, sizeof(type) * Count), Ptr += sizeof(type) * Count;
        } /* End of 'operator()' function */
  }; /* End of 'rdr' class */
}