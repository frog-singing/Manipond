// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include "list/type_list.h"
#include "list/value_list.h"


//包装器::列表 wrapper::list================================================================================

//演化塘::元工具
namespace manipond::meta
{
	using list::type_list;
	using list::as_type_list;

	using list::value_list;
	using list::as_value_list;

}

//演化塘::元工具::类型维度
namespace manipond::meta::typewise
{
	template<typename TargetList>
	concept List = list::TypeListLike<TargetList>;

}

//演化塘::元工具::值维度
namespace manipond::meta::valuewise
{
	template<typename TargetList>
	concept List = list::ValueListLike<TargetList>;

}
