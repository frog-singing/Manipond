// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once


//演化塘::元工具
namespace manipond::meta
{
	//包装器::元素 wrapper::element================================================================================

	//类型元素
	template<typename Type>
	struct type_element { using type = Type; };

	//类型-值对
	template<typename Type, auto Value>
	struct type_value_pair : type_element<Type>
	{
		using value_type = decltype(Value);
		static constexpr value_type value{ Value };
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	//值元素
	template<auto Value>
	struct value_element : type_value_pair<decltype(Value), Value> {};

}
