// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::列表::标签 wrapper::list::tag================================================================================
	
	//列表标签
	struct list_tag {};
	struct type_list_tag : list_tag {};
	struct value_list_tag : list_tag {};

}
