#version 420

#define Threshold float(0.001)
#define HUGE_VAL float(1e+38)

//painting color
layout(location = 0) out vec4 OutColor;

// camera buffer
layout(std140, binding = 2) uniform Camera
{
  mat4 View;
  mat4 Proj;
  mat4 VP;
  vec3 Loc;
  float FrameW;
  vec3 Dir;
  float ProjDist;
  vec3 Right;
  float Wp;
  vec3 Up;
  float Hp;
  vec3 At;
  float FrameH;
  vec3 Dummy;
  float Size;
};

layout(std140, binding = 3) uniform Animation
{
  vec3 Du;
  float Time;
};

layout(binding = 0) uniform sampler2D Tex0;
uniform bool IsTexture0;

in vec2 DrawTexCoord;

struct surface
{
  vec3 Ka, Kd, Ks; // ambient, diffuse, specular
  float Ph;        // Bui Tong Phong coefficient
  float Kr, Kt;    // reflected, transmitted
};

const surface MtlLib[] = {
  {/*"Black Plastic",   0*/ vec3(0.0, 0.0, 0.0             ), vec3(0.01, 0.01, 0.01          ), vec3(0.5, 0.5, 0.5                ), 32.9, 0.5, 0.7},
  {/*"Brass",           1*/ vec3(0.329412,0.223529,0.027451), vec3(0.780392,0.568627,0.113725), vec3(0.992157,0.941176,0.807843   ), 27.8, 0.5, 0.7},
  {/*"Bronze",          2*/ vec3(0.2125,0.1275,0.054       ), vec3(0.714,0.4284,0.18144      ), vec3(0.393548,0.271906,0.166721   ), 25.6, 0.5, 0.7},
  {/*"Chrome",          3*/ vec3(0.25, 0.25, 0.25          ), vec3(0.4, 0.4, 0.4             ), vec3(0.774597, 0.774597, 0.774597 ), 76.8, 0.5, 0.7},
  {/*"Copper",          4*/ vec3(0.19125,0.0735,0.0225     ), vec3(0.7038,0.27048,0.0828     ), vec3(0.256777,0.137622,0.086014   ), 12.8, 0.5, 0.7},
  {/*"Gold",            5*/ vec3(0.24725,0.1995,0.0745     ), vec3(0.75164,0.60648,0.22648   ), vec3(0.628281,0.555802,0.366065   ), 51.2, 0.5, 0.7},
  {/*"Peweter",         6*/ vec3(0.10588,0.058824,0.113725 ), vec3(0.427451,0.470588,0.541176), vec3(0.3333,0.3333,0.521569       ), 9.84, 0.5, 0.7},
  {/*"Silver",          7*/ vec3(0.19225,0.19225,0.19225   ), vec3(0.50754,0.50754,0.50754   ), vec3(0.508273,0.508273,0.508273   ), 51.2, 0.5, 0.7},
  {/*"Polished Silver", 8*/ vec3(0.23125,0.23125,0.23125   ), vec3(0.2775,0.2775,0.2775      ), vec3(0.773911,0.773911,0.773911   ), 89.6, 0.5, 0.7},
  {/*"Turquoise",       9*/ vec3(0.1, 0.18725, 0.1745      ), vec3(0.396, 0.74151, 0.69102   ), vec3(0.297254, 0.30829, 0.306678  ), 12.8, 0.5, 0.7},
  {/*"Ruby",           10*/ vec3(0.1745, 0.01175, 0.01175  ), vec3(0.61424, 0.04136, 0.04136 ), vec3(0.727811, 0.626959, 0.626959 ), 76.8, 0.5, 0.7},
  {/*"Polished Gold",  11*/ vec3(0.24725, 0.2245, 0.0645   ), vec3(0.34615, 0.3143, 0.0903   ), vec3(0.797357, 0.723991, 0.208006 ), 83.2, 0.5, 0.7},
  {/*"Polished Bronze",12*/ vec3(0.25, 0.148, 0.06475      ), vec3(0.4, 0.2368, 0.1036       ), vec3(0.774597, 0.458561, 0.200621 ), 76.8, 0.5, 0.7},
  {/*"Polished Copper",13*/ vec3(0.2295, 0.08825, 0.0275   ), vec3(0.5508, 0.2118, 0.066     ), vec3(0.580594, 0.223257, 0.0695701), 51.2, 0.5, 0.7},
  {/*"Jade",           14*/ vec3(0.135, 0.2225, 0.1575     ), vec3(0.135, 0.2225, 0.1575     ), vec3(0.316228, 0.316228, 0.316228 ), 12.8, 0.5, 0.7},
  {/*"Obsidian",       15*/ vec3(0.05375, 0.05, 0.06625    ), vec3(0.18275, 0.17, 0.22525    ), vec3(0.332741, 0.328634, 0.346435 ), 38.4, 0.5, 0.7},
  {/*"Pearl",          16*/ vec3(0.05, 0.020725, 0.020725    ), vec3(1.0, 0.829, 0.829         ), vec3(0.1, 0.1, 0.1 ), 1.2, 1, 1},
  {/*"Emerald",        17*/ vec3(0.0215, 0.1745, 0.0215    ), vec3(0.07568, 0.61424, 0.07568 ), vec3(0.033, 0.127811, 0.133       ), 6.8, 0.5, 0.7},
  {/*"Black Plastic",  18*/ vec3(0.0, 0.0, 0.0             ), vec3(0.01, 0.01, 0.01          ), vec3(0.5, 0.5, 0.5                ), 32.0, 0.5, 0.7},
  {/*"Black Rubber",   19*/ vec3(0.02, 0.02, 0.02          ), vec3(0.01, 0.01, 0.01          ), vec3(0.4, 0.4, 0.4                ), 10.0, 0.5, 0.7},
  {/*"White",          20*/ vec3(0.02, 0.02, 0.02          ), vec3(1, 1, 1          ),          vec3(0.1, 0.1, 0.1                ), 10.0, 0.5, 0.7},
  {/*"Pearl",          21*/ vec3(0.25, 0.020725, 0.20725  ), vec3(1.0, 0.829, 0.829         ), vec3(0.1, 0.1, 0.1                ), 1.2, 1, 1},
  {/*"Emerald",        22*/ vec3(0.0215, 0.1745, 0.0215    ), vec3(0.61424, 0.07568, 0.07568 ), vec3(0.033, 0.127811, 0.133       ), 6.8, 0.5, 0.7},
};


