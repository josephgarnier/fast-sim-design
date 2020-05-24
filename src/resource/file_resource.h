/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_FILE_RESOURCE_H
#define FAST_SIM_DESIGN_FILE_RESOURCE_H

#include "resource.h"

namespace FastSimDesign {
	class FileResource final : public Resource
	{
		Q_OBJECT
	public:
		explicit FileResource(QFileInfo oFilePath, QObject* pParent = Q_NULLPTR); // throw ResourceException
		virtual ~FileResource() = default; // Destructor

		inline QSharedPointer<QFile> const getFile() const noexcept { return m_oFile; }

	protected:
		void loadFile(); // throw ResourceException
	protected:
		QSharedPointer<QFile> m_oFile;

	private:
	};
}
#endif