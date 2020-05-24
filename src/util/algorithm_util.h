/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_ALGORITHM_UTIL_H
#define FAST_SIM_DESIGN_ALGORITHM_UTIL_H

#include "QSharedPointer"
#include "QTime"
#include "cmath"

namespace FastSimDesign {
	/*!
	 * Concatenates two numbers. For a comparison of different methods.
	 * @see https://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
	 */
	// unsigned int concatenate(unsigned int x, unsigned int y)
	// {
	// 	unsigned int pow = 10;
	// 	while (y >= pow)
	// 		pow *= 10;
	// 	return x * pow + y;
	// }

	template<typename InputIt, typename T>
	bool contains_if_shared_value(InputIt first, InputIt last, QSharedPointer<T> const& tElement) noexcept
	{
		return std::find_if(first, last, [&tElement](QSharedPointer<T> const& i) {
			return *i.data() == *tElement.data();
		}) != last;
	}

	template<typename InputIt, typename T>
	QSharedPointer<T>& find_if_shared_value(InputIt first, InputIt last, QSharedPointer<T> const& tElement) noexcept
	{
		auto found = std::find_if(first, last, [&tElement](QSharedPointer<T const> const& i) {
			return *i.data() == *tElement.data();
		});
		if (found == last)
			Q_ASSERT_X(false, "", "Element not found");
		return *found;
	}

	template<typename InputIt, typename T>
	InputIt find_if_shared_value_iterator(InputIt first, InputIt last, QSharedPointer<T> const& tElement) noexcept
	{
		auto found = std::find_if(first, last, [&tElement](QSharedPointer<T> const& i) {
			return *i.data() == *tElement.data();
		});
		if (found == last)
			Q_ASSERT_X(false, "", "Element not found");
		return found;
	}

	template<typename T>
	constexpr auto toUnderlyingType(T t) noexcept
	{
		return static_cast<typename std::underlying_type<T>::type>(t);
	}

	struct Time
	{
		enum
		{
			SECS_PER_DAY = 86400,
			SECS_PER_HOUR = 3600,
			SECS_PER_MIN = 60,
			MSECS_PER_DAY = 86400000,
			MSECS_PER_HOUR = 3600000,
			MSECS_PER_MIN = 60000
		};

		static QTime fromSeconds(float fFromSec) noexcept
		{
			QTime time{0, 0, 0, 0};
			QTime toReturn = time.addMSecs(static_cast<int>(fFromSec * 1000));
			Q_ASSERT_X(toReturn.isValid(), "", "Invalid QTime convertion");
			return toReturn;
		}

		static QTime fromMilliseconds(int iFromMs) noexcept
		{
			QTime time{0, 0, 0, 0};
			QTime toReturn = time.addMSecs(iFromMs);
			Q_ASSERT_X(toReturn.isValid(), "", "Invalid QTime convertion");
			return toReturn;
		}

		static double toSeconds(QTime const& oTime) noexcept
		{
			Q_ASSERT_X(oTime.isValid(), "", "Invalid QTime");
			double seconds = oTime.hour() * Time::SECS_PER_HOUR + oTime.minute() * Time::SECS_PER_MIN + oTime.second() + oTime.msec() / 1000.0;
			return std::round(seconds * 1000.0) / 1000.0;
		}

		static int toMilliseconds(QTime const& oTime) noexcept
		{
			Q_ASSERT_X(oTime.isValid(), "", "Invalid QTime");
			return oTime.hour() * Time::MSECS_PER_HOUR + oTime.minute() * Time::MSECS_PER_MIN + oTime.second() * 1000 + oTime.msec();
		}
	};
}
#endif
