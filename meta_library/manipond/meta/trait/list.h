// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/meta/wrapper/list.h>
#include <type_traits> //用于 std::remove_reference_t


//演化塘::元工具
namespace manipond::meta
{
	//元映射::列表 trait::list================================================================================

	template<typewise::List List>
	using head_t = typename list::type_list_trait<std::remove_reference_t<List>>::head;

	template<typewise::List List>
	using t_tail_list = typename list::type_list_trait<std::remove_reference_t<List>>::tail_list;

	template<valuewise::List List>
	inline constexpr auto head_v = list::value_list_trait<std::remove_reference_t<List>>::head;

	template<valuewise::List List>
	using v_tail_list = typename list::value_list_trait<std::remove_reference_t<List>>::tail_list;

	template<auto... Value>
	using type_extractor = list::type_list<decltype(Value)...>;

}