struct ray
{
  vec3 Org;
  vec3 Dir;
};

struct envi
{
  float RefractionCoef;
  float DecayCoef;
};

struct intr
{
  float T;
  vec3 P;
  vec3 N;
  surface Mtl;
};

struct light_info
{
  vec3 L;        // light source direction
  vec3 Color;    // light source color
  float Dist;    // distance to light source
};

struct point_light
{
  vec3 Pos;   // Point position
  vec3 Color; // Light color
  float Cc, Cl, Cq;
};

struct dir_light 
{ 
  vec3 Dir;   // Direction 
  vec3 Color; // Light color  
}; 

struct spot_light 
{ 
  vec3 Pos;   // Spot position 
  vec3 Dir;   // Spot direction 
  vec3 Color; // Light color 
  float A1, A2; 
}; 

struct shape
{
  int shape_type;
  int shape_id;
};

struct shapeop
{
  shape shape;
  int modify;
  int op_type;
  int next;
};

struct sphere
{
  vec3 C;
  float R;
  surface Mtl;
};

struct box
{
  vec3 C;
  vec3 R;
  surface Mtl;
};

struct plane
{
  vec3 N;
  float D;
  surface Mtl;
};

struct torus
{
  vec3 C;
  vec3 N;
  float k1, k2;
  surface Mtl;
};

struct ellipsoid 
{ 
  vec3 C; 
  vec3 R; 
  surface Mtl; 
}; 

struct cylinder
{
  vec3 P1;
  float R1;
  vec3 P2;
  float R2;
  surface Mtl;
};

struct capsule 
{ 
  vec3 P1; 
  vec3 P2; 
  float R; 
  surface Mtl; 
}; 

struct hollow_sphere
{
  vec3 C;
  float r, h, t;
  surface Mtl;
};

const float PI = 3.14159;
bool IsSky = false;

