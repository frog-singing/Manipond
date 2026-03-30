// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <ranges> //用于 std::ranges
#include <type_traits> //用于 std::remove_cvref_t
#include <cstddef> //用于 std::size_t
#include <concepts> //用于 std::same_as，C++20标准


//演化塘::元工具
namespace manipond::meta
{
	//约束::容器 constraint::container================================================================================

	//向量容器约束
	template<typename ContainerType>
	concept VectorLike =
		std::ranges::contiguous_range<std::remove_cvref_t<ContainerType>> && //内存必须是物理连续的（C++20）
		std::ranges::sized_range<std::remove_cvref_t<ContainerType>> && //必须能在 O(1) 获取大小
		requires(ContainerType & container, std::size_t i) //支持下标访问
	{
		{ container[i] } -> std::same_as<std::ranges::range_reference_t<ContainerType>>;
	};

}
