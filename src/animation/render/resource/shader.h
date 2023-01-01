/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : shader.h
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


#ifndef __shader_h_
#define __shader_h_

#include "resource.h"
#include "../../../def.h"

namespace trm
{
  class shader
  {
    friend class shader_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class render;
    friend class material;
    friend class jittering;
  private:
    UINT ProgId;

    /* Load text from file function.
     * ARGUMENTS:
     *   - file name to be load from:
     *       const std::string &FileName;
     * RETURNS:
     *   (std::string) load text.
     */
    std::string LoadTextFile( const std::string &FileName );

    /* Load shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (shader &) Self-reference.
     */
    shader & Load( VOID );

    /* Store log to file function.
     * ARGUMENTS:
     *   - text 1 to save:
     *       const std::string &Stage;
     *   - text 2 to save:
     *       const std::string &Text;
     * RETURNS: None.
     */
    VOID Log( const std::string &Stage, const std::string &Text );

    /* Free shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
  public:
    /* Shader name */
    std::string Name;

    /* Default constructor */
    shader( VOID ) : Name(), ProgId(0)
    {
    }

    /* Class constructor.
     * ARGUMENTS:
     *   shader file name prefix:
     *     const std::string &FileNamePrefix;
     */
    shader( const std::string &FileNamePrefix );

    /* Default destructor */
    ~shader( VOID )
    {
    }

    /* Update shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

    /* Apply shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL Apply( VOID );

    /* Get shader program id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (UINT) Shader program id.
     */
    UINT GetPrgId( VOID );

    /* Shader uniform value set function.
     * ARGUMENTS:
     *   - uniform name:
     *       const CHAR *Name;
     *   - uniform value:
     *       const value_type &Value;
     *   - uniform location:
     *       INT Loc;
     * RETURNS: None.
     */
    template<typename value_type>
    VOID UniSet( const CHAR *Name, const value_type &Value, INT Loc = -1 )
    {
      INT loc = Loc;
 
      if (loc == -1)
        loc = glGetUniformLocation(ProgId, Name);
      if (loc != -1)
        if constexpr (std::is_same_v<FLT, value_type>)
          glUniform1f(loc, Value);
        else if constexpr (std::is_same_v<INT, value_type>)
          glUniform1i(loc, Value);
        else if constexpr (std::is_same_v<vec3, value_type>)
          glUniform3fv(loc, 1, Value);
        else if constexpr (std::is_same_v<matr, value_type>)
          glUniformMatrix4fv(loc, 1, FALSE, Value);
    } /* End of 'UniSet' function */
  }; /* End of 'shader' class */

  /* Shader manager class */
  class shader_manager : public resource_manager<shader, std::string>
  {
    friend class material;
  public:
  protected:
    /* Create shader function.
     * ARGUMENTS:
     *   - shader name:
     *       const std::string &ShdFileNamePrefix;
     * RETURNS: 
     *   (shader *) Pointer to shader.
     */
    shader * CreateShader( const std::string &ShdFileNamePrefix );
    /* Update all shaders function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );
  };
} /* end of 'trm' namespace */

#endif // __shader_h_

/* END OF 'shader.h' FILE */