float D2R( float Degree )
{
  return (Degree * PI) / 180;
}

float R2D( float Radian )
{
  return (Radian * 180) / PI;
}

float Hash( vec2 P )
{
  float h = dot(P, vec2(127.1, 311.7));	
  return fract(sin(h) * 43758.5453123);
}

float Noise( vec2 P )
{
  vec2 i = floor(P);
  vec2 f = fract(P);	
  vec2 u = f * f * (3.0 - 2.0 * f);

  return -1.0 + 2.0 * mix(mix(Hash(i + vec2(0.0, 0.0)), Hash(i + vec2(1.0, 0.0)), u.x), mix(Hash(i + vec2(0.0, 1.0)), Hash(i + vec2(1.0, 1.0)), u.x), u.y);
}

ray SetRay( float Sx, float Sy )
{
  ray Ray;

  vec3 A = Dir * ProjDist;
  vec3 B = Right * (Sx - FrameW / 2) * Wp / FrameW;
  vec3 C = Up * (-Sy + FrameH / 2) * Hp / FrameH;
  vec3 X = (A + B) + C;
  Ray.Org = Loc + X;
  Ray.Dir = normalize(X);

  return Ray;
}

vec3 RayApply( ray R, float T )
{
  return R.Org + R.Dir * T;
}



float SDFSphere( vec3 P, sphere S )
{
  return length(P - S.C) - S.R;
}

float SDFBox( vec3 P, box B )
{
  vec3 d = abs(P - B.C) - B.R;

  return min(max(d.x, max(d.y,d.z)), 0.0) + length(max(d, 0.0));
}

float SDFPlane( vec3 P, plane Pl )
{
  return dot(P, Pl.N) - Pl.D;
}

float SDFTorus( vec3 P, torus T ) 
{
  return distance(P, T.C + T.k1 * normalize(cross(cross(T.N, (P - T.C)), T.N))) - T.k2;
}

float SDFEllipsoid( vec3 P, ellipsoid E ) 
{ 
  float k0 = length((P - E.C) / E.R); 
  float k1 = length((P - E.C) / (E.R * E.R)); 
  return k0 * (k0 - 1.0) / k1; 
}

float SDFCylinder( vec3 P, cylinder C )
{
  float rba  = C.R2 - C.R1;
  float baba = dot(C.P2 - C.P1, C.P2 - C.P1);
  float papa = dot(P - C.P1, P - C.P1);
  float paba = dot(P - C.P1, C.P2 - C.P1) / baba;
  float x = sqrt(papa - paba * paba * baba);
  float cax = max(0.0, x - ((paba < 0.5) ? C.R1 : C.R2));
  float cay = abs(paba - 0.5) - 0.5;
  float k = rba * rba + baba;
  float f = clamp((rba * (x - C.R1) + paba * baba) / k, 0.0, 1.0 );
  float cbx = x - C.R1 - f * rba;
  float cby = paba - f;
  float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;
  return s*sqrt(min(cax * cax + cay * cay * baba,
                    cbx * cbx + cby * cby * baba));
}

float SDFCapsule( vec3 P, capsule C ) 
{ 
  vec3 pa = P - C.P1, ba = C.P2 - C.P1; 
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0); 
   
  return length(pa - ba * h) - C.R; 
}

float SDFCutHollowSphere( vec3 P, hollow_sphere hs )
{
  // sampling independent computations (only depend on shape)
  float w = sqrt(hs.r * hs.r - hs.h * hs.h);
  
  P -= hs.C;

  // sampling dependant computations
  vec2 q = vec2(length(P.xz), P.y);
  return ((hs.h * q.x < w * q.y) ? length(q - vec2(w, hs.h)) : 
                                   abs(length(q) - hs.r)) - hs.t;
}

float SDFSeaHelp( vec2 uv, float Choppy )
{
  uv += Noise(uv);        
  vec2 wv = 1.0 - abs(sin(uv));
  vec2 swv = abs(cos(uv));    
  wv = mix(wv, swv, wv);
  return pow(1.0 - pow(wv.x * wv.y, 0.65), Choppy);
}

