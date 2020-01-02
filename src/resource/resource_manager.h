/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_RESOURCE_MANAGER_H
#define FAST_SIM_DESIGN_RESOURCE_MANAGER_H

#include "resource.h"
#include "map_resource.h"
#include "file_resource.h"

namespace FastSimDesign {
	class ResourceManager final
	{
	public:
		static ResourceManager& instance() noexcept;

	public:
		MapResource& requestMap(QString const& oFileName); // throw ResourceException
		FileResource& requestFile(QString const& oFileName); // throw ResourceException

		void clearAllResources();

	protected:
		template<typename R>
		inline R& findResource(QFileInfo const& oPath) noexcept
		{
			return *(dynamic_cast<R*>(m_oResources.value(oPath.absoluteFilePath()).data()));
		}

		inline bool hasResource(QFileInfo const& oPath) noexcept
		{
			return m_oResources.contains(oPath.absoluteFilePath());
		}

	private:
		explicit ResourceManager() = default; // Default constructor
		ResourceManager(ResourceManager const&) = delete; // Copy constructor
		ResourceManager(ResourceManager&&) = default; // Move constructor
		ResourceManager& operator=(ResourceManager const&) = delete; // Copy assignment operator
		ResourceManager& operator=(ResourceManager&&) = default; // Move assignment operator
		virtual ~ResourceManager() = default; // Destructor
	protected:
		QMap<QString, QSharedPointer<Resource>> m_oResources; // !!! QMap does not supports move semantic yet. Meanwhile we have to use temporarily a QSharedPointer !!!
	private:
	};
}
#endif
