// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include "tag.h"
#include "indexed_value.h"
#include <manipond/meta/util.h> //用于 meta::dependent_false_v
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::make_index_sequence, std::forward
#include <variant> //用于 std::monostate
#include <concepts> //用于 std::derived_from，C++20标准
#include <type_traits> //用于 std::false_type, std::true_type


namespace manipond::meta::list
{
	template<auto... Value>
	struct value_list;

	template<typename List>
	struct value_list_trait;
}


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::值列表 wrapper::value_list================================================================================

	//值列表
	//如果要使用递归展开，则不要存储std::monostate{}值，因为空值列表的head是std::monostate{}值
	template<auto... Value>
	struct value_list : value_list_tag
	{
		static constexpr std::size_t size{ sizeof...(Value) };
		static constexpr bool empty{ !size };

		//头参数
		using head_type = typename value_list_trait<value_list>::head_type;
		static constexpr head_type head = value_list_trait<value_list>::head;

		//尾参数包列表
		using tail_list = typename value_list_trait<value_list>::tail_list;

	private:
		//带索引值表
		using value_table = poly_indexed_value<std::make_index_sequence<size>, Value...>;

	public:
		//下标访问元素
		template<std::size_t Index>
			requires (Index < size)
		static constexpr auto element = decltype(resolve_value<Index>(value_table{}))::value;

		//值列表重包装
		template<template<auto...> typename Wrapper>
		using apply = Wrapper<Value...>;

		//调用可调用对象
		template<typename Invocable>
			requires requires(Invocable&& invocable)
			{ std::forward<Invocable>(invocable).template operator() < Value... > (); }
		static constexpr decltype(auto) invoke(Invocable&& invocable)
			noexcept(noexcept(std::forward<Invocable>(invocable).template operator() < Value... > ()))
		{
			return std::forward<Invocable>(invocable).template operator() < Value... > ();
		}

	};

	//值列表特征
	template<template<auto...> typename Wrapper>
	struct value_list_trait<Wrapper<>>
	{
		using head_type = std::monostate;
		static constexpr head_type head{};
		using tail_list = value_list<>;
	};

	template<template<auto...> typename Wrapper, auto Head, auto... Tail>
	struct value_list_trait<Wrapper<Head, Tail...>>
	{
		using head_type = decltype(Head);
		static constexpr head_type head{ Head };
		using tail_list = value_list<Tail...>;
	};

	//值列表适配器，将任意值包装器适配为值列表
	template<typename List>
	struct as_value_list_impl
	{
		static_assert(meta::dependent_false_v<List>,
			"[manipond::meta] Unsupported list type. Please use value_list to construct your list.");
	};

	template<typename List>
		requires std::derived_from<List, value_list_tag>
	struct as_value_list_impl<List>
	{
		using type = List;
	};

	template<template<auto...> typename Wrapper, auto... Value>
		requires (!std::derived_from<Wrapper<Value...>, value_list_tag>)
	struct as_value_list_impl<Wrapper<Value...>>
	{
		using type = value_list<Value...>;
	};

	template<typename List>
	using as_value_list = typename as_value_list_impl<List>::type;

	//值列表约束
	template<typename List>
	struct is_value_list_like : std::false_type {};

	template<template<auto...> typename Wrapper, auto... Value>
	struct is_value_list_like<Wrapper<Value...>> : std::true_type {};

	template<typename List>
	concept ValueListLike =
		std::derived_from<List, value_list_tag> ||
		is_value_list_like<List>::value;

}