float SDFSea( vec3 P )
{
  float freq = 0.16;
  float amp = 0.6;
  float choppy = 4;
  vec2 uv = P.xz; 
  uv.x *= 0.75;

  float d, h = 0.0;

  for (int i = 0; i < 3; i++)
  {
    d = SDFSeaHelp((uv + Time) * freq, choppy);
    d += SDFSeaHelp((uv - Time) * freq, choppy);
    h += d * amp;        
    uv *= mat2(1.6, 1.2, -1.2, 1.6);
    freq *= 1.9;
    amp *= 0.22;
    choppy = mix(choppy, 1.0, 0.2);
  }

  return P.y - h;
}

float SDFUnion( float a, float b )
{
  return min(a, b);
}

float SDFInter( float a, float b )
{
  return max(a, b);
}

float SDFDifer( float a, float b )
{
  return max(a, -b);
}
 
float SDFUnionSmooth(float distA, float distB, float k )
{
  float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
  return mix(distA, distB, h) - k*h*(1.-h); 
}

float SDFInterSmooth(float distA, float distB, float k ) 
{
  float h = clamp(0.5 - 0.5*(distA-distB)/k, 0., 1.);
  return mix(distA, distB, h ) + k*h*(1.-h); 
}
 
float SDFDiferSmooth(float distA, float distB, float k)
{
  float h = clamp(0.5 - 0.5*(distB+distA)/k, 0., 1.);
  return mix(distA, -distB, h ) + k*h*(1.-h); 
}

surface SDFSurfaceSmoothUnion( float k1, surface surface1, float k2, in surface surface2, float smoothness )
{
  float interpolation = clamp(0.5 + 0.5 * (k1 - k2) / smoothness, 0.0, 1.0);
  return surface(mix(surface2.Ka, surface1.Ka, 1 - interpolation),// - smoothness * interpolation * (1.0 - interpolation);
                 mix(surface2.Kd, surface1.Kd, 1 - interpolation),// - smoothness * interpolation * (1.0 - interpolation);
                 mix(surface2.Ks, surface1.Ks, 1 - interpolation),// - smoothness * interpolation * (1.0 - interpolation);
                 mix(surface2.Ph, surface1.Ph, 1 - interpolation), 0, 0);// - smoothness * interpolation * (1.0 - interpolation);
}

mat4 MatrRotate( float d, vec3 a )
{
  float R = D2R(d);
  float s = sin(R), c = cos(R);
  return mat4(c + a.x * a.x * (1 - c),       a.x * a.y * (1 - c) + a.z * s, a.x * a.z * (1 - c) - a.y * s, 0,
              a.y * a.x * (1 - c) - a.z * s, c + a.y * a.y * (1 - c),       a.y * a.z * (1 - c) + a.x * s, 0,
              a.z * a.x * (1 - c) + a.y * s, a.z * a.y * (1 - c) - a.x * s, c + a.z * a.z * (1 - c),       0,
              0,                             0,                             0,                             1);
}

vec3 Rotate( float a, vec3 Axis, vec3 p )
{
  return inverse(mat3(MatrRotate(a, Axis))) * p;
}

vec3 Translate( vec3 q, vec3 p )
{
  return q + p;
}

vec3 Scale( vec3 s, vec3 p )
{
  return p / s;
}

vec3 Replication( vec3 c, vec3 p )
{
  return mod(p, c) - 0.5 * c;
}

