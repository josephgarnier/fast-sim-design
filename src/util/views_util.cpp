/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "views_util.h"

namespace FastSimDesign {
	QProgressDialog* const ViewsUtil::createProgressBox(QString sTitle, QObject const& oSender)
	{
		QProgressDialog* dialog = new QProgressDialog{};
		dialog->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
		dialog->setLabelText(QString{std::move(sTitle)});
		dialog->setCancelButton(0);
		QObject::connect(&oSender, SIGNAL(finished()), dialog, SLOT(reset()));
		QObject::connect(dialog, SIGNAL(canceled()), &oSender, SLOT(cancel()));
		QObject::connect(&oSender, SIGNAL(progressRangeChanged(int, int)), dialog, SLOT(setRange(int, int)));
		QObject::connect(&oSender, SIGNAL(progressValueChanged(int)), dialog, SLOT(setValue(int)));
		return dialog;
	}
}