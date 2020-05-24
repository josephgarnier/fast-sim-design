/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_RESOURCE_H
#define FAST_SIM_DESIGN_RESOURCE_H

#include "QFileInfo"

namespace FastSimDesign {
	class Resource : public QObject
	{
		Q_OBJECT
	private:
		using Parent = QObject;

	public:
		enum class Status
		{
			Null,
			Ready,
			Error,
			Loading
		};

		explicit Resource(QFileInfo oPath, QObject* pParent = Q_NULLPTR) noexcept;
		virtual ~Resource() = default; // Destructor

		inline Resource::Status const& getStatus() const noexcept { return m_oStatus; }
		inline void setStatus(Resource::Status oNewStatus) noexcept
		{
			if (m_oStatus != oNewStatus)
				m_oStatus = std::move(oNewStatus);
		}

		inline QFileInfo const& absoluteFilePath() const noexcept { return m_oAbsoluteFilePath; }

	protected:
		void checkReady() noexcept;

	protected:
		QFileInfo m_oAbsoluteFilePath;

	private:
		Status m_oStatus;
	};
}
#endif
