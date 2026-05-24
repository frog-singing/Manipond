// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <version> //用于 __cpp_pack_indexing (C++26)，C++20标准
#include <manipond/exosuit/adaptation/cxx_standard.h>

#include "tag.h"
#include "indexed_value.h"
#include <manipond/exosuit/error/dependent_false.h>
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::make_index_sequence, std::forward
#include <variant> //用于 std::monostate
#include <concepts> //用于 std::derived_from，C++20标准
#include <type_traits> //用于 std::remove_cvref_t (C++20), std::false_type, std::true_type, std::integral_constant
#include <tuple> //用于 std::tuple_size, std::tuple_element


namespace manipond::meta::list
{
	template<typename List>
	struct value_list_trait;
}


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::值列表 wrapper::value list================================================================================

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

#if defined(__cpp_pack_indexing) && MANIPOND_CXX_STANDARD >= MANIPOND_CXX_26
		//C++26
		//下标访问元素类型
		template<std::size_t Index>
			requires (Index < sizeof...(Value))
		using element_type = decltype(Value...[Index]);

		//下标访问元素，成员常量
		template<std::size_t Index>
			requires (Index < sizeof...(Value))
		static constexpr element_type<Index> element = Value...[Index];
#else
		//C++20
	private:
		//带索引值表
		using value_table = poly_indexed_value<std::make_index_sequence<size>, Value...>;

	public:
		//下标访问元素类型
		template<std::size_t Index>
			requires (Index < sizeof...(Value)) //不能用静态成员常量 size，因为此时 value_list 还没有实例化
		using element_type = typename decltype(resolve_value<Index>(value_table{}))::value_type;

		//下标访问元素，成员常量
		template<std::size_t Index>
			requires (Index < sizeof...(Value)) //不能用静态成员常量 size，因为此时 value_list 还没有实例化
		static constexpr element_type<Index> element = decltype(resolve_value<Index>(value_table{}))::value;
#endif		

		//下标访问元素，成员函数
		template<std::size_t Index>
		static constexpr auto get() noexcept { return element<Index>; }

		//重包装
		template<template<auto...> typename Wrapper>
		using apply = Wrapper<Value...>;

	private:
		// MSVC 无法识别 operator() 调用后接 ...
		template<auto Mapping, auto CurrentValue>
		static constexpr auto lambda_mapping_result = Mapping.template operator() < CurrentValue > ();

	public:
		//变换
		template<auto Mapping>
		using transform = value_list<lambda_mapping_result<Mapping, Value>...>;

		//变换为类型
		template<template<auto> typename Mapping>
		using to_type = type_list<Mapping<Value>...>;

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

	template<template<auto...> typename Wrapper>
	struct value_list_trait<const Wrapper<>> : value_list_trait<Wrapper<>>
	{
		using tail_list = const value_list<>;
	};

	template<template<auto...> typename Wrapper>
	struct value_list_trait<volatile Wrapper<>> : value_list_trait<Wrapper<>>
	{
		using tail_list = volatile value_list<>;
	};

	template<template<auto...> typename Wrapper>
	struct value_list_trait<const volatile Wrapper<>> : value_list_trait<Wrapper<>>
	{
		using tail_list = const volatile value_list<>;
	};

	template<template<auto...> typename Wrapper, auto Head, auto... Tail>
	struct value_list_trait<Wrapper<Head, Tail...>>
	{
		using head_type = decltype(Head);
		static constexpr head_type head{ Head };
		using tail_list = value_list<Tail...>;
	};

	template<template<auto...> typename Wrapper, auto Head, auto... Tail>
	struct value_list_trait<const Wrapper<Head, Tail...>>
	{
		using head_type = const decltype(Head);
		static constexpr head_type head{ Head };
		using tail_list = const value_list<Tail...>;
	};

	template<template<auto...> typename Wrapper, auto Head, auto... Tail>
	struct value_list_trait<volatile Wrapper<Head, Tail...>>
	{
		using head_type = volatile decltype(Head);
		static constexpr head_type head{ Head };
		using tail_list = volatile value_list<Tail...>;
	};

	template<template<auto...> typename Wrapper, auto Head, auto... Tail>
	struct value_list_trait<const volatile Wrapper<Head, Tail...>>
	{
		using head_type = const volatile decltype(Head);
		static constexpr head_type head{ Head };
		using tail_list = const volatile value_list<Tail...>;
	};

	//值列表适配器，将任意值包装器适配为值列表
	template<typename List>
	struct as_value_list_impl
	{
		static_assert(exosuit::dependent_false_v<List>,
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
	using as_value_list = typename as_value_list_impl<std::remove_cvref_t<List>>::type;

	//值列表约束
	template<typename List>
	struct is_value_list_like : std::false_type {};

	template<template<auto...> typename Wrapper, auto... Value>
	struct is_value_list_like<Wrapper<Value...>> : std::true_type {};

	template<typename List>
	concept ValueListLike =
		std::derived_from<std::remove_cvref_t<List>, value_list_tag> ||
		is_value_list_like<std::remove_cvref_t<List>>::value;

	//值列表的 get 函数重载，用于 tuple-like 访问
	template<std::size_t Index, auto... Value>
		requires (Index < sizeof...(Value))
	constexpr auto get(const value_list<Value...>&) noexcept
	{
		return value_list<Value...>::template element<Index>;
	}

	template<std::size_t Index, auto... Value>
		requires (Index < sizeof...(Value))
	constexpr auto get(const volatile value_list<Value...>&) noexcept
	{
		return value_list<Value...>::template element<Index>;
	}

}


namespace std
{
	//值列表的 tuple_size 和 tuple_element 特化，用于 tuple-like 访问
	template<auto... Value>
	struct tuple_size<manipond::meta::list::value_list<Value...>>
		: integral_constant<size_t, sizeof...(Value)> {};

	template<size_t Index, auto... Value>
		requires (Index < sizeof...(Value))
	struct tuple_element<Index, manipond::meta::list::value_list<Value...>>
	{
		using type = decltype(manipond::meta::list::value_list<Value...>::template element<Index>);
	};

	//值列表的 get 函数重载
	template<size_t Index, auto... Value>
		requires (Index < sizeof...(Value))
	constexpr auto get(const manipond::meta::list::value_list<Value...>&) noexcept
	{
		return manipond::meta::list::value_list<Value...>::template element<Index>;
	}

	template<size_t Index, auto... Value>
		requires (Index < sizeof...(Value))
	constexpr auto get(const volatile manipond::meta::list::value_list<Value...>&) noexcept
	{
		return manipond::meta::list::value_list<Value...>::template element<Index>;
	}

}
