/**/

#version 450

layout(location = 0) out vec4 OutColor;

layout(binding = 0) uniform sampler2D Tex0;
uniform bool IsTexture0;

in vec2 DrawTexCoord;

void main( void )
{
  vec4 P1 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(-1, 1), 0)),
    P2 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(0, 1), 0)),
    P3 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(1, 1), 0)),
    P4 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(-1, 0), 0)),
    P5 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy), 0)),
    P6 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(1, 0), 0)),
    P7 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(-1, -1), 0)),
    P8 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(0, -1), 0)),
    P9 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(1, -1), 0)),
    P10 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(2, 1), 0)),
    P11 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(2, 0), 0)),
    P12 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(2, -1), 0)),
    P13 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(2, -2), 0)),
    P14 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(1, -2), 0)),
    P15 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(0, -2), 0)),
    P16 = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy) + ivec2(-1, -2), 0));

  OutColor = (P1 + P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9 + P10 + P11 + P12 + P13 + P14 + P15 + P16) / 16;
  //if (IsTexture0)
  //OutColor = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy), 0));//(P1 + P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9) / 9;
  //OutColor = vec4(1);
}