float SceneSDF( in vec3 point, inout surface mtl )
{
float res = 0, tmp;
surface tmp_mtl;         

// create 'c' var
surface mtl_c;
vec2 uv_c;
vec3 N2_c, N3_c, mod_c = point;
float c;

c = SDFPlane(mod_c, plane(vec3(0,0,1), -10, MtlLib[6]));
mtl_c = MtlLib[6];
uv_c = vec2(0);
if (vec3(0,0,1).x == 1)
  N2_c = vec3(0, 0, 1);
else
  N2_c = vec3(1, 0, 0);
N3_c = normalize(cross(vec3(0,0,1), N2_c));
N2_c = normalize(cross(N3_c, vec3(0,0,1)));
uv_c = vec2(dot(point - vec3(0,0,1) * -10, N2_c) / 4, -dot(point - vec3(0,0,1) * -10, N3_c) / 4);
mtl_c.Kd = texture(Tex0, uv_c).bgr;


// create 'head' var
surface mtl_head;
vec2 uv_head;
vec3 N2_head, N3_head, mod_head = point;
float head;

head = SDFCapsule(mod_head, capsule(vec3(0), vec3(0,4,0), 8, MtlLib[9]));
mtl_head = MtlLib[9];


// create 'e1' var
surface mtl_e1;
vec2 uv_e1;
vec3 N2_e1, N3_e1, mod_e1 = point;
float e1;

e1 = SDFSphere(mod_e1, sphere(vec3(-3,5,6+abs(cos(Time*5))*0.5), 1.5+abs(sin(Time*5))*0.5, MtlLib[20]));
mtl_e1 = MtlLib[20];


// create 'z1' var
surface mtl_z1;
vec2 uv_z1;
vec3 N2_z1, N3_z1, mod_z1 = point;
float z1;

z1 = SDFSphere(mod_z1, sphere(vec3(-3.2,5.2,7+abs(cos(Time*5))*0.5), 0.5+abs(sin(Time*5))*0.5, MtlLib[18]));
mtl_z1 = MtlLib[18];


// update 'e1' var
tmp = e1;
tmp_mtl = mtl_e1;
e1 = SDFUnion(e1, z1);
if (e1 == tmp)
  mtl_e1 = tmp_mtl;
else if (e1 == z1)
  mtl_e1 = mtl_z1;
else
  mtl_e1 = SDFSurfaceSmoothUnion(tmp, tmp_mtl, z1, mtl_z1, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFDiferSmooth(head, e1, 0.5);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == e1)
  mtl_head = mtl_e1;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, e1, mtl_e1, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFUnion(head, e1);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == e1)
  mtl_head = mtl_e1;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, e1, mtl_e1, 0.5);

// create 'e2' var
surface mtl_e2;
vec2 uv_e2;
vec3 N2_e2, N3_e2, mod_e2 = point;
float e2;

e2 = SDFSphere(mod_e2, sphere(vec3(3,5,6+abs(sin(Time*5))*0.5), 1.5+abs(cos(Time*5))*0.5, MtlLib[20]));
mtl_e2 = MtlLib[20];


// create 'z2' var
surface mtl_z2;
vec2 uv_z2;
vec3 N2_z2, N3_z2, mod_z2 = point;
float z2;

z2 = SDFSphere(mod_z2, sphere(vec3(3.2,5.2,7+abs(sin(Time*5))*0.5), 0.5+abs(cos(Time*5))*0.5, MtlLib[18]));
mtl_z2 = MtlLib[18];


// update 'e2' var
tmp = e2;
tmp_mtl = mtl_e2;
e2 = SDFUnion(e2, z2);
if (e2 == tmp)
  mtl_e2 = tmp_mtl;
else if (e2 == z2)
  mtl_e2 = mtl_z2;
else
  mtl_e2 = SDFSurfaceSmoothUnion(tmp, tmp_mtl, z2, mtl_z2, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFDiferSmooth(head, e2, 0.5);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == e2)
  mtl_head = mtl_e2;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, e2, mtl_e2, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFUnion(head, e2);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == e2)
  mtl_head = mtl_e2;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, e2, mtl_e2, 0.5);

// create 'm' var
surface mtl_m;
vec2 uv_m;
vec3 N2_m, N3_m, mod_m = point;
float m;

m = SDFCapsule(mod_m, capsule(vec3(0,0,8), vec3(0,abs(sin(Time*10)),8), 2+cos(Time)*0.8, MtlLib[2]));
mtl_m = MtlLib[2];


// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFDiferSmooth(head, m, 0.3);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == m)
  mtl_head = mtl_m;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, m, mtl_m, 0.5);

// create 'b1' var
surface mtl_b1;
vec2 uv_b1;
vec3 N2_b1, N3_b1, mod_b1 = point;
float b1;

b1 = SDFTorus(mod_b1, torus(vec3(-3,6+abs(cos(Time*5))*2,8), vec3(0,0,1), 2, 0.2, MtlLib[18]));
mtl_b1 = MtlLib[18];


