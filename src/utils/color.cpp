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

#include "../../include/utils/color.h"

namespace vxr {

	const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
	const Color Color::Transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
	const Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
	const Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
	const Color Color::LightBlue = Color(0.0f, 1.0f, 1.0f, 1.0f);

	// Red
	const Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::DarkRed = Color(139.0f / 250.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::Salmon = Color(250.0f / 255.0f, 128.0f / 255.0f, 114.0f / 255.0f, 1.0f);
	const Color Color::LightSalmon = Color(1.0f, 160.0f / 255.0f, 122.0f / 255.0f, 1.0f);
	const Color Color::Crimson = Color(220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f, 1.0f);
	const Color Color::FireBrick = Color(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f, 1.0f);

	// Pink
	const Color Color::Pink = Color(255.0f / 255.0f, 192.0f / 255.0f, 203.0f / 255.0f, 1.0f);
	const Color Color::HotPink = Color(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f, 1.0f);
	const Color Color::DeepPink = Color(255.0f / 255.0f, 20.0f / 255.0f, 147.0f / 255.0f, 1.0f);
	const Color Color::MediumVioletRed = Color(199.0f / 255.0f, 21.0f / 255.0f, 133.0f / 255.0f, 1.0f);
	const Color Color::PaleVioletRed = Color(219.0f / 255.0f, 112.0f / 255.0f, 147.0f / 255.0f, 1.0f);

	// Orange
	const Color Color::Orange = Color(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	const Color Color::DarkOrange = Color(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	const Color Color::OrangeRed = Color(255.0f / 255.0f, 69.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	const Color Color::Tomato = Color(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f, 1.0f);
	const Color Color::Coral = Color(255.0f / 255.0f, 127.0f / 255.0f, 80.0f / 255.0f, 1.0f);

	// Yellow
	const Color Color::Yellow = Color(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	const Color Color::Gold = Color(255.0f / 255.0f, 215.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	const Color Color::LightYellow = Color(255.0f / 255.0f, 255.0f / 255.0f, 224.0f / 255.0f, 1.0f);
	const Color Color::Peach = Color(255.0f / 255.0f, 218.0f / 255.0f, 185.0f / 255.0f, 1.0f);
	const Color Color::Moccasin = Color(255.0f / 255.0f, 228.0f / 255.0f, 181.0f / 255.0f, 1.0f);

	// Purple
  /// Add all colors!

	Color::Color() 
  {
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 1.0f;
	}

	Color::Color(const Color &o) 
  {
		r = o.r;
		g = o.g;
		b = o.b;
		a = o.a;
	}

	Color::Color(vec4 rgba) 
  {
		r = rgba.r;
		g = rgba.g;
		b = rgba.b;
		a = rgba.a;
	}

	Color::Color(float red, float green, float blue, float alpha) 
  {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	Color::~Color() 
  {
  }

	vec4 Color::rgba() 
  {
		return vec4(r, g, b, a);
	}

 vec3 Color::rgb() 
 {
   return vec3(r, g, b);
 }

	const Color Color::Random() 
  {
		return Color(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
	}

	void Color::set_opacity(float opacity) 
  {
		a = opacity;
	}

	bool Color::equals(const Color& o, float epsilon) 
  {
		return glm::abs(this->r - o.r) < epsilon &&
			glm::abs(this->g - o.g) < epsilon &&
			glm::abs(this->b - o.b) < epsilon &&
			glm::abs(this->a - o.a) < epsilon;
	}

	float Color::luminance() const
  {
		return r * 0.3f + g * 0.59f + b * 0.11f;
	}

	Color Color::desaturate(float desaturation) const 
  {
		float lum = luminance();
		return lerp(*this, Color(lum, lum, lum, 0), desaturation);
	}

	Color Color::lerp(Color a, Color b, float alpha) 
  {
		Color c = b - a;
		c *= alpha;
		c += a;
		return c;
	}
}