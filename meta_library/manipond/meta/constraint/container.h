// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <ranges> //用于 std::ranges
#include <type_traits> //用于 std::remove_cvref_t (C++20), std::remove_reference_t
#include <cstddef> //用于 std::size_t
#include <concepts> //用于 std::same_as, std::convertible_to，C++20标准
#include <tuple> //用于 std::tuple_size, std::tuple_element
#include <utility> //用于 std::forward


//演化塘::元工具
namespace manipond::meta
{
	//约束::容器 constraint::container================================================================================

	//向量容器约束
	template<typename Vector>
	concept VectorLike =
		std::ranges::contiguous_range<std::remove_cvref_t<Vector>> && //内存必须是物理连续的 (C++20)
		std::ranges::sized_range<std::remove_cvref_t<Vector>> && //必须能在 O(1) 获取大小
		requires(std::remove_reference_t<Vector>& container, std::size_t i) //支持下标访问
	{
		{ container[i] } -> std::same_as<std::ranges::range_reference_t<Vector>>;
	};

	//元组容器约束
	template<typename Tuple>
	concept has_tuple_size = requires {
        { std::tuple_size<std::remove_cvref_t<Tuple>>::value } -> std::convertible_to<std::size_t>;
    };

    //需要满足 has_tuple_size<Tuple>
    template <typename Tuple>
    concept empty_tuple = std::tuple_size<std::remove_cvref_t<Tuple>>::value == 0;

    template <typename Tuple>
    concept has_tuple_element = requires {
        typename std::tuple_element<0, std::remove_cvref_t<Tuple>>::type;
    };

    //为了配合 std::apply，锁定为 std::get，暂不支持成员函数 get 和 ADL get
    template <typename Tuple>
    concept can_get_element = requires(Tuple&& tuple)
    {
        std::get<0>(std::forward<Tuple>(tuple));
    };

    template <typename Tuple>
    concept TupleLike = has_tuple_size<Tuple> &&
        (
            empty_tuple<Tuple> ||
            (has_tuple_element<Tuple> && can_get_element<Tuple>)
        );

}
