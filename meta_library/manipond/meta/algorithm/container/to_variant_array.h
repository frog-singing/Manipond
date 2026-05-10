// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/meta/constraint/container.h>
#include <manipond/meta/wrapper/list.h>
#include <manipond/meta/trait/list.h>
#include <variant> //用于 std::variant
#include <tuple> //用于 std::apply
#include <array> //用于 std::array
#include <utility> //用于 std::forward


//演化塘::元工具::容器
namespace manipond::meta::container
{
	//算法::变体数组 algorithm::variant array================================================================================

	template<typewise::List TupleType>
		requires TupleLike<TupleType>
	static constexpr auto to_variant_array(TupleType&& tuple)
	{
		using variant_type = typename as_type_list<TupleType>::template apply<std::variant>;
		
		return std::apply([](auto&&... instance) {
			return std::array<variant_type, sizeof...(instance)>{ std::forward<decltype(instance)>(instance)... };
		}, std::forward<TupleType>(tuple));
	}

	template<typename TupleType>
	static constexpr auto value_list_to_variant_array_impl()
	{
		using value_list = as_value_list<TupleType>;
		using variant_type = typename value_list::template apply<type_extractor>::template apply<std::variant>;

		return value_list::invoke([] <auto... Value> {
			return std::array<variant_type, sizeof...(Value)>{ Value... };
		});
	}

	template<valuewise::List TupleType>
		requires TupleLike<TupleType>
	static constexpr auto to_variant_array()
	{
		return value_list_to_variant_array_impl<TupleType>();
	}

	template<valuewise::List TupleType>
		requires TupleLike<TupleType>
	static constexpr auto to_variant_array(TupleType)
	{
		return value_list_to_variant_array_impl<TupleType>();
	}

}
