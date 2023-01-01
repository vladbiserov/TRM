/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : anim.cpp
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
#include "animation.h"

trm::animation trm::animation::Instance;

/* Render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::animation::Render( VOID )
{
  UboAnim->Apply();
  timer::Response();
  input::Response();
  Scene->Response(this);
  render::Start();
  if (DW.IsChanged())
  {
    if (Parser.ReadFile())
    {
      Parser.GetExprs();
      Parser.WriteFile();
    }
    UpdateTextures();
    shader_manager::Update();
    OutputDebugString("Shader was update!\n");
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
  DW.StartWatch("bin/scenes");

  if (Parser.ReadFile())
  {
    Parser.GetExprs();
    Parser.WriteFile();
  }
  UpdateTextures();
  shader_manager::Update();
  OutputDebugString("Shader was update!\n");

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