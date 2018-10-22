#pragma once

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

#include "../engine/types.h"

/**
* \file color.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Color structure.
*
*/
namespace vxr 
{

	struct Color 
  {
		Color();
		Color(const Color &o);
		Color(vec4 rgba);
		Color(float r, float g, float b, float a);
		~Color();

		vec4 rgba();
    vec3 rgb();

		float r, g, b, a;

    static const Color Random();

		static const Color Black;
		static const Color White;
		static const Color Transparent;
		static const Color Green;
		static const Color Blue;
		static const Color LightBlue;

		// Red
		static const Color Red;
		static const Color DarkRed;
		static const Color Salmon;
		static const Color LightSalmon;
		static const Color Crimson;
		static const Color FireBrick;

		// Pink
		static const Color Pink;
		static const Color HotPink;
		static const Color DeepPink;
		static const Color MediumVioletRed;
		static const Color PaleVioletRed;

		// Orange
		static const Color Orange;
		static const Color DarkOrange;
		static const Color OrangeRed;
		static const Color Tomato;
		static const Color Coral;

		// Yellow
		static const Color Yellow;
		static const Color Gold;
		static const Color LightYellow;
		static const Color Peach;
		static const Color Moccasin;

		// Purple
		static const Color Lavender;
		static const Color Violet;
		static const Color DarkViolet;
		static const Color Magenta;
		static const Color DarkMagenta;
		static const Color Purple;
		static const Color Indigo;
		static const Color SlateBlue;
		static const Color DarkSlateBlue;
		static const Color MediumSlateBlue;

		// Color codes source: http://htmlcolorcodes.com/color-names/

		static Color lerp(Color a, Color b, float alpha);

		void set_opacity(float opacity);

		bool equals(const Color& o, float epsilon = 1.e-4f);
		Color desaturate(float desaturation) const;
		float luminance() const;

		inline Color operator+(const Color& o) const { return Color(this->r + o.r, this->g + o.g, this->b + o.b, this->a + o.a); }
		inline Color& operator+=(const Color& o) { r += o.r; g += o.g; b += o.b; a += o.a; return *this; }
		inline Color operator-(const Color& o) const { return Color(this->r - o.r, this->g - o.g, this->b - o.b, this->a - o.a); }
		inline Color& operator-=(const Color& o) { r -= o.r; g -= o.g; b -= o.b; a -= o.a; return *this; }
		inline Color operator*(const Color& o) const { return Color(this->r * o.r, this->g * o.g, this->b * o.b, this->a * o.a); }
		inline Color& operator*=(const Color& o) { r *= o.r; g *= o.g; b *= o.b; a *= o.a; return *this; }
		inline Color operator*(float value) const { return Color(this->r * value, this->g * value, this->b * value, this->a * value); }
		inline Color& operator*=(float value) { r *= value; g *= value; b *= value; a *= value; return *this; }
		inline Color operator/(const Color& o) const { return Color(this->r / o.r, this->g / o.g, this->b / o.b, this->a / o.a); }
		inline Color& operator/=(const Color& o) { r /= o.r; g /= o.g; b /= o.b; a /= o.a; return *this; }
		inline Color operator/(float value) const { return Color(this->r / value, this->g / value, this->b / value, this->a / value); }
		inline Color& operator/=(float value) { r /= value; g /= value; b /= value; a /= value; return *this; }
		inline bool operator==(const Color& o) const { return this->r == o.r && this->g == o.g && this->b == o.b && this->a == o.a; }
		inline bool operator!=(const Color& o) const { return this->r != o.r || this->g != o.g || this->b != o.b || this->a != o.a; }
	};
}