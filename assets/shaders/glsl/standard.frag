// ----------------------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------------------

#version 330

in vec3 position;
in vec3 normal;
in vec2 uv;

layout(std140) uniform Uniforms
{
  vec4 color;
};

uniform sampler2D u_tex0;

out vec4 fragColor;

vec3 lightPos = vec3(0.5,0.3,-1.8); 
vec3 viewPos = vec3(0,0,-1.8); 
vec3 lightColor = vec3(1,1,1);

void main()
{
  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
  
  // diffuse 
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - position);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  
      
  vec3 result = (ambient + diffuse + specular) * color.xyz;
  fragColor = vec4(result, 1.0);
}