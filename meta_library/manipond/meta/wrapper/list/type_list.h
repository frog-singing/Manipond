// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include "tag.h"
#include "indexed_type.h"
#include <manipond/exosuit/error/dependent_false.h>
#include <cstddef> //用于 std::size_t
#include <utility> //用于 std::make_index_sequence, std::forward
#include <concepts> //用于 std::derived_from，C++20标准
#include <type_traits> //用于 std::remove_cvref_t, std::false_type, std::true_type


namespace manipond::meta::list
{
	template<typename List>
	struct type_list_trait;
}


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::类型列表 wrapper::type list================================================================================

	//类型列表
	//如果要使用递归展开，则不要存储void类型，因为空类型列表的head是void类型
	template<typename... Type>
	struct type_list : type_list_tag
	{
		static constexpr std::size_t size{ sizeof...(Type) };
		static constexpr bool empty{ !size };

		//头参数
		using head = typename type_list_trait<type_list>::head;

		//尾参数包列表
		using tail_list = typename type_list_trait<type_list>::tail_list;

	private:
		//带索引类型表
		using type_table = poly_indexed_type<std::make_index_sequence<size>, Type...>;

	public:
		//下标访问元素
		template<std::size_t Index>
			requires (Index < sizeof...(Type)) //不能用静态成员常量 size，因为此时 type_list 还没有实例化
		using element = typename decltype(resolve_type<Index>(type_table{}))::type;

		//重包装
		template<template<typename...> typename Wrapper>
		using apply = Wrapper<Type...>;

		//变换
		template<template<typename> typename Mapping>
		using transform = type_list<Mapping<Type>...>;

	private:
		// MSVC 无法识别 operator() 调用后接 ...
		template<auto Mapping, typename CurrentType>
		static constexpr auto lambda_mapping_result = Mapping.template operator() < CurrentType > ();

	public:
		//变换为值
		template<auto Mapping>
		using to_value = value_list<lambda_mapping_result<Mapping, Type>...>;

		//调用可调用对象
		template<typename Invocable>
			requires requires(Invocable&& invocable)
			{ std::forward<Invocable>(invocable).template operator() < Type... > (); }
		static constexpr decltype(auto) invoke(Invocable&& invocable)
			noexcept(noexcept(std::forward<Invocable>(invocable).template operator() < Type... > ()))
		{
			return std::forward<Invocable>(invocable).template operator() < Type... > ();
		}

	};

	//类型列表特征
	template<template<typename...> typename Wrapper>
	struct type_list_trait<Wrapper<>>
	{
		using head = void;
		using tail_list = type_list<>;
	};

	template<template<typename...> typename Wrapper>
	struct type_list_trait<const Wrapper<>> : type_list_trait<Wrapper<>>
	{
		using tail_list = const type_list<>;
	};

	template<template<typename...> typename Wrapper>
	struct type_list_trait<volatile Wrapper<>> : type_list_trait<Wrapper<>>
	{
		using tail_list = volatile type_list<>;
	};

	template<template<typename...> typename Wrapper>
	struct type_list_trait<const volatile Wrapper<>> : type_list_trait<Wrapper<>>
	{
		using tail_list = const volatile type_list<>;
	};	

	template<template<typename...> typename Wrapper, typename Head, typename... Tail>
	struct type_list_trait<Wrapper<Head, Tail...>>
	{
		using head = Head;
		using tail_list = type_list<Tail...>;
	};

	template<template<typename...> typename Wrapper, typename Head, typename... Tail>
	struct type_list_trait<const Wrapper<Head, Tail...>>
	{
		using head = const Head;
		using tail_list = const type_list<Tail...>;
	};

	template<template<typename...> typename Wrapper, typename Head, typename... Tail>
	struct type_list_trait<volatile Wrapper<Head, Tail...>>
	{
		using head = volatile Head;
		using tail_list = volatile type_list<Tail...>;
	};

	template<template<typename...> typename Wrapper, typename Head, typename... Tail>
	struct type_list_trait<const volatile Wrapper<Head, Tail...>>
	{
		using head = const volatile Head;
		using tail_list = const volatile type_list<Tail...>;
	};

	//类型列表适配器，将任意类型包装器适配为类型列表
	template<typename List>
	struct as_type_list_impl
	{
		static_assert(exosuit::dependent_false_v<List>,
			"[manipond::meta] Unsupported list type. Please use type_list to construct your list.");
	};

	template<typename List>
		requires std::derived_from<List, type_list_tag>
	struct as_type_list_impl<List>
	{
		using type = List;
	};

	template<template<typename...> typename Wrapper, typename... Type>
		requires (!std::derived_from<Wrapper<Type...>, type_list_tag>)
	struct as_type_list_impl<Wrapper<Type...>>
	{
		using type = type_list<Type...>;
	};

	template<typename List>
	using as_type_list = typename as_type_list_impl<std::remove_cvref_t<List>>::type;

	//类型列表约束
	template<typename List>
	struct is_type_list_like : std::false_type {};

	template<template<typename...> typename Wrapper, typename... Type>
	struct is_type_list_like<Wrapper<Type...>> : std::true_type {};

	template<typename List>
	concept TypeListLike =
		std::derived_from<std::remove_cvref_t<List>, type_list_tag> ||
		is_type_list_like<std::remove_cvref_t<List>>::value;

}
