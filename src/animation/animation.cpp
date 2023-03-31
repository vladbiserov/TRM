/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : anim.cpp
  * PURPOSE     : Ray marching project.
  *               Animation module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include <fstream>
#include "animation.h"
#include "../utils/parser/parser.h"

trm::animation trm::animation::Instance;

/* Render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Render( VOID )
{
  static std::string SName = "bin\\scenes\\a.scene";
  UboAnim->Apply();
  timer::Response();
  if (win::IsActive)
    input::Response();
  Scene->Response(this);
  render::Start();
  
  if (DW.IsChanged(GlobalTime))
  {
    parser::Parse(SName,
      "bin\\shaders\\RT\\myfrag.glsl", "bin\\shaders\\RT\\frag.glsl");
    shader_manager::Update();
  }
  if (win::IsFileChanged)
  {
    win::IsFileChanged = false;
    SName = "bin\\scenes\\" + win::CurSceneName;
    SetCurrentDirectory(win::WorkDirectory.c_str());
    win::UpdateMenuSceneName();
    parser::Parse(SName,
      "bin\\shaders\\RT\\myfrag.glsl", "bin\\shaders\\RT\\frag.glsl");
    shader_manager::Update();
  }
  UBO_ANIM UC =
  {
    vec4(0, 0, 0, Time),
  };
  UboAnim->Update(&UC);
  Scene->Render(this);
  render::End();
}; /* End of 'trm::animation::Render' function */

VOID trm::animation::UpdateTextures( VOID )
{
  for (auto tex : Textures)
  {
    Textures[tex.first].Tex = texture_manager::CreateTexture(tex.first);
  }
}

/* Initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Init( VOID )
{
  DW.StartWatch("bin\\scenes");

  SetCurrentDirectory(win::WorkDirectory.c_str());
  parser::Parse("bin\\scenes\\a.scene",
    "bin\\shaders\\RT\\myfrag.glsl", "bin\\shaders\\RT\\frag.glsl");
  shader_manager::Update();

  // if (Parser.ReadFile())
  // {
  //   Parser.GetExprs();
  //   if (Parser.WriteFile())
  //   {
  //     UpdateTextures();
  //     shader_manager::Update();
  //     OutputDebugString("Shader was update!\n");
  //   }
  // }
}; /* End of 'trm::animation::Init' function */

/* Deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Close( VOID )
{
  DW.StopWatch();
}; /* End of 'trm::animation::Close' function */

/* Resize window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Resize( VOID )
{
  render::Resize();
  Render();
  render::CopyFrame();
}; /* End of 'trm::animation::Resize' function */

/* Update timer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Timer( VOID )
{
  Render();
  render::CopyFrame();
}; /* End of 'trm::animation::Timer' function */

/* Activation window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Activate( VOID )
{
}; /* End of 'trm::animation::Activate' function */

/* Idle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Idle( VOID )
{
  Render();
  render::CopyFrame();
}; /* End of 'trm::animation::Idle' function */

/* Erase background function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Erase( HDC hDC )
{
}; /* End of 'trm::animation::Erase' function */

/* Paint window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Paint( HDC hDC )
{
}; /* End of 'trm::animation::Paint' function */

/* Add unit to scene operator.
 * ARGUMENTS:
 *   - name of the unit:
 *       const std::string &Name;
 * RETURNS:
 *   (scene &) Self-reference.
 */
trm::scene & trm::scene::operator<<( const std::string &Name )
{
  animation &Ani = animation::Get();
  if (Ani.UnitNames.find(Name) != Ani.UnitNames.end())
  {
    Units << Ani.UnitNames[Name]();
  }
  return *this;
} /* End of 'trm::scene::operator<<' function */

/* END OF 'animation.cpp' FILE */