// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once


#if defined(_MSVC_LANG)
	#define MANIPOND_CXX_STANDARD _MSVC_LANG
#else
	#define MANIPOND_CXX_STANDARD __cplusplus
#endif

#define MANIPOND_CXX_20 202002L
#define MANIPOND_CXX_23 202302L
#define MANIPOND_CXX_26 202600L //占位符，C++26 正式发布后更新
