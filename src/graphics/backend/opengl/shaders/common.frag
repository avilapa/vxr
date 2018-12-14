// -------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2018 Víctor Ávila
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Attributes
//--------------------------------------------------------------------------------

in vec3 in_world_position;
in vec3 in_world_normal;
in vec3 in_position;
in vec3 in_normal;
in vec2 in_uv;

vec3 getWorldPosition()
{
	return in_world_position;
}

vec3 getWorldNormal()
{
	return in_world_normal;
}

vec3 getPosition()
{
	return in_position;
}

vec3 getNormal()
{
	return in_normal;
}

vec2 getUV()
{
	return in_uv;
}

out vec4 out_fragment_color;

void setFragmentColor(vec4 out_color)
{
	out_fragment_color = out_color;
}

void setFragmentColor(vec3 out_color)
{
	out_fragment_color = vec4(out_color, 1.0);
}

//--------------------------------------------------------------------------------
// Uniforms
//--------------------------------------------------------------------------------

// Textures

#define MAX_TEXTURE_UNITS 16

uniform sampler1D u_tex1d0;
uniform sampler1D u_tex1d1;
uniform sampler1D u_tex1d2;
uniform sampler1D u_tex1d3;
uniform sampler1D u_tex1d4;
uniform sampler1D u_tex1d5;
uniform sampler1D u_tex1d6;
uniform sampler1D u_tex1d7;
uniform sampler1D u_tex1d8;
uniform sampler1D u_tex1d9;
uniform sampler1D u_tex1d10;
uniform sampler1D u_tex1d11;
uniform sampler1D u_tex1d12;
uniform sampler1D u_tex1d13;
uniform sampler1D u_tex1d14;
uniform sampler1D u_tex1d15;

uniform sampler2D u_tex2d0;
uniform sampler2D u_tex2d1;
uniform sampler2D u_tex2d2;
uniform sampler2D u_tex2d3;
uniform sampler2D u_tex2d4;
uniform sampler2D u_tex2d5;
uniform sampler2D u_tex2d6;
uniform sampler2D u_tex2d7;
uniform sampler2D u_tex2d8;
uniform sampler2D u_tex2d9;
uniform sampler2D u_tex2d10;
uniform sampler2D u_tex2d11;
uniform sampler2D u_tex2d12;
uniform sampler2D u_tex2d13;
uniform sampler2D u_tex2d14;
uniform sampler2D u_tex2d15;

uniform sampler3D u_tex3d0;
uniform sampler3D u_tex3d1;
uniform sampler3D u_tex3d2;
uniform sampler3D u_tex3d3;
uniform sampler3D u_tex3d4;
uniform sampler3D u_tex3d5;
uniform sampler3D u_tex3d6;
uniform sampler3D u_tex3d7;
uniform sampler3D u_tex3d8;
uniform sampler3D u_tex3d9;
uniform sampler3D u_tex3d10;
uniform sampler3D u_tex3d11;
uniform sampler3D u_tex3d12;
uniform sampler3D u_tex3d13;
uniform sampler3D u_tex3d14;
uniform sampler3D u_tex3d15;

uniform samplerCube u_texCube0;
uniform samplerCube u_texCube1;
uniform samplerCube u_texCube2;
uniform samplerCube u_texCube3;
uniform samplerCube u_texCube4;
uniform samplerCube u_texCube5;
uniform samplerCube u_texCube6;
uniform samplerCube u_texCube7;
uniform samplerCube u_texCube8;
uniform samplerCube u_texCube9;
uniform samplerCube u_texCube10;
uniform samplerCube u_texCube11;
uniform samplerCube u_texCube12;
uniform samplerCube u_texCube13;
uniform samplerCube u_texCube14;
uniform samplerCube u_texCube15;

// Common

uniform mat4 u_model;

mat4 getModelMatrix()
{
	return u_model;
}

layout(std140) uniform Common
{
  mat4 u_proj;
  mat4 u_view;

  vec2 u_resolution;
  vec2 u_xy;

  vec4 u_clear_color;

  vec4 u_view_pos_num_lights;
};

mat4 getViewMatrix()
{
	return u_view;
}

mat4 getProjectionMatrix()
{
	return u_proj;
}

vec2 getScreenResolution()
{
	return u_resolution;
}

/// u_xy

vec3 getViewPosition()
{
	return u_view_pos_num_lights.xyz;
}

int getNumLights()
{
	return int(u_view_pos_num_lights.w);
}