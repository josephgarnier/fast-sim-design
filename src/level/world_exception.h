/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_WORLD_EXCEPTION_H
#define FAST_SIM_DESIGN_WORLD_EXCEPTION_H

#include "QLatin1String"

namespace FastSimDesign {
	class WorldException final : public std::exception
	{
	private:
		using Parent = std::exception;

	public:
		explicit WorldException() = default; // Default constructor
		explicit WorldException(QString const& what);
		explicit WorldException(QLatin1String what);
		WorldException(WorldException const&) = default; // Copy constructor
		WorldException(WorldException&&) = default; // Move constructor
		WorldException& operator=(WorldException const&) = default; // Copy assignment operator
		WorldException& operator=(WorldException&&) = default; // Move assignment operator
		virtual ~WorldException() = default; // Destructor

		virtual char const* what() const noexcept override;

	protected:
	private:
		QByteArray m_whatMessage;
	};
}
#endif
