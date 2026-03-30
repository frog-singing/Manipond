// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::index_sequence


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::带索引值 wrapper::indexed_value================================================================================

	//带索引值
	template <std::size_t Index, auto Value>
	struct indexed_value
	{
		static constexpr std::size_t index{ Index };

		using type = decltype(Value);
		using value_type = type;
		static constexpr type value{ Value };
		constexpr operator type() const noexcept { return value; }
		constexpr type operator()() const noexcept { return value; }
	};

	//聚合带索引值
	template <typename IndexSequence, auto... Value>
	struct poly_indexed_value;

	template <std::size_t... Index, auto... Value>
		requires (sizeof...(Index) == sizeof...(Value))
	struct poly_indexed_value<std::index_sequence<Index...>, Value...>
		: indexed_value<Index, Value>...
	{
		static constexpr std::size_t size{ sizeof...(Index) };
	};

	//解析值
	template <std::size_t Index, auto Value>
	consteval auto resolve_value(indexed_value<Index, Value>) noexcept -> indexed_value<Index, Value>
	{ return {}; }

}
