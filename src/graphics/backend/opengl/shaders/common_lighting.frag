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
  vec4 position_falloff[MAX_LIGHT_SOURCES];
  vec4 color_intensity[MAX_LIGHT_SOURCES];
  vec4 direction_ambient[MAX_LIGHT_SOURCES];
} u_light;

const float cone_angle = 50.0;

vec3 getLightPosition(int index)
{
	return u_light.position_falloff[index].xyz;
}

float getLightFalloff(int index)
{
  return u_light.position_falloff[index].w;
}

bool getLightIsDirectional(int index)
{
  return (u_light.position_falloff[index].w == 0.0);
}

vec3 getLightDirection(int index)
{
	return u_light.direction_ambient[index].xyz;
}

vec3 getLightColor(int index)
{
	return u_light.color_intensity[index].xyz;
}

float getLightIntensity(int index)
{
	return u_light.color_intensity[index].w;
}

float getLightAmbient(int index)
{
	return u_light.direction_ambient[index].w;
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

  return applyGammaCorrection(result);
}

//--------------------------------------------------------------------------------
// PBR
//--------------------------------------------------------------------------------

float getSquareFalloffAttenuation(float distanceSquare, float falloff) 
{
    float factor = distanceSquare * falloff;
    float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
    return smoothFactor * smoothFactor;
}

float getDistanceAttenuation(const vec3 positionToLight, float falloff)
{
  float distanceSquare = dot(positionToLight, positionToLight);
  float attenuation = getSquareFalloffAttenuation(distanceSquare, falloff);
  return attenuation * 1.0 / max(distanceSquare, 1e-4);
}

float getDistanceAttenuationFast(const vec3 positionToLight)
{
  // The dot product of a vector with itself gives the squared length
  // of that vector.
  float distanceSquare = dot(positionToLight, positionToLight);
  return 1.0 / max(distanceSquare, 1e-4);
}

Light getLight(int index)
{
  /// Evaluate visibility at per light level for shadowing directional/punctual
  Light light;
  light.color = getLightColor(index);
  light.intensity = getLightIntensity(index); /// Multiply by the exposure
  light.attenuation = light.intensity;
  light.NoL = clamp(dot(getWorldNormal(), light.L), 0.0, 1.0);
  if (getLightIsDirectional(index))
  {
    // Directional Light
    light.L = normalize(getLightDirection(index));
    /// sample sun area?
  }
  else
  {
    // Punctual Light
    vec3 positionToLight = getLightPosition(index) - getWorldPosition(); 
    light.L = normalize(positionToLight);
    
    /// intensity missing
    light.attenuation = getDistanceAttenuation(positionToLight, getLightFalloff(index));
    //light.attenuation = getDistanceAttenuationFast(positionToLight); 
    
    float light_to_surface_angle = degrees(acos(dot(-light.L, normalize(getLightDirection(index)))));
    if (light_to_surface_angle > cone_angle) 
    {
      //attenuation = 0.0;
    }
    /// cone angle vs scale offset
  }
  return light;
}

vec3 evaluateIBL(const MaterialInput material, const PixelParameters pixel, const ShadingParameters shading)
{
  return surfaceShadingIBL(material, pixel, shading);
}

vec3 evaluateLights(const MaterialInput material, const PixelParameters pixel, const ShadingParameters shading)
{
  vec3 color = vec3(0.0);

  for (int i = 0; i < getNumLights(); i++)
  {
    Light light = getLight(i);

    float visibility = 1.0;
    /// Calculate shadows

    color += surfaceShading(pixel, shading, light, visibility);
  }

  return color;
}