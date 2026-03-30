// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include "container.h"
#include <type_traits> //用于 std::is_trivially_copyable_v, std::remove_cvref_t


//演化塘::元工具
namespace manipond::meta
{
	//约束::视图 constraint::view================================================================================

	//向量视图约束
	template<typename ViewType>
	concept SpanLike =
		VectorLike<ViewType> &&
		std::is_trivially_copyable_v<std::remove_cvref_t<ViewType>> && //必须支持平凡拷贝，确保值传递开销极低
		(sizeof(std::remove_cvref_t<ViewType>) <= sizeof(void*) * 4);  //经验法则：视图不应比 4 个指针还大

}
