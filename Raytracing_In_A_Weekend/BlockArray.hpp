#pragma once

#include <cstddef>	 //std::size_t
#include <stdexcept> // std::out_of_range
#include <cstring>	 //memcpy

#define UNUSED(m_x) (void)m_x;

template <typename T>
class ArrayView
{
public:
	ArrayView(T* m_arr, size_t len) : m_arr(m_arr)
#ifdef _DEBUG
		, length(len)
#endif
	{
#ifndef _DEBUG
		UNUSED(len);
#endif
	}

	T& operator[](size_t index)
	{
#ifdef _DEBUG
		if (index >= length)
		{
			throw std::out_of_range("Array View Accessed Out Of Bounds");
		}
#endif
		return m_arr[index];
	}

	const T& operator[](size_t index) const
	{
#ifdef _DEBUG
		if (index >= length)
		{
			throw std::out_of_range("Array View Accessed Out Of Bounds");
		}
#endif
		return m_arr[index];
	}
private:
	T* m_arr;
#ifdef _DEBUG
	const size_t length;
#endif
};

template <typename T>
class BlockArray2D
{
public:
	BlockArray2D(size_t dim_1_in, size_t dim_2_in) : m_arr(new T[dim_1_in * dim_2_in]), m_dim_1(dim_1_in), m_dim_2(dim_2_in) {};

	BlockArray2D(BlockArray2D& other) : m_arr(new T[other.m_dim_1, other.m_dim_2]), m_dim_1(other.m_dim_1), m_dim_2(other.m_dim_2)
	{
		for (size_t i = 0; i < m_dim_1 * m_dim_2; i++)
		{
			m_arr[i] = std::copy(other.m_arr[i]);
		}
	};

	// Copy assignment
	// Do deep copy of a.m_ptr to m_ptr
	BlockArray2D& operator=(const BlockArray2D& other)
	{
		// Self-assignment detection
		if (&other == this)
			return *this;

		// Release any resource we're holding
		delete m_arr;

		// Copy the resource
		m_arr = new T[other.m_dim_1, other.m_dim_2];
		m_dim_1 = other.m_dim_1;
		m_dim_2 = other.m_dim_2;

		for (size_t i = 0; i < m_dim_1 * m_dim_2; i++)
		{
			m_arr[i] = std::copy(other.m_arr[i]);
		}

		return *this;
	}

	// Move Constructor
	// Transfer ownership of a.m_ptr to m_ptr
	BlockArray2D(BlockArray2D&& other) noexcept : m_arr(other.m_arr), m_dim_1(other.m_dim_1), m_dim_2(other.m_dim_2) {
		// Remove ownership from other
		other.m_arr = nullptr;
		other.m_dim_1 = 0;
		other.m_dim_2 = 0;
	};

	// Move assignment
	// Transfer ownership of a.m_ptr to m_ptr
	BlockArray2D& operator=(BlockArray2D&& other) noexcept {
		// Self-assignment detection
		if (&other == this)
			return *this;

		// Release any resource we're holding
		delete m_arr;

		// Transfer ownership of other.m_arr to self
		m_arr = other.m_arr;
		m_dim_1 = other.m_dim_1;
		m_dim_2 = other.m_dim_2;

		// Remove ownership from other
		other.m_arr = nullptr;
		other.m_dim_1 = 0;
		other.m_dim_2 = 0;

		return *this;
	}

	ArrayView<T> operator[](size_t index)
	{
#ifdef _DEBUG
		if (index >= m_dim_1)
		{
			throw std::out_of_range("Array View Accessed Out Of Bounds");
		}
#endif
		return ArrayView<T>(&m_arr[index * m_dim_2], m_dim_2);
	}

	const ArrayView<T> operator[](size_t index) const
	{
#ifdef _DEBUG
		if (index >= m_dim_1)
		{
			throw std::out_of_range("Array View Accessed Out Of Bounds");
		}
#endif
		return ArrayView<T>(&m_arr[index * m_dim_2], m_dim_2);
	}

	size_t dim_1() const {
		return m_dim_1;
	}

	size_t dim_2() const {
		return m_dim_2;
	}



	~BlockArray2D()
	{
		delete[] m_arr;
	}

private:
	T* m_arr;
	size_t m_dim_1, m_dim_2;
};
