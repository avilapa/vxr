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
// Lights
//--------------------------------------------------------------------------------

layout(std140) uniform Lights
{
  vec4 pos[MAX_LIGHT_SOURCES];
  vec4 dir_intensity[MAX_LIGHT_SOURCES];
  vec4 col_ambient[MAX_LIGHT_SOURCES];
} u_light;

vec3 getLightPosition(int index)
{
	return u_light.pos[index].xyz;
}

vec3 getLightDirection(int index)
{
	return u_light.dir_intensity[index].xyz;
}

vec3 getLightColor(int index)
{
	return u_light.col_ambient[index].xyz;
}

float getLightType(int index)
{
	return u_light.pos[index].w;
}

float getLightIntensity(int index)
{
	return u_light.dir_intensity[index].w;
}

float getLightAmbient(int index)
{
	return u_light.col_ambient[index].w;
}

//--------------------------------------------------------------------------------
// Phong
//--------------------------------------------------------------------------------

vec3 computeLightContribution()
{
  vec3 result = vec3(0);

  for (int i = 0; i < getNumLights(); ++i)
  {
    vec3 ambient = getLightAmbient(i) * getLightColor(i);
    
    vec3 norm = normalize(getWorldNormal());
    vec3 lightDir = normalize(getLightPosition(i) - getWorldPosition());
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * getLightColor(i);
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(getViewPosition() - getWorldPosition());
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * getLightColor(i);

    result += (ambient + diffuse + specular) * getLightIntensity(i);
  }

  return result;
}