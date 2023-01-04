/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : test.cpp
  * PURPOSE     : Ray marching project.
  *               Control module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include "../animation/animation.h"
#include "../def.h"
#include <cmath>

namespace trm
{
  class ctrl_unit : public unit
  {
  private:
  public:
    ctrl_unit(animation* Ani)
    {
    }
    ~ctrl_unit(VOID)
    {
    }
    VOID Response(animation* Ani) override
    {
      FLT Time = Ani->GlobalDeltaTime * 10;

      mth::vec3<FLT> Right = Ani->Camera.Right;
      Right.Y = 0;
      mth::vec3<FLT> Dir = Ani->Camera.Dir;
      Dir.Y = 0;

      if (Ani->MouseX != 0 && Ani->Keys[VK_LBUTTON])
      {
        Ani->Camera.At -= Ani->Camera.Loc;
        Ani->Camera.At = mth::matr<FLT>::Rotate(Ani->Camera.Up, Ani->MouseDx / 30.0).TransformVector(Ani->Camera.At);
        Ani->Camera.Right = mth::matr<FLT>::Rotate(Ani->Camera.Up, Ani->MouseDx / 30.0).TransformVector(Ani->Camera.At);
        Ani->Camera.At += Ani->Camera.Loc;
        Ani->Camera.SetLocAtUp(Ani->Camera.Loc, Ani->Camera.At);
      }
      if (Ani->MouseDy != 0 && Ani->Keys[VK_LBUTTON])
      {
        Ani->Camera.At -= Ani->Camera.Loc;
        Ani->Camera.At = mth::matr<FLT>::Rotate(Ani->Camera.Right, Ani->MouseDy / 30.0).TransformVector(Ani->Camera.At);
        Ani->Camera.At += Ani->Camera.Loc;
        Ani->Camera.SetLocAtUp(Ani->Camera.Loc, Ani->Camera.At);
      }
      if (Ani->Keys[VK_RBUTTON])
      {
        FLT Hp, Wp = Hp = Ani->Camera.ProjDist;

        if (Ani->Camera.FrameW > Ani->Camera.FrameH)
          Wp *= static_cast<FLT>(Ani->Camera.FrameW) / Ani->Camera.FrameH;
        else
          Hp *= static_cast<FLT>(Ani->Camera.FrameH) / Ani->Camera.FrameW;

        FLT Dist = !(Ani->Camera.At - Ani->Camera.Loc);

        FLT sx = -Ani->MouseDx * Wp / Ani->Camera.FrameW * Dist / Ani->Camera.ProjDist;
        FLT sy = Ani->MouseDy * Hp / Ani->Camera.FrameH * Dist / Ani->Camera.ProjDist;

        Ani->Camera.Move((Ani->Camera.Right * sx + Ani->Camera.Up * sy) / 5);
      }
      if (Ani->Keys['W'])
        Ani->Camera.Move(Dir * Time);
      if (Ani->Keys['S'])
        Ani->Camera.Move(-Dir * Time);
      if (Ani->Keys['D'])
        Ani->Camera.Move(Right * Time);
      if (Ani->Keys['A'])
        Ani->Camera.Move(-Right * Time);
      if (Ani->Keys[VK_LSHIFT])
        Ani->Camera.Move(vec3(0, Time, 0));
      if (Ani->Keys[VK_CONTROL])
        Ani->Camera.Move(vec3(0, -Time, 0));
      Ani->Camera.Move(Dir * Ani->MouseDz * Time / 140);

      if (Ani->KeysClick['P'])
        Ani->IsPaused = !Ani->IsPaused;
      if (Ani->KeysClick['F'])
        Ani->Camera.SetLocAtUp(vec3(0, 3, 10), vec3(0));
    }
    VOID Render(animation* Ani) override
    {
    }
  };
  static animation::unit_register<ctrl_unit> _("Control");
}

/* END OF 'test.cpp' FILE */