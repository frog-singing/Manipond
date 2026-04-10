// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <concepts> //用于 std::same_as，C++20标准
#include <type_traits> //用于 std::is_empty_v


//约束::等价 constraint::equivalence================================================================================

//演化塘::元工具::同一
namespace manipond::meta::identical
{
	template<auto, auto>
	constexpr bool is_same_value_v = false;

	template<auto V>
	constexpr bool is_same_value_v<V, V> = true;

}

//演化塘::元工具::类型维度
namespace manipond::meta::typewise
{
	// ≡
	template<typename T1, typename T2>
	concept same_as = std::same_as<T1, T2>;

}

//演化塘::元工具::值维度
namespace manipond::meta::valuewise
{
	// ≡
	template<auto V1, auto V2>
	concept same_as = identical::is_same_value_v<V1, V2> && identical::is_same_value_v<V2, V1>;

	// =
	template<auto V1, auto V2>
	concept equal_to =
		(requires { requires V1 == V2; }) ||
		typewise::same_as<decltype(V1), decltype(V2)> &&
		(std::is_empty_v<decltype(V1)> || valuewise::same_as<V1, V2>);

	/*
	* equal_to 中的等价关系必须满足：
	* Equivalence relation in equal_to must satisfy:
	*
	* 自反性 Reflexivity		: bool(V == V) == true
	* 对称性 Symmetry		: bool(V1 == V2) == bool(V2 == V1)
	* 传递性 Transitivity	: (V1 == V2) && (V2 == V3)  ==>  (V1 == V3)
	*
	* 否则视为未定义行为    Undefined behavior otherwise
	*/

}
