/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "world_exception.h"

namespace FastSimDesign {
	WorldException::WorldException(QString const& sWhat)
		: std::exception{}
		, m_pWhatMessage{sWhat.toUtf8().constData()}
	{
	}

	WorldException::WorldException(QLatin1String sWhat)
		: std::exception{}
		, m_pWhatMessage{sWhat.data()}
	{
	}

	char const* WorldException::what() const noexcept
	{
		return m_pWhatMessage;
	}
}