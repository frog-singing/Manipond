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
	//包装器::带索引类型 wrapper::indexed type================================================================================

	//带索引类型
	template <std::size_t Index, typename Type>
	struct indexed_type : index_element<Index>, type_element<Type> {};

	//聚合带索引类型
	template <typename IndexSequence, typename... Type>
	struct poly_indexed_type;

	template <std::size_t... Index, typename... Type>
		requires (sizeof...(Index) == sizeof...(Type))
	struct poly_indexed_type<std::index_sequence<Index...>, Type...>
		: indexed_type<Index, Type>...
	{
		static constexpr std::size_t size{ sizeof...(Index) };
	};

	//解析类型
	template <std::size_t Index, typename Type>
	consteval auto resolve_type(indexed_type<Index, Type>) noexcept -> indexed_type<Index, Type>
	{ return {}; }

}
