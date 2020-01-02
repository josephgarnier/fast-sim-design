/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_DLLCONFIG_H
#define FAST_SIM_DESIGN_DLLCONFIG_H

// Visual Studio Compiler
#if defined(_MSC_VER)
	#if defined(FAST_SIM_DESIGN_EXPORTS)
		#define FAST_SIM_DESIGN_API __declspec(dllexport)
		#define FAST_SIM_DESIGN_API_TEMPLATE(T) template class __declspec(dllexport) T
	#else
		#define FAST_SIM_DESIGN_API __declspec(dllimport)
		#define FAST_SIM_DESIGN_API_TEMPLATE(T) extern template class __declspec(dllimport) T
	#endif
// GNU Compiler
#elif defined(__clang__)
	#define FAST_SIM_DESIGN_API __attribute__((visibility("default")))
	#define FAST_SIM_DESIGN_API_TEMPLATE(T)
// LLVM Compiler
#elif defined(__GNUC__)
	#define FAST_SIM_DESIGN_API __declspec(dllexport)
	#define FAST_SIM_DESIGN_API_TEMPLATE(T) template class __declspec(dllexport) T
#else
	#error Unknown Compiler, please implement shared library macros
#endif // COMPILER

#endif