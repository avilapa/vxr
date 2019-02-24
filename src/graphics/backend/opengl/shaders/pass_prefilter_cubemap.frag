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

layout(std140) uniform CubemapConvolution
{
	mat4 conv_proj;
	mat4 conv_view;
	vec4 aux; // x - Roughness
};

const uint SAMPLE_COUNT = 1024u;
const float INVERSE_SAMPLE_COUNT = 1.0 / float(SAMPLE_COUNT);

vec3 prefilterCubemap()
{
    vec3 prefilteredColor = vec3(0.0);
	vec3 N = normalize(getPosition());

    // Karis 2003, "Real Shading in Unreal Engine 4"
	// Assumption V = N: no stretching at grazing angles.
	vec3 V = N;
    vec3 R = N;

	float totalWeight = 0.0;
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		// Generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
		vec2 U = Hammersley(i, INVERSE_SAMPLE_COUNT);
		vec3 H = D_GGX_HemisphereImportanceSample(aux.x, U, N);
		vec3 L = normalize(2.0 * dot(V, H) * H - V);

		float NoL = max(dot(N, L), 0.0);

		if (NoL > 0.0)
		{
			// Sample from the environment's mip level based on roughness/pdf
            float NoH = max(dot(N, H), 0.0);
            float HoV = max(dot(H, V), 0.0);

            float D   = D_GGX(aux.x, NoH);
            float pdf = D * NoH / (4.0 * HoV) + 0.0001; 

            float resolution = 512.0; // Resolution of source cubemap (per face).
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = (aux.x == 0.0) ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefilteredColor += textureLod(u_texCube0, L, mipLevel).rgb * NoL;
            totalWeight     += NoL;
		} 
	}

	return prefilteredColor / totalWeight;
}

void main()
{
  	setFragmentColor(prefilterCubemap());
}