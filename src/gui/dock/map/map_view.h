/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_MAP_VIEW_H
#define FAST_SIM_DESIGN_MAP_VIEW_H

#include "QGraphicsView"

namespace FastSimDesign {
	class MapView final : public QGraphicsView
	{
		Q_OBJECT
	public:
		explicit MapView(QWidget* pParent = nullptr) noexcept;
		virtual ~MapView() = default; // Destructor
	protected:
	private:
	};
}
#endif