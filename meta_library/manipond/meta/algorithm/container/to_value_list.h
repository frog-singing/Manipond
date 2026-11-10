// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/exosuit/error/dependent_false.h>
#include <manipond/meta/constraint/container.h>
#include <manipond/meta/wrapper/list.h>
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::index_sequence, std::make_index_sequence
#include <type_traits> //用于 std::remove_cvref_t (C++20)
#include <ranges> //用于 std::ranges，C++20标准
#include <tuple> //用于 std::get, std::tuple_size_v


//演化塘::元工具::容器
namespace manipond::meta::container
{
	//算法::值列表 algorithm::value list================================================================================

	template<auto Container, template<auto...> typename ValueList>
	struct to_value_list_impl
	{
		static_assert(exosuit::dependent_false_v<decltype(Container)>,
			"[manipond::meta] Error: Unsupported container type!\n"
			"Expected: vector-like container or tuple-like container.");
	};

	//MSCV 无法将 VectorLike auto Container 识别为有效特化
	template<auto Container, template<auto...> typename ValueList>
		requires VectorLike<decltype(Container)>
	struct to_value_list_impl<Container, ValueList>
	{
		template<typename IndexSequence> struct impl;
		template<std::size_t... I> struct impl<std::index_sequence<I...>>
		{
			using type = ValueList<
				Container[I]...
			>;
		};

		using type = typename impl< std::make_index_sequence<
			std::ranges::size(Container)
		> >::type;
	};

	template<TupleLike auto Container, template<auto...> typename ValueList>
		requires (!VectorLike<decltype(Container)>)
	struct to_value_list_impl<Container, ValueList>
	{
		template<typename IndexSequence> struct impl;
		template<std::size_t... I> struct impl<std::index_sequence<I...>>
		{
			using type = ValueList<
				std::get<I>(Container)...
			>;
		};

		using type = typename impl< std::make_index_sequence<
			std::tuple_size_v<std::remove_cvref_t<decltype(Container)>>
		> >::type;
	};

	template<auto Container, template<auto...> typename ValueList = value_list>
	using to_value_list = typename to_value_list_impl<Container, ValueList>::type;

}
