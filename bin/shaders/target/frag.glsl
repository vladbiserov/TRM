/**/

#version 450

layout(location = 0) out vec4 OutColor;

layout(binding = 0) uniform sampler2D Tex0;
layout(binding = 1) uniform sampler2D Tex1;
layout(binding = 2) uniform sampler2D Tex2;

in vec2 DrawTexCoord;

void main( void )
{
  OutColor = vec4(texelFetch(Tex0, ivec2(gl_FragCoord.xy), 0));
}

