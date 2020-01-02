/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_PATH_H
#define FAST_SIM_DESIGN_PATH_H

#include "path_point.h"
#include "QStack"

namespace FastSimDesign {
	class Path final : private QStack<PathPoint>
	{
	private:
		using Parent = QStack<PathPoint>;

	public:
		using Parent::pop;
		using Parent::push;
		using Parent::top;
		using Parent::clear;

	public:
		explicit Path() = default; // Default constructor
		Path(Path const&) = default; // Copy constructor
		Path(Path&&) = default; // Move constructor
		Path& operator=(Path const&) = default; // Copy assignment operator
		Path& operator=(Path&&) = default; // Move assignment operator
		virtual ~Path() = default; // Destructor

		inline bool isFinished() const noexcept { return Parent::isEmpty(); }
		inline int length() const noexcept { return Parent::size(); }

		inline PathPoint const& getFinalPathPoint() const noexcept { return Parent::first(); }

		template<typename... Args>
		void emplace(Args&&... args) noexcept
		{
			push(PathPoint(std::forward<Args>(args)...));
		}

	protected:
	private:
	};
}
#endif