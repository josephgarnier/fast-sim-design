/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_MATH_UTIL_H
#define FAST_SIM_DESIGN_MATH_UTIL_H

#include "qmath.h"

namespace FastSimDesign {

	/*!
	 * Encore two natural numbers into a single natural number. It's deterministic.
	 * @see https://en.wikipedia.org/wiki/Pairing_function
	 * @see https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
	 */
	inline int cantorPairingFunction(int k1, int k2) noexcept
	{
		int p = 1.0f / 2.0f * (k1 + k2) * (k1 + k2 + 1) + k2;
		// Cantor's function only works on non-negative numbers. We define a bijection
		if (p >= 0)
			return p * 2;
		else
			return (-p * 2) - 1;
	}

	/*!
	* Decode a natural numbers into two natural numbers.
	* @see https://en.wikipedia.org/wiki/Pairing_function
	* @see https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
	*/
	inline std::pair<int, int> cantorDepairingFunction(int z) noexcept
	{
		double w = std::floor((std::sqrt(8 * z + 1) - 1) / 2);
		double t = (w * w + w) / 2;
		int y = static_cast<int>(z - t);
		int x = static_cast<int>(w - y);
		return std::make_pair(x, y);
	}

	template<typename T>
	inline int sign(T val) noexcept
	{
		return (T(0) < val) - (val < T(0));
	}

	template<typename T>
	inline int signNotNull(T val) noexcept
	{
		return (T(0) <= val) - (val < T(0));
	}

	/*!
	* Rescale x with Min-Max scaling. Result value is in [0;1].
	* It's compute with (X-Xmin)/(Xmax-Xmin)
	* @see https://en.wikipedia.org/wiki/Feature_scaling
	*/
	template<typename T>
	inline T rescaledData(T xmin, T xmax, T x)
	{
		Q_ASSERT_X(xmax >= xmin, "", "Min and Max are reversed");
		Q_ASSERT_X(x >= xmin && x <= xmax, "", "X is out of bound");
		Q_ASSERT_X(xmin != 0 || xmax != 0, "", "At least one parameter must be different of zero");
		T rescaledData = (x - xmin) / (xmax - xmin);
		Q_ASSERT_X(rescaledData <= 1.0f && rescaledData >= 0.0f, "", "Normalized data should be between 0 and 1");
		return rescaledData;
	}

	/*!
	 * Normalizes x with Euclidean norm. Result value is in [0;1].
	 * It's compute with Xi/Norm
	 * @see https://en.wikipedia.org/wiki/Feature_scaling
	 */
	template<typename T>
	inline T normalizedData(T x, T norm)
	{
		Q_ASSERT_X(norm != 0, "", "norm should be different from 0");
		return x / norm;
	}

#ifndef M_2PI
#define M_2PI M_PI * 2.0 // pi*2
#endif
}
#endif
