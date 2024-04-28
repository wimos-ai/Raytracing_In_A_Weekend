#pragma once

#include <cstddef>	//std::size_t
#include <span>		//std::span
#include <vector>	//std::vector


template <typename T>
class BlockArray2D
{
public:
	BlockArray2D(size_t dim_1_in, size_t dim_2_in) : m_arr(dim_1_in * dim_2_in), m_dim_1(dim_1_in), m_dim_2(dim_2_in) {};

	BlockArray2D(BlockArray2D& other) : m_arr(other.m_arr), m_dim_1(other.m_dim_1), m_dim_2(other.m_dim_2)
	{};

	// Copy assignment
	BlockArray2D& operator=(const BlockArray2D& other)
	{
		// Self-assignment detection
		if (&other == this)
			return *this;

		
		// Copy the resource
		m_arr = other.m_arr;
		m_dim_1 = other.m_dim_1;
		m_dim_2 = other.m_dim_2;

		return *this;
	}

	// Move Constructor
	// Transfer ownership of a.m_ptr to m_ptr
	BlockArray2D(BlockArray2D&& other) noexcept : m_arr(std::move(other.m_arr)), m_dim_1(other.m_dim_1), m_dim_2(other.m_dim_2) {
		// Remove ownership from other
		other.m_arr.clear();
		other.m_dim_1 = 0;
		other.m_dim_2 = 0;
	};

	// Move assignment
	// Transfer ownership of a.m_ptr to m_ptr
	BlockArray2D& operator=(BlockArray2D&& other) noexcept {
		// Self-assignment detection
		if (&other == this)
			return *this;

		// Transfer ownership of other.m_arr to self
		m_arr = std::move(other.m_arr);
		m_dim_1 = other.m_dim_1;
		m_dim_2 = other.m_dim_2;

		// Remove ownership from other
		other.m_arr.clear();
		other.m_dim_1 = 0;
		other.m_dim_2 = 0;

		return *this;
	}

	std::span<T> operator[](size_t index)
	{
		T* addr = &m_arr[index * m_dim_2];
		return std::span<T>(addr, m_dim_2);
	}

	std::span<const T> operator[](size_t index) const
	{
		const T* addr = &m_arr[index * m_dim_2];
		return std::span<const T>(addr, m_dim_2);
	}

	size_t size() const {
		return m_dim_1;
	}

	~BlockArray2D() = default;
private:
	std::vector<T> m_arr;
	size_t m_dim_1, m_dim_2;
};
