#pragma once

#include <cassert>

// TODO allocate from pools for each type T
template<typename T>
class ArrayBuffer
{
public:
	inline ArrayBuffer() : _size(0), buffer(nullptr) {}
	inline ArrayBuffer(size_t size) : _size(size), buffer(nullptr)
	{
		if (_size > 0)
			buffer = new T[_size]; // constructors will be called
	}

	inline ~ArrayBuffer()
	{
		if (nullptr != buffer)
		{
			delete[] buffer; // destructors will be called
		}
	}

	// the possession of the resource and the responsibility to release the resource
	// cannot be shared, which means it cannot be copied, but can be moved

	// forbid copy
	ArrayBuffer(const ArrayBuffer<T>&) = delete;
	ArrayBuffer& operator = (const ArrayBuffer<T>&) = delete;

	// move sematics
	inline ArrayBuffer(ArrayBuffer<T>&& other)
		:
		_size(other._size)
		buffer(other.buffer)
	{
		other.buffer = nullptr;
	}

	inline ArrayBuffer<T>& operator = (ArrayBuffer<T>&& other)
	{
		if (this != &other)
		{
			if (nullptr != buffer)
				delete[] buffer;

			_size = other._size;
			buffer = other.buffer;

			other.buffer = nullptr;
		}
		return *this;
	}

	//
	inline size_t size() const { return _size; }
	inline bool empty() const { return _size == 0; }

	inline void release() {
		_size = 0;
		if (nullptr != buffer)
			delete[] buffer;
		buffer = nullptr;
	}

	inline operator T* () { return buffer; }

	inline operator const T* () const { return buffer; }

	inline T& operator [] (size_t index)
	{
		assert(index < _size);
		return buffer[index];
	}

	inline const T& operator [] (size_t index) const
	{
		assert(index < _size);
		return buffer[index];
	}

private:
	size_t		_size;
	T*			buffer;
};
