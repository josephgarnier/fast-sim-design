/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_PRIORITY_QUEUE_H
#define FAST_SIM_DESIGN_PRIORITY_QUEUE_H

#include "algorithm"
#include "QVector"
#include "QSharedPointer"

namespace FastSimDesign {
	template<typename T>
	class PriorityQueue final : private QVector<T>
	{
	private:
		using Parent = QVector<T>;
		using value_type = T;

	public:
		using Parent::isEmpty;
		using Parent::at;
		using Parent::clear;
		using Parent::size;
		using Parent::contains;
		using Parent::begin;
		using Parent::end;

	public:
		explicit PriorityQueue() = default; // Default constructor
		PriorityQueue(PriorityQueue const&) = default; // Copy constructor
		PriorityQueue(PriorityQueue&&) = default; // Move constructor
		PriorityQueue& operator=(PriorityQueue const&) = default; // Copy assignment operator
		PriorityQueue& operator=(PriorityQueue&&) = default; // Move assignment operator
		virtual ~PriorityQueue() = default; // Destructor

		T dequeue() noexcept
		{
			return Parent::takeFirst();
		}

		template<typename Compare>
		void enqueue(T const& tElement, Compare oComp) noexcept
		{
			Parent::append(tElement);
			std::sort(Parent::begin(), Parent::end(), oComp);
		}

		template<typename Compare>
		void enqueue(T&& tElement, Compare oComp) noexcept
		{
			Parent::append(std::forward<T>(tElement));
			std::sort(Parent::begin(), Parent::end(), oComp);
		}

		T& head() noexcept
		{
			return Parent::first();
		}

		T& find(T const& tElement) noexcept
		{
			int index = Parent::indexOf(tElement);
			Q_ASSERT_X(index != -1, "", "Element not found");
			return Parent::operator[](index);
		}

		void remove(T const& tElement) noexcept
		{
			int index = Parent::indexOf(tElement);
			if (index != -1)
				Parent::removeAt(index);
		}

	protected:
	private:
	};

	template<typename T>
	struct less_shared_value
	{ // functor for operator<
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(QSharedPointer<T> const& oLeft, QSharedPointer<T> const& oRight) const noexcept
		{ // apply operator< to operands
			return (*oLeft.data() < *oRight.data());
		}
	};
}
#endif