// create 'b2' var
surface mtl_b2;
vec2 uv_b2;
vec3 N2_b2, N3_b2, mod_b2 = point;
float b2;

b2 = SDFBox(mod_b2, box(vec3(-3,5.2+abs(cos(Time*5))*2,8), vec3(3,2.3,2.3), MtlLib[18]));
mtl_b2 = MtlLib[18];


// update 'b1' var
tmp = b1;
tmp_mtl = mtl_b1;
b1 = SDFDifer(b1, b2);
if (b1 == tmp)
  mtl_b1 = tmp_mtl;
else if (b1 == b2)
  mtl_b1 = mtl_b2;
else
  mtl_b1 = SDFSurfaceSmoothUnion(tmp, tmp_mtl, b2, mtl_b2, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFUnion(b1, head);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == b1)
  mtl_head = mtl_b1;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, b1, mtl_b1, 0.5);

// update 'b1' var
b1 = SDFTorus(mod_b1, torus(vec3(3,6+abs(sin(Time*5))*2,8), vec3(0,0,1), 2, 0.2, MtlLib[18]));
mtl_b1 = MtlLib[18];

// update 'b2' var
b2 = SDFBox(mod_b2, box(vec3(3,5.2+abs(sin(Time*5))*2,8), vec3(3,2.3,2.3), MtlLib[18]));
mtl_b2 = MtlLib[18];

// update 'b1' var
tmp = b1;
tmp_mtl = mtl_b1;
b1 = SDFDifer(b1, b2);
if (b1 == tmp)
  mtl_b1 = tmp_mtl;
else if (b1 == b2)
  mtl_b1 = mtl_b2;
else
  mtl_b1 = SDFSurfaceSmoothUnion(tmp, tmp_mtl, b2, mtl_b2, 0.5);

// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFUnion(b1, head);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == b1)
  mtl_head = mtl_b1;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, b1, mtl_b1, 0.5);

// create 's' var
surface mtl_s;
vec2 uv_s;
vec3 N2_s, N3_s, mod_s = point;
float s;

s = SDFSphere(mod_s, sphere(vec3(sin(Time*2)*8,2,-2), 9+cos(Time*2), MtlLib[10]));
mtl_s = MtlLib[10];


// update 'head' var
tmp = head;
tmp_mtl = mtl_head;
head = SDFUnionSmooth(head, s, 1);
if (head == tmp)
  mtl_head = tmp_mtl;
else if (head == s)
  mtl_head = mtl_s;
else
  mtl_head = SDFSurfaceSmoothUnion(tmp, tmp_mtl, s, mtl_s, 0.5);

// add to scene 'c' var
res = c;
mtl = mtl_c;

// add to scene 'head' var
tmp = res;
tmp_mtl = mtl;
res = SDFUnion(tmp, head);
if (res == head)
  mtl = mtl_head;
else if (res == tmp)
  mtl = tmp_mtl;
else
  mtl = SDFSurfaceSmoothUnion(tmp, tmp_mtl, head, mtl_head, 0.5);


// create 'd' var
surface mtl_d;
vec2 uv_d;
vec3 N2_d, N3_d, mod_d = point;
float d;


return res;
}

vec3 SDFSceneNormal( vec3 P )
{
  float EPSILON = Threshold;
  surface Mtl;

  float a = SceneSDF(vec3(P.x + EPSILON, P.y, P.z), Mtl),
        b = SceneSDF(vec3(P.x - EPSILON, P.y, P.z), Mtl),
        c = SceneSDF(vec3(P.x, P.y + EPSILON, P.z), Mtl),
        d = SceneSDF(vec3(P.x, P.y - EPSILON, P.z), Mtl),
        e = SceneSDF(vec3(P.x, P.y, P.z + EPSILON), Mtl),
        f = SceneSDF(vec3(P.x, P.y, P.z - EPSILON), Mtl);


  return normalize(vec3(a - b, c - d, e - f));
}

