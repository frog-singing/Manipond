// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/meta/wrapper/list.h>
#include <manipond/meta/wrapper/list/type_list.h>
#include <manipond/meta/wrapper/list/value_list.h>


//演化塘::元工具
namespace manipond::meta
{
	//类型映射::列表 trait::list================================================================================

	template<typewise::List List>
	using head_t = typename list::type_list_trait<List>::head;

	template<typewise::List List>
	using t_tail_list = typename list::type_list_trait<List>::tail_list;

	template<valuewise::List List>
	inline constexpr auto head_v = list::value_list_trait<List>::head;

	template<valuewise::List List>
	using v_tail_list = typename list::value_list_trait<List>::tail_list;

}
