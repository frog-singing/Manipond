// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/exosuit/error/dependent_false.h>


//演化塘::外装件
namespace manipond::exosuit
{
	//前面的区域 area ahead================================================================================

	template<typename... Type>
	struct area_ahead
	{
		static_assert(dependent_false_v<Type...>,
			"[WIP] How about we explore the area ahead of us later? "
			"前面的区域以后再来探索吧？");
	};

	template<typename... Type>
	consteval void explore_later()
	{
		static_assert(dependent_false_v<Type...>,
			"[WIP] How about we explore the area ahead of us later? "
			"前面的区域以后再来探索吧？");
	}

}
