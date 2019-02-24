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

vec3 computeIrradiance()
{
	vec3 irradiance = vec3(0);
	vec3 N = normalize(getPosition());
	
	// Tangent space calculation from origin point.
	vec3 up = vec3(0,1,0);
	vec3 right = cross(up, N);
	up = cross(N, right);

	float delta = 0.025;
	float numSamples = 0.0;

	for (float phi = 0.0; phi < 2.0 * PI; phi += delta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += delta)
		{
			// Convert spherical to cartesian.
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// Convert tangent to world space.
			vec3 sampleVector = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += texture(u_texCube0, sampleVector).rgb * cos(theta) * sin(theta);
			numSamples++;
		}
	}

	return PI * irradiance * (1.0 / float(numSamples));
}

void main()
{
  	setFragmentColor(computeIrradiance());
}