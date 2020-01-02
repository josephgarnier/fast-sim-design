/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "resource_manager.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	FastSimDesign::ResourceManager& ResourceManager::instance() noexcept
	{
		static ResourceManager ms_oInstance;
		return ms_oInstance;
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	MapResource& ResourceManager::requestMap(QString const& oFileName)
	{
		try
		{
			QFileInfo absoluteFilePath{QLatin1String{"assets/maps/"} % oFileName};
			absoluteFilePath.makeAbsolute();
			QLatin1String mapExtension{"tmx"};
			if (absoluteFilePath.suffix() != mapExtension)
			{
				absoluteFilePath = QFileInfo{absoluteFilePath.filePath() % QLatin1String{"."} % mapExtension};
			}
			Q_ASSERT_X(absoluteFilePath.isAbsolute(), "", "The path is not absolute");

			if (!hasResource(absoluteFilePath))
			{
				QSharedPointer<MapResource> mapCreated = QSharedPointer<MapResource>::create(absoluteFilePath);
				m_oResources.insert(absoluteFilePath.filePath(), std::move(mapCreated));
			}
			MapResource& map = findResource<MapResource>(absoluteFilePath);
			return map;
		} catch (...)
		{
			throw;
		}
	}

	FileResource& ResourceManager::requestFile(QString const& oFileName)
	{
		try
		{
			QFileInfo absoluteFilePath{QLatin1String{"assets/"} % oFileName};
			absoluteFilePath.makeAbsolute();
			Q_ASSERT_X(absoluteFilePath.isAbsolute(), "", "The path is not absolute");

			if (!hasResource(absoluteFilePath))
			{
				QSharedPointer<FileResource> fileCreated = QSharedPointer<FileResource>::create(absoluteFilePath);
				m_oResources.insert(absoluteFilePath.filePath(), std::move(fileCreated));
			}
			FileResource& file = findResource<FileResource>(absoluteFilePath);
			return file;
		} catch (...)
		{
			throw;
		}
	}

	void ResourceManager::clearAllResources()
	{
		for (QSharedPointer<Resource>& resource : m_oResources)
		{
			resource.reset();
		}
		m_oResources.clear();
	}
}