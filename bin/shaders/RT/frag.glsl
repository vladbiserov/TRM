#version 430

#define Threshold float(0.001)
#define HUGE_VAL float(1e+38)

//painting color
layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 Dist;

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

layout(location = 0) uniform samplerCube skybox;
layout(origin_upper_left) in vec4 gl_FragCoord;

layout(binding = 1) uniform sampler2D Tex1;
uniform bool IsTexture1;

in vec2 DrawTexCoord;

struct ray
{
  vec3 Dir;
  vec3 Org;
  vec3 Color;
  float n, Weight, Kr;
  bool IsSky, IsInObj;
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

struct mtl
{
  vec3 Albedo;
  float roughness, metallic;
};

struct sphere
{
  vec3 C;
  float R;         
};

struct box
{
  vec3 C;
  vec3 R;     
};

struct plane
{
  vec3 N;
  float D;
};

struct torus
{
  vec3 C;
  vec3 N;
  float k1, k2;
};

struct ellipsoid 
{ 
  vec3 C; 
  vec3 R;     
}; 

struct cylinder
{
  vec3 P1;
  float R1;
  vec3 P2;
  float R2;   
};

struct capsule 
{ 
  vec3 P1; 
  vec3 P2; 
  float R;     
}; 

struct hollow_sphere
{
  vec3 C;
  float r, h, t; 
};

struct sea
{
  float h, amp;
  int n;
};

const mtl MtlLib[] = {
  {vec3(0.5, 0.5, 0.0), 0.5, 0.7},
  {vec3(0.9, 0.1, 0.3), 0.3, 0.8},
};


const float PI = 3.14159;
/*const bool IsSkybox = true;
const bool IsReflection = true;
const bool IsShadows = true;
const bool IsAO = false;    */
const bool IsSkybox = true;
const bool IsReflection = true;
const bool IsShadows = true;
const bool IsAO = false;

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
  vec3 C = cross(Right, Dir) * (-Sy + FrameH / 2) * Hp / FrameH;
  vec3 X = (A + B) + C;
  Ray.Org = Loc + X;
  Ray.Dir = normalize(X);
  Ray.Color = vec3(0);
  Ray.n = 1;
  Ray.Weight = 1;
  Ray.IsSky = false;
  Ray.IsInObj = true;
  Ray.Kr = 1;

  return Ray;
}

vec3 RayApply( ray R, float T )
{
  return R.Org + R.Dir * T;
}

vec3 Gamma( vec3 Color, float g )
{
  return vec3(pow(Color.x, g), pow(Color.y, g), pow(Color.z, g));
}

float SDFSphere( vec3 P, sphere S, out vec2 TexCoord )
{
  TexCoord = vec2(1 - atan((P.z - S.C.z) / S.R, (P.x - S.C.x) / S.R) / PI, acos((P.y - S.C.y) / S.R));
  return length(P - S.C) - S.R;
}

float SDFBox( vec3 P, box B, out vec2 TexCoord )
{
  vec3 d = abs(P - B.C) - B.R;
  TexCoord = vec2(0);
  
  if (abs(P.z + B.R.z - B.C.z) < Threshold || abs(P.z - B.R.z - B.C.z) < Threshold)
    TexCoord = vec2(P.x - B.C.x, -P.y + B.C.y) / 4;
  else if (abs(P.x + B.R.x - B.C.x) < Threshold || abs(P.x - B.R.x - B.C.x) < Threshold)
    TexCoord = vec2(P.z - B.C.z, -P.y + B.C.y) / 4;
  else if (abs(P.y + B.R.y - B.C.y) < Threshold || abs(P.y - B.R.y - B.C.y) < Threshold)
    TexCoord = vec2(P.x - B.C.x, P.z - B.C.z) / 4;

  return min(max(d.x, max(d.y,d.z)), 0.0) + length(max(d, 0.0));
}

float SDFPlane( vec3 P, plane Pl, out vec2 TexCoord )
{
  TexCoord = vec2(0);
  vec3 N2, N3;
                    
  if (Pl.N.x == 1)
    N2 = vec3(0, 0, 1);
  else
    N2 = vec3(1, 0, 0);
  N3 = normalize(cross(Pl.N, N2));
  N2 = normalize(cross(N3, Pl.N));
  TexCoord = vec2(dot(P - Pl.N * Pl.D, N2) / 4, -dot(P - Pl.N * Pl.D, N3) / 4);

  return dot(P, Pl.N) - Pl.D;
}

