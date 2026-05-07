// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <cstddef> //用于 std::size_t


//演化塘::元工具::列表
namespace manipond::meta::list
{
	//包装器::索引元素 wrapper::index element================================================================================

	//索引元素
	template <std::size_t Index>
	struct index_element
	{
		static constexpr std::size_t index{ Index };
	};

}
