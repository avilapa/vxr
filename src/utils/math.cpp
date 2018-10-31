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

#include "../../include/utils/math.h"

namespace vxr 
{
  vec3 Math::lerp(vec3 a, vec3 b, float alpha) 
  {
    float ax = a[0], ay = a[1], az = a[2];
    vec3 out;
    out[0] = ax + alpha * (b[0] - ax);
    out[1] = ay + alpha * (b[1] - ay);
    out[2] = az + alpha * (b[2] - az);
    return out;
  }

  vec4 Math::lerp(vec4 a, vec4 b, float alpha)
  {
    float ax = a[0], ay = a[1], az = a[2], aw = a[3];
    vec4 out;
    out[0] = ax + alpha * (b[0] - ax);
    out[1] = ay + alpha * (b[1] - ay);
    out[2] = az + alpha * (b[2] - az);
    out[3] = aw + alpha * (b[3] - aw);
    return out;
  }

  float Math::inverseLerp(float a, float b, float value)
  {
    return ((value - a) / (b - a));
  }

}