float SphereTracing( ray R, float MaxDist, inout intr Intr )
{
  float t = 0;
  float io;
  
  while (t < MaxDist)
  {
    io = SceneSDF(RayApply(R, t), Intr.Mtl);

    if (io <= Threshold)
    {
      Intr.T = t;
      Intr.P = RayApply(R, t);
      Intr.N = SDFSceneNormal(RayApply(R, t));

      return t;
    }

    t += io;
  }

  return HUGE_VAL;
}

float ShadowPoint( inout light_info Li, point_light Lgt, vec3 P )
{
  float Dist = distance(Lgt.Pos, P);
  vec3 Direction = (Lgt.Pos - P) / Dist;
  float att = 1 / (Lgt.Cq * Dist * Dist + Lgt.Cl * Dist + Lgt.Cc);
  Li.L = Direction;
  Li.Color = Lgt.Color;
  Li.Dist = Dist;
  return min(att, 1); 
}

float ShadowDir( inout light_info Li, dir_light Lgt, vec3 P ) 
{ 
  Li.L = Lgt.Dir; 
  Li.Color = Lgt.Color; 
  Li.Dist = 1; 
  return 0.2;  
} 

float ShadowSpot( inout light_info Li, spot_light Lgt, vec3 P ) 
{ 
  vec3 Direction = Lgt.Pos - P; 
  float Dist = length(Direction), att = 1, 
    cosa = dot(Lgt.Dir, Direction) / Dist; 
 
  if (cosa >= Lgt.A1 && cosa <= 1) 
    att = cosa / Lgt.A1; 
  else if (cosa >= Lgt.A2 && cosa < Lgt.A1) 
    att -= (Lgt.A1 - cosa) / (Lgt.A1 - Lgt.A2); 
  else 
    att = 0; 
 
  Li.L = Lgt.Dir; 
  Li.Color = Lgt.Color; 
  Li.Dist = Dist; 
  return min(att, 1);  
} 

bool IsPointLgt = false;
const int PointLgtCnt = 1;
const point_light PointLgt[PointLgtCnt] = {point_light(vec3(13, 8, 8), vec3(1, 0.7, 0.47), 0.4, 0.6, 0.06)};
bool IsDirLgt = true;
const int DirLgtCnt = 2;
const dir_light DirLgt[DirLgtCnt] = {dir_light(vec3(0,0,1), vec3(0.4)),dir_light(vec3(0,0,-1), vec3(0.4)),}; 
bool IsSpotLgt = false;
const int SpotLgtCnt = 1;
const spot_light SpotLgt[SpotLgtCnt] = {spot_light(vec3(1, 2, 1), vec3(0, 1, 0), vec3(0.1), cos(0.1), cos(1.1))};

const envi Air = envi(1, 0);
ray RRay;

float calcAO( vec3 P, vec3 N )
{
  float occ = 0.0;
  surface mtl;
  float sca = 1.0;

  for (int i = 0; i < 5; i++)
  {
    float h = 0.01 + 0.12 * float(i) / 4.0;
    float a = SceneSDF(P + h * N, mtl);

    occ += (h - a.x) * sca;
    sca *= 0.95;
    if (occ > 0.35)
      break;
  }
  return clamp(1.0 - 3.0 * occ, 0.0, 1.0) * (0.5 + 0.5 * N.y);
}

float SoftShadow( in ray R, float Min, float Max, float k )
{
  float res = 1.0;
  float ph = 1e20;
  surface Mtl;

  for (float t = Min; t < Max;)
  {   
    float io = SceneSDF(RayApply(R, t), Mtl);
    float h = io;
   
    if (h < 0.001)
      return 0.5;

    float y = h * h / (2.0 * ph);
    float d = sqrt(h * h - y * y);
    res = min(res, k * d / max(0.0, t - y));
    ph = h;
    t += h;
  } 

  return min(res + 0.5, 1);
}

float HardShadow( in ray R, float Min, float Max )
{                 
  surface Mtl;

  for (float t = Min; t < Max;)
  {   
    float io = SceneSDF(RayApply(R, t), Mtl);
    float h = io;
   
    if (h < 0.001)
      return 0.5;
    
    t += h;
  } 

  return 1;
}

