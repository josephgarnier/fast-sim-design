/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "resource_exception.h"

namespace FastSimDesign {
	/*****************************************************************************
	ResourceException::Methods
	*****************************************************************************/
	ResourceException::ResourceException(QString const& what) noexcept
		: Parent{}
		, m_whatMessage{what.toUtf8()}
	{
	}

	ResourceException::ResourceException(QLatin1String what) noexcept
		: ResourceException{QString{std::move(what)}}
	{
	}

	char const* ResourceException::what() const noexcept
	{
		return m_whatMessage.constData();
	}
}
