#pragma once

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// Scoped_array, original code from boost.
// ----------------------------------------------------------------------------

#include <cassert>
#include <algorithm>

/**
* \file scoped_array.h
*
* \author Jose Luis Hidalgo
*
* \brief
*
*/
namespace vxr {

	template<class T, class T_index = unsigned int> class scoped_array {  // noncopyable
	public:

		typedef T element_type;

		scoped_array() : px_(NULL), elements_(0) {}

		~scoped_array() {
			delete[] px_;
		}

		T* alloc(size_t num_elements) {
			internal_reset(new T[num_elements], num_elements);
			return px_;
		}

		void release() {
			internal_reset(NULL, 0);
		}

		void copy(const T *begin, const T *end) {
			size_t elements = ((size_t)end - (size_t)begin) / sizeof(T);
			// Alloc memory but do not initialize it
			internal_reset(reinterpret_cast<T*>(new T[elements]), elements);
			// Use copy constructor to initialize all the elements
			for (size_t i = 0; i < elements; ++i) {
				new (px_ + i) T(begin[i]);
			}
		}

		void copy(const scoped_array<T> &other) {
			if (other.px_) {
				copy(&other[0], &other[0] + other.size());
			}
			else {
				internal_reset(NULL, 0);
			}
		}

		T & operator[](const T_index i) const {
			assert(px_ != 0 && "Invalid array, not allocated yet");
			assert(i >= 0 && "Invalid index (underflow)");
			assert(i < elements_ && "Invalid index (overflow)");
			return px_[i];
		}

		T* get() const {
			return px_;
		}

		// returns the number of elements
		size_t size() const { return elements_; }

		size_t sizeInBytes() const { return size() * sizeof(T); }

		bool valid() const {
			return px_ != 0;
		}

		void swap(scoped_array &b) {
			T *tmp = b.px_;
			b.px_ = px_;
			px_ = tmp;
			std::swap(b.elements_, elements_);
		}

		void memClear() {
			memset(px_, 0, sizeInBytes());
		}

		void memSet(const T &value) {
			T_index total = size();
			for (T_index i = 0; i < total; ++i) {
				(*this)[i] = value;
			}
		}

		void memCopy(const T_index index_offset, const T *from, const T_index elements_to_copy) {
			assert(elements_to_copy + index_offset <= size() && "Too many elements to copy");
			for (T_index i = 0; i < elements_to_copy; ++i) {
				(*this)[i + index_offset] = from[i];
			}
		}

	private:
		T* px_;
		T_index elements_;

		void internal_reset(T *p, const T_index num_elements) {
			assert(p == 0 || p != px_);  // catch self-reset errors
			this_type(p, num_elements).swap(*this);
		}

		scoped_array(T *p, const T_index num_elements)
			: px_(p), elements_(num_elements) {}

		typedef scoped_array<T, T_index> this_type;

		explicit scoped_array(scoped_array const &);
		scoped_array & operator=(scoped_array const &);
		void operator==(scoped_array const&) const;
		void operator!=(scoped_array const&) const;
	};

	template<class T> inline void swap(scoped_array<T> &a, scoped_array<T> &b) {
		a.swap(b);
	}

} /* end of vxr namespace */