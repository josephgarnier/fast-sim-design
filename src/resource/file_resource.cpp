/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "file_resource.h"
#include "resource_exception.h"

namespace FastSimDesign {
	FileResource::FileResource(QFileInfo oFilePath, QObject* pParent /*= 0*/)
		: Resource{std::move(oFilePath), pParent}
		, m_oFile{Q_NULLPTR}
	{
		try
		{
			setStatus(Resource::Status::Loading);
			loadFile();
			qInfo() << "File loaded.";
			checkReady();
		} catch (...)
		{
			throw;
		}
	}

	void FileResource::loadFile()
	{
		if (!m_oAbsoluteFilePath.exists() || !m_oAbsoluteFilePath.isReadable())
		{
			setStatus(Resource::Status::Error);
			qCritical() << "The file path" << m_oAbsoluteFilePath.filePath() << "does not exists or is unreadable!";
			throw ResourceException{QLatin1String{"The file path "} % m_oAbsoluteFilePath.filePath() % QLatin1String{" does not exists or is unreadable"}};
		}
		m_oFile = QSharedPointer<QFile>::create(m_oAbsoluteFilePath.filePath());
	}
}