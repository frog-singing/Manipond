// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once


//演化塘::元工具
namespace manipond::meta
{
	//辅助工具 util================================================================================

	//模板依赖静态断言组件，用于报错
	template<typename...>
	inline constexpr bool dependent_false_v = false;

}
