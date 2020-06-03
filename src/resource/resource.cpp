/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "resource.h"

namespace FastSimDesign {
	Resource::Resource(QFileInfo oPath, QObject* pParent /*= 0*/) noexcept
		: QObject{pParent}
		, m_oAbsoluteFilePath{std::move(oPath)}
		, m_oStatus{Status::Null}
	{
		if (!m_oAbsoluteFilePath.isAbsolute())
			m_oAbsoluteFilePath.makeAbsolute();
	}

	void Resource::checkReady() noexcept
	{
		if (getStatus() == Resource::Status::Loading)
			setStatus(Resource::Status::Ready);
	}
}
