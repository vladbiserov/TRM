/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : shapes.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __shapes_h_
#define __shapes_h_

#include "objects.h"

/* Sphere shape class */
class sphere : public shape
{
private:
  std::string C, R;
public:
  /* Class constructor */
  sphere(std::vector<std::string> Params) : shape(2)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    C = Params[0];
    R = Params[1];
    Mtl = Params[2];
    if (Params.size() > 3)
      Tex = Params[3];
  } /* End of 'sphere' function */

  /* Class destructor */
  ~sphere(void) override
  {
    C.clear();
    R.clear();
  } /* End of '~sphere' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    if (Tex == "")
      return "";
    return std::format("uv_{2} = vec2(-atan((point.z - {0}.z) / {1}, (point.x - {0}.x) / {1}) / 1.7 + 0.4, 0.15 + acos((point.y - {0}.y) / {1}));\n", C, R, Val);
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFSphere(mod_{0}, sphere({1}, {2}, {3}));\n", Val, C, R, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'sphere' class */

/* Box shape class */
class box : public shape
{
private:
  std::string C, R;
public:
  /* Class constructor */
  box(std::vector<std::string> Params) : shape(2)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    C = Params[0];
    R = Params[1];
    Mtl = Params[2];
    if (Params.size() > 3)
      Tex = Params[3];
  } /* End of 'box' function */

  /* Class destructor */
  ~box(void) override
  {
    C.clear();
    R.clear();
  } /* End of '~box' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    if (Tex == "")
      return "";
    return std::format(
      "uv_{2} = vec2(0);\n"
      "if (abs(point.z + {1}.z - {0}.z) < {3} || abs(point.z - {1}.z - {0}.z) < {3})\n"
      "  uv_{2} = vec2(point.x, -point.y) / 4;\n"
      "else if (abs(point.x + {1}.x - {0}.x) < {3} || abs(point.x - {1}.x - {0}.x) < {3})\n"
      "  uv_{2} = vec2(point.z, -point.y) / 4;\n"
      "else if (abs(point.y + {1}.y - {0}.y) < {3} || abs(point.y - {1}.y - {0}.y) < {3})\n"
      "  uv_{2} = vec2(point.x, point.z) / 4;\n", C, R, Val, 0.0001);
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFBox(mod_{0}, box({1}, {2}, {3}));\n", Val, C, R, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'box' class */

/* Plane shape class */
class plane : public shape
{
private:
  std::string N, D;
public:
  /* Class constructor */
  plane(std::vector<std::string> Params) : shape(2)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    N = Params[0];
    D = Params[1];
    Mtl = Params[2];
    if (Params.size() > 3)
      Tex = Params[3];
  } /* End of 'plane' function */

  /* Class destructor */
  ~plane(void) override
  {
    N.clear();
    D.clear();
  } /* End of '~plane' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    if (Tex == "")
      return "";
    return std::format(
      "uv_{0} = vec2(0);\n"
      "if ({1}.x == 1)\n"
      "  N2_{0} = vec3(0, 0, 1);\n"
      "else\n"
      "  N2_{0} = vec3(1, 0, 0);\n"
      "N3_{0} = normalize(cross({1}, N2_{0}));\n"
      "N2_{0} = normalize(cross(N3_{0}, {1}));\n"
      "uv_{0} = vec2(dot(point - {1} * {2}, N2_{0}) / 4, -dot(point - {1} * {2}, N3_{0}) / 4);\n", Val, N, D);
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFPlane(mod_{0}, plane({1}, {2}, {3}));\n", Val, N, D, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'plane' class */

/* Ellipsoid shape class */
class ellipsoid : public shape
{
private:
  std::string C, R;
public:
  /* Class constructor */
  ellipsoid(std::vector<std::string> Params) : shape(2)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    C = Params[0];
    R = Params[1];
    Mtl = Params[2];
    if (Params.size() > 3)
      Tex = Params[3];
  } /* End of 'ellipsoid' function */

  /* Class destructor */
  ~ellipsoid(void) override
  {
    C.clear();
    R.clear();
  } /* End of '~ellipsoid' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    //if (Tex == "")
    return "";
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFEllipsoid(mod_{0}, ellipsoid({1}, {2}, {3}));\n", Val, C, R, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'ellipsoid' class */

