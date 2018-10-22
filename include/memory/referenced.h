#pragma once

// ----------------------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// Referenced Class.
// ----------------------------------------------------------------------------------------

#include <cassert>
#include <stdio.h>

/**
* \file referenced.h
*
* \author Jose Luis Hidalgo
*
* \brief Reference count class
*
*/
namespace vxr 
{

  class Referenced {
  public:
    unsigned int ref_counter() const { return ref_counter_; }
    void ref();
    void unref();
    void unref_noDelete();

  protected:
    Referenced() : ref_counter_(0) {}
    virtual ~Referenced() {}

  private:
    unsigned int ref_counter_;
    Referenced(const Referenced &);
	  Referenced& operator=(const Referenced &);

  };

  inline void Referenced::ref() {
    ++ref_counter_;
  }

  inline void Referenced::unref() {
    assert(ref_counter_ > 0);///
    --ref_counter_;
    if (ref_counter_ == 0) {
      delete this;
    }
  }
  
  inline void Referenced::unref_noDelete() {
    assert(ref_counter_ > 0);
    --ref_counter_;
  }

} /* end of vxr namespace */

