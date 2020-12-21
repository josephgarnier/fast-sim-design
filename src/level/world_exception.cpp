/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world_exception.h"

namespace FastSimDesign {
	/*****************************************************************************
	WorldException::Methods
	*****************************************************************************/
	WorldException::WorldException(QString const& what)
		: Parent{}
		, m_whatMessage{what.toUtf8()}
	{
	}

	WorldException::WorldException(QLatin1String what)
		: WorldException{QString{std::move(what)}}
	{
	}

	char const* WorldException::what() const noexcept
	{
		return m_whatMessage.constData();
	}
}
