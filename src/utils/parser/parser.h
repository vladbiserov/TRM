/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : parser.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __parser_h_
#define __parser_h_

#include <string>
#include <map>
#include <vector>

#include "../../animation/render/resource/texture.h"
#include "error.h"
#include "objects/obj_def.h"

/* Parser class */
class parser
{
public:
  std::string SceneIn, ShaderIn, ShaderOut;
private:
  bool
    IsFileInit = false,  // Flag is file inited
    IsFailed = false;    // Flag is something went wrong
  std::string InBuf, OutBuf, PointLgtBuf, DirLgtBuf, SpotLgtBuf; // Different buffers
  std::map<std::string, std::string> Voc; // Parser vocabulary
  std::map<std::string, lgt *> Lgts;  // Lights
  error_log ErrLog;  // Error logger
  int TexCnt = 0,  // Texture count
    PointLgtCnt = 0, DirLgtCnt = 0, SpotLgtCnt = 0; // Lights count

  /* Texture */
  std::map<std::string, trm::texture::tex_data>& Textures; // Textures
  std::map<int, int> AddedTex; // TexId, Index
  int CountOfTexs = 0;  // Count if textures

  /* Object types */
  enum class obj_type 
  {
    /* Failed */
    NONE = 0,
    /* Shapes */
    SPHERE = -1, BOX = -2, PLANE = -3, TORUS = -4,
    ELLIPSOID = -5, CYLINDER = -6, CAPSULE = -7,
    HOLLOW_SPHREE = -8,
    /* Operations */
    INTER = 1, DIF = 2, UNION = 3,
    SMTH_INTER = 4, SMTH_DIF = 5, SMTH_UNION = 6,
    /* Modifications */
    ROTATE = 10, TRANSLATE = 11, SCALE = 12,
    /* Lights */
    POINT = 100, DIR = 101, SPOT = 102
  }; /* End of 'obj_type' enum */

  /* Identification map */
  std::map<std::string, obj_type> Objects
  {
    // shapes
    {"sphere", obj_type::SPHERE},
    {"box", obj_type::BOX},
    {"plane", obj_type::PLANE},
    {"torus", obj_type::TORUS},
    {"ellipsoid", obj_type::ELLIPSOID},
    {"cylinder", obj_type::CYLINDER},
    {"capsule", obj_type::CAPSULE},
    {"hollow_sphere", obj_type::HOLLOW_SPHREE},
    // lights
    {"point_light", obj_type::POINT},
    {"dir_light", obj_type::DIR},
    {"spot_light", obj_type::SPOT},
    // operations
    {"inter", obj_type::INTER},
    {"dif", obj_type::DIF},
    {"union", obj_type::UNION},
    // smooth operations
    {"smth_inter", obj_type::SMTH_INTER},
    {"smth_dif", obj_type::SMTH_DIF},
    {"smth_union", obj_type::SMTH_UNION},
    // modifications
    {"rotate", obj_type::ROTATE},
    {"translate", obj_type::TRANSLATE},
    {"scale", obj_type::SCALE},
  }; /* End of 'objects' map */

public:
  /* Parser constructor.
   * ARGUMENTS:
   *   - User scene:
   *       const std::string &ScIn;
   *   - Primary shader:
   *       const std::string &ScIn;
   * RETURNS:
   *  (std::string) description.
   */
  parser(const std::string &ScIn, const std::string& ShIn, const std::string &ShOut, const std::string& ErrName,
    std::map<std::string, trm::texture::tex_data>& Tex) :
    SceneIn(ScIn), ShaderIn(ShIn), ShaderOut(ShOut), ErrLog(ErrName), Textures(Tex)
  {
  }

  /* Read scene file function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (bool) if read or not.
   */
  bool ReadFile(void);

  /* Update shader function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (bool) if wrote or not.
   */
  bool WriteFile(void);

  /* Get expressions from file function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void GetExprs(void);

private:
  /* Parse variables function.
   * ARGUMENTS:
   *   - expression to parse:
   *       const std::string& Expr;
   * RETURNS:
   *  (bool) if parse or not.
   */
  bool ParseVars(const std::string& Expr);

  /* Parse scene add objects function.
   * ARGUMENTS:
   *   - expression to parse:
   *       const std::string& Expr;
   * RETURNS:
   *  (bool) if parse or not.
   */
  bool ParseAdd(const std::string& Expr);

  /* Clear buffers function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Clear(void)
  {
    InBuf.clear();
    OutBuf.clear();
    
    PointLgtBuf.clear();
    DirLgtBuf.clear();
    SpotLgtBuf.clear();

    Voc.clear();

    IsFileInit = false;
    IsFailed = false;

    TexCnt = 0;
    PointLgtCnt = 0;
    DirLgtCnt = 0;
    SpotLgtCnt = 0;

    for (auto l : Lgts)
      delete l.second;
    Lgts.clear();
  } /* End of 'Clear' function */

  /* Clear texture variables function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void ClearTextures(void)
  {
    Textures.clear();
    AddedTex.clear();
    CountOfTexs = 0;
  } /* End of 'ClearTextures' function */

  /* Get function parameters from string function.
   * ARGUMENTS:
   *   - string:
   *       const std::string &Str;
   * RETURNS:
   *     (std::vector<std::string>) function parameters;
   */
  std::vector<std::string> ObjectHelper_GetParametrs(const std::string &Str);

  /* Get variable type from string function.
   * ARGUMENTS:
   *   - string:
   *       const std::string &Str;
   * RETURNS:
   *     (obj_type) object type;
   */
  obj_type ObjectHelper_GetType(const std::string &Str);

  /* Get string by variable name and parameters function.
   * ARGUMENTS:
   *   - variable name:
   *       const std::string &Var;
   *   - parameters array:
   *       std::vector<std::string> Params;
   *   - flag is modified:
   *       bool &IsMod;
   * RETURNS:
   *     (std::string) string;
   */
  std::string ObjectHelper_GetStr(const std::string &Var, std::vector<std::string> Params, bool &IsMod);

  /* Check parameters in operation function.
   * ARGUMENTS:
   *   - parameters array:
   *       std::vector<std::string> Params;
   * RETURNS:
   *     (bool) true or false;
   */
  bool ObjectHelper_CheckOperParams(std::vector<std::string> Params);

  /* Add texture function.
   * ARGUMENTS:
   *   - texture name:
   *       const std::string& TexName;
   * RETURNS:
   *     (int) texture index;
   */
  int ObjectHelper_AddTex(const std::string& TexName);

  /* Add string to shader buffer function.
   * ARGUMENTS:
   *   - string to add:
   *       const std::string& Str;
   * RETURNS: None.
   */
  void ObjectHelper_PrintBuf(const std::string& Str);

  /* Get result string by variable name function.
   * ARGUMENTS:
   *   - variable:
   *       const std::string& Var;
   *   - flag is modifiers:
   *       bool &IsMod;
   * RETURNS:
   *  (std::string) result string.
   */
  std::string GetStrByVar(const std::string &Var, bool &IsMod);

  /* Get result string by variable name and description function.
   * ARGUMENTS:
   *   - variable and description:
   *       const std::string& Var, Desc;
   *   - flag is modifiers:
   *       bool &IsMod;
   * RETURNS:
   *  (std::string) result string.
   */
  std::string GetStrByVarDesc(const std::string& Var, const std::string& Desc, bool &IsMod);
}; /* End of 'parser' class */

#endif /* __parser_h_ */

/* END OF 'parser.h' FILE */