vec3 Shade( const vec3 Dir, const envi Media, intr I, float Weight )
{
  vec3 Color = vec3(0);
  float att;
  light_info li;
  intr Intr;
  int i = 0;
  
  vec3 R = reflect(Dir, I.N);
  vec3 N = faceforward(I.N, Dir, I.N);

  if (IsPointLgt)
    for (i = 0; i < PointLgtCnt; i++)
    { 
      att = ShadowPoint(li, PointLgt[i], I.P);

      float nl = max(dot(I.N, li.L), 0.0);
      float rl = max(dot(R, li.L), 0.0);

      Color += (li.Color * I.Mtl.Kd * nl + I.Mtl.Ks * li.Color * pow(rl, I.Mtl.Ph) * rl) * att;// * calcAO(I.P, I.N);

      ray Shadow = ray(I.P + li.L * 0.1, li.L);    
      //Color *= SoftShadow(Shadow, 0, 100, 4);
      Color *= HardShadow(Shadow, 0, 100);
    }

  if (IsDirLgt)
    for (i = 0; i < DirLgtCnt; i++)
    { 
      att = ShadowDir(li, DirLgt[i], I.P);

      float nl = max(dot(I.N, li.L), 0.0);
      float rl = max(dot(R, li.L), 0.0);

      Color += (li.Color * I.Mtl.Kd * nl + I.Mtl.Ks * li.Color * pow(rl, I.Mtl.Ph) * rl) * att * calcAO(I.P, I.N);

      ray Shadow = ray(I.P + li.L * 0.1, li.L);
      //Color *= SoftShadow(Shadow, 0, 100, 4);
      Color *= HardShadow(Shadow, 0, 100);
    }

  if (IsSpotLgt)
    for (i = 0; i < SpotLgtCnt; i++)
    { 
      att = ShadowSpot(li, SpotLgt[i], I.P);

      float nl = max(dot(I.N, li.L), 0.0);
      float rl = max(dot(R, li.L), 0.0);

      Color += (li.Color * I.Mtl.Kd * nl + I.Mtl.Ks * li.Color * pow(rl, I.Mtl.Ph) * rl) * att * calcAO(I.P, I.N);

      ray Shadow = ray(I.P + li.L * 0.1, li.L);    
      //Color *= SoftShadow(Shadow, 0, 100, 4);
      Color *= HardShadow(Shadow, 0, 100);
    }

  float eta1 = Media.RefractionCoef, eta2 = Media.RefractionCoef;
  vec3 T = (-N * dot(N, Dir) + Dir) * eta2 / eta1 -
    N * sqrt(1 - (1 - dot(N, Dir) * dot(N, Dir)) * eta2 * eta2 / (eta1 * eta1));

  RRay = ray(I.P + R * Threshold, R);
  
  return Color * Weight * 3;
}

vec3 Sky( vec3 e )
{
  e.y = (max(e.y, 0.0) * 0.8 + 0.2) * 0.8;
  return vec3(pow(1.0 - e.y, 2.0), 1.0 - e.y, 0.6 + (1.0 - e.y) * 0.4) * 1.1;
}

vec3 Trace( ray Ray, const envi Media, float Weight )
{
  vec3 color = vec3(0);
  intr Intr;
  int i = 0;
  float a = SphereTracing(Ray, 100, Intr);

  if (a != 0)
  {
    color = Shade(Ray.Dir, Media, Intr, Weight);
    color *= exp(-Intr.T * Media.DecayCoef);
  }
  
  return color;
}

void main( void )
{
  int RI_cnt = 2, i;
  vec3 Res = vec3(0);
  
  //for (i = 0; i < RI_cnt; i++)
  //{
  //  if (i == 0)
  //  {
  //    ray R = SetRay(DrawTexCoord.x * FrameW + 0.5, (1 - DrawTexCoord.y) * FrameH - 0.5);
  //    Res = Trace(R, Air, 7.5);
  //  }
  //  else
  //    Res += Trace(RRay, Air, 1 / float(i + 1) * 1.2);
  //}
  
  ray R = SetRay(DrawTexCoord.x * FrameW + 0.5, (1 - DrawTexCoord.y) * FrameH - 0.5);
  Res = Trace(R, Air, 7.5);

  OutColor = vec4(Res, 1);
}
