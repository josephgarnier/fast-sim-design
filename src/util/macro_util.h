/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_MACRO_UTIL_H
#define FAST_SIM_DESIGN_MACRO_UTIL_H

namespace FastSimDesign {

#define QT_DEFAULT_CONSTRUCTOR(x_ClassName) \
	{                                         \
		friend class QVector<x_ClassName>;      \
                                            \
	private:                                  \
		explicit x_ClassName() = default;       \
                                            \
	public:                                   \
	}

#define QVECTOR_NON_DEFAULT_CONSTRUCTIBLE(Type)               \
	{                                                           \
		template<>                                                \
		inline QVector<Type>::QVector(int) = delete;              \
		template<>                                                \
		inline void QVector<Type>::resize(int newSize)            \
		{                                                         \
			Q_ASSERT(newSize <= size());                            \
			detach();                                               \
		}                                                         \
		template<>                                                \
		inline void QVector<Type>::defaultConstruct(Type*, Type*) \
		{                                                         \
			Q_ASSERT(false);                                        \
		}                                                         \
	}
}
#endif
