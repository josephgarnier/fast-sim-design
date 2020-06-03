/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world_exception.h"

namespace FastSimDesign {
	WorldException::WorldException(QString const& sWhat)
		: Parent{}
		, m_pWhatMessage{sWhat.toUtf8().constData()}
	{
	}

	WorldException::WorldException(QLatin1String sWhat)
		: Parent{}
		, m_pWhatMessage{sWhat.data()}
	{
	}

	char const* WorldException::what() const noexcept
	{
		return m_pWhatMessage;
	}
}
