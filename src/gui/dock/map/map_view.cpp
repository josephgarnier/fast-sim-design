/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "map_view.h"
#include "tield/hexagonalrenderer.h"
#include "tield/isometricrenderer.h"
#include "tield/orthogonalrenderer.h"
#include "tield/staggeredrenderer.h"
#include "map_scene.h"

namespace FastSimDesign {
	MapView::MapView(QWidget* parent /*= 0*/) noexcept
		: QGraphicsView{parent}
	{
	}
}