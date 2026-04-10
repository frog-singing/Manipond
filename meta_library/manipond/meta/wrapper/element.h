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

	//值元素
	template<auto Value>
	struct value_element
	{
		using type = decltype(Value);
		using value_type = type;
		static constexpr type value{ Value };
		constexpr operator type() const noexcept { return value; }
		constexpr type operator()() const noexcept { return value; }
	};

}
