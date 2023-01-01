/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : shader.cpp
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
#include "shader.h"

/* Class constructor.
 * ARGUMENTS:
 *   shader file name prefix:
 *     const std::string &FileNamePrefix;
 */
trm::shader::shader( const std::string &FileNamePrefix ) : Name(FileNamePrefix), ProgId(0)
{
} /* End of 'trm::shader::shader' function */

/* Get shader program id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (UINT) Shader program id.
 */
UINT trm::shader::GetPrgId( VOID )
{
  return ProgId;
} /* End of 'trm::shader::GetPrgId' function */

/* Store log to file function.
 * ARGUMENTS:
 *   - text 1 to save:
 *       const std::string &Stage;
 *   - text 2 to save:
 *       const std::string &Text;
 * RETURNS: None.
 */
VOID trm::shader::Log( const std::string &Stage, const std::string &Text )
{
  std::ofstream("bin/shaders/error.log", std::ios_base::app) <<
    Name << ":" << Stage << ":\n" << Text << "\n";
} /* End of 'trm::shader::Log' function */

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       const std::string &FileName;
 * RETURNS:
 *   (std::string) load text.
 */
std::string trm::shader::LoadTextFile( const std::string &FileName )
{
  std::ifstream f(FileName);

  return std::string((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
} /* End of 'trm::shader::LoadTextFile' function */

/* Load shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (shader &) Self-reference.
 */
trm::shader & trm::shader::Load( VOID )
{
  Free();
  struct
  {
    INT Type;          /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
    std::string Stuff; /* Shader file prefix name (e.g. "VERT") */
    INT Id;            /* Obtained shader Id from OpenGL */
  } shdr[] =
  {
    {GL_VERTEX_SHADER, "VERT", 0},
    {GL_TESS_CONTROL_SHADER, "CTRL", 0},
    {GL_TESS_EVALUATION_SHADER, "EVAL", 0},
    {GL_GEOMETRY_SHADER, "GEOM", 0},
    {GL_FRAGMENT_SHADER, "FRAG", 0},
  };
  INT res;
  CHAR Buf[1000];
  BOOL is_ok = TRUE;

  for (auto &s : shdr)
  {
    /* Create shader */
    if ((s.Id = glCreateShader(s.Type)) == 0)
    {
      Log(s.Stuff, "Error create shader");
      is_ok = FALSE;
      break;
    }
    /* Load shader text from FILE */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", const_cast<CHAR *>(Name.c_str()), const_cast<CHAR *>(s.Stuff.c_str()));
    std::string txt = LoadTextFile(Buf);
    if (txt.empty())
    {
      if (s.Stuff != "VERT" && s.Stuff != "FRAG")
      {
        glDeleteShader(s.Id);
        s.Id = 0;
        continue;
      }
      else
      {
        Log(s.Stuff, "Error create shader");
        is_ok = FALSE;
        break;
      }
    }
    
    /* Attach shader text to shader */
    const CHAR *Src[]= {txt.c_str()};
    glShaderSource(s.Id, 1, Src, NULL);

    /* Compile shader */
    glCompileShader(s.Id);
    glGetShaderiv(s.Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(s.Id, sizeof(Buf), &res, Buf);
      Log(s.Stuff, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create shader program */
  if (is_ok)
  {
    if ((ProgId = glCreateProgram()) == 0)
      is_ok = FALSE;
    else
    {
      /* Attach shaders to program */
      for (auto s : shdr)
        if (s.Id != 0)
          glAttachShader(ProgId, s.Id);
      /* Link shader program */
      glLinkProgram(ProgId);
      glGetProgramiv(ProgId, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(ProgId, sizeof(Buf), &res, Buf);
        Log("LINK", Buf);
        is_ok = FALSE;
      }
    }
  }

  if (!is_ok)
  {
    /* Delete all created shaders */
    for (auto s : shdr)
      if (s.Id != 0)
      {
        if (ProgId != 0)
          glDetachShader(ProgId, s.Id);
        glDeleteShader(s.Id);
      }
    /* Delete program */
    if (ProgId != 0)
      glDeleteProgram(ProgId);
    ProgId = 0;
  }
  return *this;
} /* End of 'trm::shader::Load' function */

/* Free shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::shader::Free( VOID )
{
  INT n;
  UINT shds[5];

  if (ProgId == 0)
    return;

  glGetAttachedShaders(ProgId, 5, &n, shds);

  for (auto s: shds)
  {
    glDetachShader(ProgId, s);
    glDeleteShader(s);
  }
  glDeleteProgram(ProgId);
} /* End of 'trm::shader::Free' function */

/* Update shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::shader::Update( VOID )
{
  Load();
} /* End of 'trm::shader::Update' function */

/* Apply shader function.
 * ARGUMENTS: None.
 * RETURNS: 
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL trm::shader::Apply( VOID )
{
  return glIsProgram(ProgId) ? (glUseProgram(ProgId), TRUE) : FALSE;
} /* End of 'trm::shader::Apply' function */

/* Create shader function.
 * ARGUMENTS:
 *   - shader name:
 *       const std::string &ShdFileNamePrefix;
 * RETURNS: 
 *   (shader *) Pointer to shader.
 */
trm::shader * trm::shader_manager::CreateShader( const std::string &ShdFileNamePrefix )
{
  shader *find = {};

  if ((find = Find(ShdFileNamePrefix)) != nullptr)
    return find;

  return Add(shader(ShdFileNamePrefix).Load());
} /* End of 'trm::shader_manager::CreateShader' function */

/* Update all shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::shader_manager::Update( VOID )
{
  for (auto &s : Stock)
    s.second.Update();
} /* End of 'trm::shader_manager::Update' function */

/* END OF 'shader.cpp' FILE */