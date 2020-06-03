/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "resource_exception.h"

namespace FastSimDesign {
	ResourceException::ResourceException(QString const& sWhat) noexcept
		: Parent{}
		, m_pWhatMessage{sWhat.toUtf8().constData()}
	{
	}

	ResourceException::ResourceException(QLatin1String sWhat) noexcept
		: Parent{}
		, m_pWhatMessage{sWhat.data()}
	{
	}

	char const* ResourceException::what() const noexcept
	{
		return m_pWhatMessage;
	}
}