float SDFTorus( vec3 P, torus T, out vec2 TexCoord ) 
{
  TexCoord = vec2(0);
  return distance(P, T.C + T.k1 * normalize(cross(cross(T.N, (P - T.C)), T.N))) - T.k2;
}

float SDFEllipsoid( vec3 P, ellipsoid E, out vec2 TexCoord ) 
{ 
  float k0 = length((P - E.C) / E.R); 
  float k1 = length((P - E.C) / (E.R * E.R)); 

  TexCoord = vec2(0);

  return k0 * (k0 - 1.0) / k1; 
}

float SDFCylinder( vec3 P, cylinder C, out vec2 TexCoord )
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

  TexCoord = vec2(0);

  return s * sqrt(min(cax * cax + cay * cay * baba,
                      cbx * cbx + cby * cby * baba));
}

float SDFCapsule( vec3 P, capsule C, out vec2 TexCoord ) 
{ 
  vec3 pa = P - C.P1, ba = C.P2 - C.P1; 
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0); 
   
  TexCoord = vec2(0);

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

float SDFSea( vec3 P, sea S )
{
  float freq = 0.16;
  float amp = S.amp;
  float choppy = 4;
  vec2 uv = P.xz; 
  uv.x *= 0.75;

  float d, h = S.h;

  for (int i = 0; i < S.n; i++)
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
 
float SDFUnionSmooth( float distA, float distB, float k )
{
  float h = clamp(0.5 + 0.5 * (distA - distB) / k, 0., 1.);
  return mix(distA, distB, h) - k * h * (1. - h); 
}

float SDFInterSmooth( float distA, float distB, float k ) 
{
  float h = clamp(0.5 - 0.5 * (distA - distB) / k, 0., 1.);
  return mix(distA, distB, h ) + k * h * (1. - h); 
}
 
float SDFDiferSmooth( float distA, float distB, float k )
{
  float h = clamp(0.5 - 0.5 * (distB + distA) / k, 0., 1.);
  return mix(distA, -distB, h) + k * h * (1. - h); 
}

mtl SDFSurfaceSmoothUnion( float k1, mtl surface1, float k2, in mtl surface2, float smoothness )
{
  float interpolation = clamp(0.5 + 0.5 * (k1 - k2) / smoothness, 0.0, 1.0);
  return mtl(mix(surface2.Albedo, surface1.Albedo, 1 - interpolation),
                 mix(surface2.roughness, surface1.roughness, 1 - interpolation),
                 mix(surface2.metallic, surface1.metallic, 1 - interpolation));
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

float SceneSDF( in vec3 point, inout mtl Mtl )
{ 
float res, tmp;

mtl tmp_mtl;

// add shape 'a2'
float a2;
mtl mtl_a2;
vec3 mod_a2 = point;
vec2 tex_a2;

// add mtl 'a'
mtl a;

// add shape 'a4'
float a4;
mtl mtl_a4;
vec3 mod_a4 = point;
vec2 tex_a4;

// add shape 'am'
float am;
mtl mtl_am;
vec3 mod_am = point;
vec2 tex_am;

// apply SDF function to 'a2'
a2 = SDFPlane(mod_a2, plane(vec3(0,1,0), 0), tex_a2);
mtl_a2 = MtlLib[0];
mtl_a2.Albedo = texture(Tex1, tex_a2).bgr;

// set mtl value to 'a'
a = mtl(vec3(abs(sin((Time*5.500000))), 0.000000, abs(cos((Time*4.000000)))), 1.000000, 0.500000);

// apply SDF function to 'a4'
a4 = SDFBox(mod_a4, box(vec3(sin(Time)*8,abs(cos(Time))*8,0), vec3(2)), tex_a4);
mtl_a4 = a;

// apply modification function to 'a4'
mod_a4 = Rotate(100, vec3(0,1,0), mod_a4);
a4 = SDFBox(mod_a4, box(vec3(sin(Time)*8,abs(cos(Time))*8,0), vec3(2)), tex_a4);
mtl_a4 = a;

// apply SDF function to 'am'
am = SDFSea(mod_am, sea(0, 0.6, 3));
mtl_am = a;

// add to scene 'am' var
res = am;
Mtl = mtl_am;


return res;
}

vec3 SDFSceneNormal( vec3 P )
{
  float EPSILON = Threshold;
  mtl Mtl;

  float a = SceneSDF(vec3(P.x + EPSILON, P.y, P.z), Mtl),
        b = SceneSDF(vec3(P.x - EPSILON, P.y, P.z), Mtl),
        c = SceneSDF(vec3(P.x, P.y + EPSILON, P.z), Mtl),
        d = SceneSDF(vec3(P.x, P.y - EPSILON, P.z), Mtl),
        e = SceneSDF(vec3(P.x, P.y, P.z + EPSILON), Mtl),
        f = SceneSDF(vec3(P.x, P.y, P.z - EPSILON), Mtl);


  return normalize(vec3(a - b, c - d, e - f));
}

bool IsPointLgt = false;
const int PointLgtCnt = 1;
const point_light PointLgt[PointLgtCnt] = {point_light(vec3(13, 8, 8), vec3(1, 0.7, 0.47), 0.4, 0.6, 0.06)};
bool IsDirLgt = true;
const int DirLgtCnt = 1;
const dir_light DirLgt[DirLgtCnt] = {dir_light(vec3(0,1,1), vec3(1,1,1))}; 
bool IsSpotLgt = false;
const int SpotLgtCnt = 1;
const spot_light SpotLgt[SpotLgtCnt] = {spot_light(vec3(1, 2, 1), vec3(0, 1, 0), vec3(0.1), cos(0.1), cos(1.1))};

vec3 SkyboxColor( void )
{
  vec2 ScreenCoords = vec2(DrawTexCoord.x - 0.5, DrawTexCoord.y - 0.5);
  ScreenCoords *= vec2(clamp(FrameH / FrameW, 0.47, 1.0), clamp(FrameW / FrameH, 0.47, 1.0));
  ScreenCoords *= 1.8;

  vec3 SkyDir = normalize(Dir) + normalize(Right) * ScreenCoords.x + normalize(cross(Right, Dir)) * ScreenCoords.y;

  return texture(skybox, normalize(SkyDir)).bgr;
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

vec3 fresnelSchlick( float cosTheta, vec3 F0 )
{
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX( vec3 N, vec3 H, float roughness )
{
  float a      = roughness * roughness;
  float a2     = a * a;
  float NdotH  = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;
	
  float num   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;
	
  return num / denom;
}

float GeometrySchlickGGX( float NdotV, float roughness )
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float num   = NdotV;
  float denom = NdotV * (1.0 - k) + k;
	
  return num / denom;
}

float GeometrySmith( vec3 N, vec3 V, vec3 L, float roughness )
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2  = GeometrySchlickGGX(NdotV, roughness);
  float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
  return ggx1 * ggx2;
}

vec3 BRDF( vec3 n, vec3 l, vec3 v, float ao, float att, mtl Mtl )
{
  vec3 F0 = vec3(0.04); 
  F0 = mix(F0, Mtl.Albedo, Mtl.metallic);

  vec3 h = normalize(v + l);
  float NDF = DistributionGGX(n, h, Mtl.roughness);        
  float G   = GeometrySmith(n, v, l, Mtl.roughness);      
  vec3 F    = fresnelSchlick(max(dot(h, v), 0.0), F0);       
  
  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - Mtl.metallic;	  
  
  vec3 numerator    = NDF * G * F;
  float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + Threshold;
  vec3 specular     = numerator / denominator;  
      
  // add to outgoing radiance Lo
  float NdotL = max(dot(n, l), 0.0);                
  vec3 Lo = (kD * Mtl.Albedo / PI + specular) * NdotL;      

  vec3 ambient = vec3(0.03) * Mtl.Albedo * ao;
  vec3 color = ambient + Lo * att;
	
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));

  return color;
}

