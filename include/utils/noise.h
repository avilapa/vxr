#pragma once

// ----------------------------------------------------------------------------------------
// 
// This file is part of libnoise-dotnet.
// libnoise-dotnet is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libnoise-dotnet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with libnoise-dotnet.  If not, see <http://www.gnu.org/licenses/>.
//
// Simplex Noise in 2D, 3D and 4D. Based on the example code of this paper:
// http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
//
// From Stefan Gustavson, Linkping University, Sweden (stegu at itn dot liu dot se)
// From Karsten Schmidt (slight optimizations & restructuring)
//
// Some changes by Sebastian Lague for use in a tutorial series.
//
//
// Ported from Unity to C++ by Víctor Ávila.
//
// ----------------------------------------------------------------------------------------

#include "../../include/core/object.h"
/*
 * Noise module that outputs 3-dimensional Simplex Perlin noise.
 * This algorithm has a computational cost of O(n+1) where n is the dimension.
 *
 * This noise module outputs values that usually range from
 * -1.0 to +1.0, but there are no guarantees that all output values will exist within that range.
*/

/**
* \file noise.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Simplex Noise.
*
* Noise module that outputs 3-dimensional Simplex Perlin noise.
* This algorithm has a computational cost of O(n+1) where n is the dimension.
*
* This noise module outputs values that usually range from
* -1.0 to +1.0, but there are no guarantees that all output values will exist within that range.
*
*/
namespace vxr {

  class Noise : public Object
  {

    VXR_OBJECT(Noise, Object);
  
  public:
    Noise();
    
    void init(int seed = 0);

    // Generates value, typically in range [-1, 1]
    float evaluate(vec3 point);
    
  private:
    // Unpack the given integer (int32) to an array of 4 bytes  in little endian format.
    // If the length of the buffer is too small, it will be resized.
    static unsigned char* unpackIntLittleEndian(uint32 value, unsigned char* buffer);

    const int RandomSize = 256;
    const double Sqrt3 = 1.7320508075688772935;
    const double Sqrt5 = 2.2360679774997896964;
    int *_random;

    // Skewing and unskewing factors for 2D, 3D and 4D, some of them pre-multiplied.
    const double F2 = 0.5*(Sqrt3 - 1.0);

    const double G2 = (3.0 - Sqrt3) / 6.0;
    const double G22 = G2 * 2.0 - 1;

    const double F3 = 1.0 / 3.0;
    const double G3 = 1.0 / 6.0;

    const double F4 = (Sqrt5 - 1.0) / 4.0;
    const double G4 = (5.0 - Sqrt5) / 20.0;
    const double G42 = G4 * 2.0;
    const double G43 = G4 * 3.0;
    const double G44 = G4 * 4.0 - 1.0;
  };

} /* end of vxr namespace */