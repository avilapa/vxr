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

#include "../core/object.h"

/**
* \file gradient.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Color Gradient class.
*
*/
namespace vxr 
{

  class Gradient : public Object
  {
    VXR_OBJECT(Gradient, Object);

  public:
    Gradient();
    Gradient(const Gradient &o);
    ~Gradient();

    virtual void onGUI() override;

    struct Key
    {
      float value;
      Color color;
    };

    bool blend = true;

    void addKey(Gradient::Key key);
    void remKey(unsigned int index);
    Color evaluate(float value_between_zero_and_one);

    /// TODO: Add texture formats.
    unsigned char* textureData(const int texture_1d_resolution = 255, TexelsFormat::Enum format = TexelsFormat::RGB_U8);

    void randomizeColors();

    inline Gradient& operator=(const Gradient& o)
    {
      if (this != &o) 
      {
        this->keys_.clear();
        for (uint32 i = 0; i < o.keys_.size(); ++i)
        {
          Key k = o.keys_[i];
          this->keys_.push_back(k);
        }
      }
      return *this;
    }

    inline bool operator==(const Gradient& o) const 
    { 
      if (this->keys_.size() != o.keys_.size())
      {
        return false;
      }
      for (uint32 i = 0; i < o.keys_.size(); ++i)
      {
        if (this->keys_[i].color != o.keys_[i].color || this->keys_[i].value != o.keys_[i].value)
        {
          return false;
        }
      }
      return true;
    }

  private:
    std::vector<Gradient::Key> keys_;
  };

} /* end of vxr namespace */