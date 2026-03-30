// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <utility> //用于 std::forward


//演化塘::元工具
namespace manipond::meta
{
	//约束::构造 constraint::construct================================================================================

	//可构造至
	template<typename Source, typename Target>
	concept constructible_to = requires(Source && source)
	{
		Target{ std::forward<Source>(source) };
	};

}
