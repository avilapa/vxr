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

const uint SAMPLE_COUNT = 1024u;
const float INVERSE_SAMPLE_COUNT = 1.0 / float(SAMPLE_COUNT);

vec2 integrateBRDF(float NoV, float roughness)
{
    vec2 R = vec2(0);
    vec3 V = vec3(sqrt(1.0 - NoV * NoV), 0.0, NoV);
    vec3 N = vec3(0.0, 0.0, 1.0);

    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // Generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        vec2 U = Hammersley(i, INVERSE_SAMPLE_COUNT);
        vec3 H = D_GGX_HemisphereImportanceSample(roughness, U, N);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NoL = max(L.z, 0.0);

        if(NoL > 0.0)
        {
            float NoH = max(H.z, 0.0);
            float VoH = max(dot(V, H), 0.0);
            float NoL = max(dot(N, L), 0.0);

            float G = (V_SmithGGX_DFG(roughness, NoV, NoL) * VoH) / (NoH * NoV);
            float Fc = pow(1.0 - VoH, 5.0);

        #if USE_MULTIPLE_SCATTERING
            R.x += G * Fc;
            R.y += G;
        #else
            R.x += G * (1.0 - Fc);
            R.y += G * Fc;
        #endif
        }
    }
    
    return R * INVERSE_SAMPLE_COUNT;
}

void main()
{
  	setFragmentColor(vec3(integrateBRDF(getUV().x, getUV().y), 0.0));
}