/* Torus shape class */
class torus : public shape
{
private:
  std::string C, N, R1, R2;
public:
  /* Class constructor */
  torus(std::vector<std::string> Params) : shape(4)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    C = Params[0];
    N = Params[1];
    R1 = Params[2];
    R2 = Params[3];
    Mtl = Params[4];
    if (Params.size() > 5)
      Tex = Params[5];
  } /* End of 'torus' function */

  /* Class destructor */
  ~torus(void) override
  {
    C.clear();
    N.clear();
    R1.clear();
    R2.clear();
  } /* End of '~torus' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    return "";
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFTorus(mod_{0}, torus({1}, {2}, {3}, {4}, {5}));\n", Val, C, N, R1, R2, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'torus' class */

/* Cylinder shape class */
class cylinder : public shape
{
private:
  std::string P1, P2, R1, R2;
public:
  /* Class constructor */
  cylinder(std::vector<std::string> Params) : shape(4)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    P1 = Params[0];
    R1 = Params[1];
    P2 = Params[2];
    R2 = Params[3];
    Mtl = Params[4];
    if (Params.size() > 5)
      Tex = Params[5];
  } /* End of 'cylinder' function */

  /* Class destructor */
  ~cylinder(void) override
  {
    P1.clear();
    P2.clear();
    R1.clear();
    R2.clear();
  } /* End of '~cylinder' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    return "";
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFCylinder(mod_{0}, cylinder({1}, {2}, {3}, {4}, {5}));\n", Val, P1, R1, P2, R2, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'cylinder' class */

/* Casule shape class */
class capsule : public shape
{
private:
  std::string P1, P2, R;
public:
  /* Class constructor */
  capsule(std::vector<std::string> Params) : shape(3)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    P1 = Params[0];
    P2 = Params[1];
    R = Params[2];
    Mtl = Params[3];
    if (Params.size() > 4)
      Tex = Params[4];
  } /* End of 'capsule' function */

  /* Class destructor */
  ~capsule(void) override
  {
    P1.clear();
    P2.clear();
    R.clear();
  } /* End of '~capsule' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    return "";
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFCapsule(mod_{0}, capsule({1}, {2}, {3}, {4}));\n", Val, P1, P2, R, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'capsule' class */

/* Hollow sphere shape class */
class hollow_sphere : public shape
{
private:
  std::string C, R, T, H;
public:
  /* Class constructor */
  hollow_sphere(std::vector<std::string> Params) : shape(4)
  {
    if ((int)Params.size() < ParamCount + 1)
      return;
    IsInit = true;
    C = Params[0];
    R = Params[1];
    T = Params[2];
    H = Params[3];
    Mtl = Params[4];
    if (Params.size() > 5)
      Tex = Params[5];
  } /* End of 'hollow_sphere' function */

  /* Class destructor */
  ~hollow_sphere(void) override
  {
    C.clear();
    R.clear();
    T.clear();
    H.clear();
  } /* End of '~hollow_sphere' function */

  /* Get shape texture function.
   * ARGUMENTS:
   *   - shape:
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) texture.
   */
  std::string GetTex(const std::string& Val) override
  {
    return "";
  } /* End of 'GetTex' function */

  /* Get shape description by string function.
   * ARGUMENTS:
   *   - string
   *       const std::string &Val;
   * RETURNS:
   *  (std::string) description.
   */
  std::string GetStr(const std::string& Val) override
  {
    return std::format("{0} = SDFCutHollowSphere(mod_{0}, hollow_sphere({1}, {2}, {3}, {4}, {5}));\n", Val, C, R, H, T, Mtl);
  } /* End of 'GetStr' function */
}; /* End of 'hollow_sphere' class */

#endif /* __shapes_h_ */

/* END OF 'shapes.h' FILE */