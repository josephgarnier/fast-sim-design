/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_VIEWS_UTIL_H
#define FAST_SIM_DESIGN_VIEWS_UTIL_H

#include "QProgressDialog"
#include "QtConcurrent"

namespace FastSimDesign {
	class ViewsUtil
	{
	public:
		explicit ViewsUtil() = default; // Default constructor
		ViewsUtil(ViewsUtil const&) = delete; // Copy constructor
		ViewsUtil(ViewsUtil&&) = default; // Move constructor
		ViewsUtil& operator=(ViewsUtil const&) = delete; // Copy assignment operator
		ViewsUtil& operator=(ViewsUtil&&) = default; // Move assignment operator
		virtual ~ViewsUtil() = default; // Destructor

		static QProgressDialog* const createProgressBox(QString sTitle, QObject const& oSender);

		template<typename T, typename Class>
		static void waitThreadProgressBox(QString sTitle, Class* pObject, T (Class::*fn)())
		{
			QFutureWatcher<void> futureWatcher;
			QProgressDialog* const dialog = ViewsUtil::createProgressBox(std::move(sTitle), futureWatcher);
			QFuture<void> future = QtConcurrent::run(pObject, fn);
			futureWatcher.setFuture(future);
			dialog->exec();
			futureWatcher.waitForFinished();
		}

	protected:
	private:
	};
}
#endif
