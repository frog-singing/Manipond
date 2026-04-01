// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once


//演化塘::外装件
namespace manipond::exosuit
{
	//有依赖错误 dependent false================================================================================

	//模板依赖静态断言组件，用于报错
	template<typename...>
	inline constexpr bool dependent_false_v = false;

}