vec3 Tonemap_ACES( const vec3 x )
{  
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float calcAO( vec3 P, vec3 N )
{
  float occ = 0.0;
  mtl Mtl;
  float sca = 1.0;

  for (int i = 0; i < 5; i++)
  {
    float h = 0.01 + 0.12 * float(i) / 4.0;
    float a = SceneSDF(P + h * N, Mtl);

    occ += (h - a.x) * sca;
    sca *= 0.95;
    if (occ > 0.35)
      break;
  }

  return clamp(1.0 - 3.0 * occ, 0.0, 1.0) * (0.5 + 0.5 * N.y);
}

float HardShadow( in ray R, float Min, float Max )
{                 
  mtl Mtl;

  for (float t = Min; t < Max;)
  {   
    float io = SceneSDF(RayApply(R, t), Mtl);
    float h = io;
   
    if (abs(h) < 0.001)
      return 0.65;
    
    t += h;
  } 

  return 1;
}

float SoftShadow( in ray R, float Min, float Max, float k )
{
  float res = 1.0;
  float ph = 1e20;
  mtl Mtl;

  for (float t = Min; t < Max;)
  {   
    float io = SceneSDF(RayApply(R, t), Mtl);
    float h = io;
   
    if (h < 0.001)
      return 0.65;

    float y = h * h / (2.0 * ph);
    float d = sqrt(h * h - y * y);
    res = min(res, k * d / max(0.0, t - y));
    ph = h;
    t += h;
  } 

  return clamp(res, 0.65, 1.0);
}

vec3 Shade( inout ray R, vec3 P, vec3 N, mtl Mtl )
{
  vec3 color = vec3(0);
  float AO = 0.8;

  if (IsAO)
    AO = calcAO(P, N);

  if (IsPointLgt)
  { 
    for (int i = 0; i < PointLgtCnt; i++)
    {
      light_info li;

      float att = ShadowPoint(li, PointLgt[i], P);

      if (IsShadows)
      {
        ray Shadow;
        Shadow.Org = P + li.L * 0.1;
        Shadow.Dir = li.L;
      
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * HardShadow(Shadow, 0, 100) * li.Color;
      }
      else
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * li.Color;
    }
  }

  if (IsDirLgt)
  { 
    for (int i = 0; i < DirLgtCnt; i++)
    {
      light_info li;

      float att = ShadowDir(li, DirLgt[i], P);

      if (IsShadows)
      {
        ray Shadow;
        Shadow.Org = P + li.L * 0.1;
        Shadow.Dir = li.L;
      
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * HardShadow(Shadow, 0, 100) * li.Color;
      }
      else
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * li.Color;
    }
  }

  if (IsSpotLgt)
  { 
    for (int i = 0; i < SpotLgtCnt; i++)
    {
      light_info li;

      float att = ShadowSpot(li, SpotLgt[i], P);

      if (IsShadows)
      {
        ray Shadow;
        Shadow.Org = P + li.L * 0.1;
        Shadow.Dir = li.L;
      
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * HardShadow(Shadow, 0, 100) * li.Color;
      }
      else
        color += BRDF(N, li.L, -R.Dir, AO, att, Mtl) * li.Color;
    }
  }

  vec3 Ref = reflect(R.Dir, N);
  R.Org = P + Ref * 0.01;
  R.Dir = Ref;

  return Tonemap_ACES(color);
}

void SphereTracing( inout ray R, float MaxDist )
{
  float t = 0;
  float io;
  mtl Mtl;

  while (t < MaxDist)
  {
    io = SceneSDF(RayApply(R, t), Mtl);

    if (abs(io) <= Threshold)
    { 
      vec3 P = RayApply(R, t);
      vec3 N = SDFSceneNormal(P);
      R.Color += Shade(R, P, N, Mtl) * R.Weight * R.Kr;
      R.Kr = 1 - Mtl.roughness;
      R.Weight *= 0.5;
      return;
    }

    t += io;
  }

  R.IsSky = true;
  R.Color += texture(skybox, normalize(R.Dir)).bgr * R.Weight * float(IsSkybox) * R.Kr;
}

vec3 Render( void )
{
  int RI_cnt = 1 + int(IsReflection);
  ray R = SetRay(DrawTexCoord.x * FrameW + 0.5, (1 - DrawTexCoord.y) * FrameH - 0.5);

  SphereTracing(R, 100);

  for (int i = 0; i < RI_cnt - 1; i++)
  {
    if (R.IsSky)
      break;
    SphereTracing(R, 100);
  }

  return R.Color;
}

void main( void )
{ 
  OutColor = vec4(Render(), 1);
}
