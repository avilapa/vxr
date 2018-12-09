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

#version 330

//--------------------------------------------------------------------------------
// Attributes
//--------------------------------------------------------------------------------

in vec3 attr_position;
in vec3 attr_normal;
in vec2 attr_uv;

vec3 getPosition()
{
	return attr_position;
}

vec3 getNormal()
{
	return attr_normal;
}

vec2 getUV()
{
	return attr_uv;
}

out vec3 in_position;
out vec3 in_normal;
out vec2 in_uv;

//--------------------------------------------------------------------------------
// Uniforms
//--------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------

vec4 getClipPosition()
{
	return u_proj * u_view * u_model * vec4(attr_position, 1.0);
}

vec4 getClipPosition(vec3 offset)
{
	return u_proj * u_view * u_model * vec4(attr_position + offset, 1.0);
}

void setupWorldSpaceOutput()
{
	// World space position and normals
	in_position = vec3(u_model * vec4(attr_position, 1.0));
	/// Inverse Transposed may cause problems
  	in_normal = mat3(transpose(inverse(u_model))) * attr_normal; 
  	in_uv = attr_uv;
}

void setupScreenSpaceOutput()
{
	in_position = attr_position;
  	in_normal = attr_normal; 
  	in_uv = attr_uv;
}

void setupPositionOutput()
{
	in_position = attr_position;
}

void setupPositionOutput(vec3 position_output)
{
	in_position = position_output;
}

void setupNormalOutput()
{
	in_normal = attr_normal;
}

void setupNormalOutput(vec3 normal_output)
{
	in_normal = normal_output;
}

void setupUVOutput()
{
	in_uv = attr_uv;
}

void setupUVOutput(vec2 uv_output)
{
	in_uv = uv_output;
}