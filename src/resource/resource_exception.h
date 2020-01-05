/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_RESOURCE_EXCEPTION_H
#define FAST_SIM_DESIGN_RESOURCE_EXCEPTION_H

#include "QLatin1String"

namespace FastSimDesign {
	class ResourceException final : public std::exception
	{
	public:
		explicit ResourceException() = default; // Default constructor
		explicit ResourceException(QString const& sWhat) noexcept;
		explicit ResourceException(QLatin1String sWhat) noexcept;
		ResourceException(ResourceException const&) = default; // Copy constructor
		ResourceException(ResourceException&&) = default; // Move constructor
		ResourceException& operator=(ResourceException const&) = default; // Copy assignment operator
		ResourceException& operator=(ResourceException&&) = default; // Move assignment operator
		virtual ~ResourceException() = default; // Destructor

		virtual char const* what() const noexcept override;

	protected:
	private:
		char const* m_pWhatMessage;
	};
}
#endif
