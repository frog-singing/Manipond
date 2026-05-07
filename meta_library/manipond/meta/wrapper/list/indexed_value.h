// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include "index_element.h"
#include <manipond/meta/wrapper/element.h>
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::index_sequence


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::带索引值 wrapper::indexed value================================================================================

	//带索引值
	template <std::size_t Index, auto Value>
	struct indexed_value : index_element<Index>, value_element<Value> {};

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
