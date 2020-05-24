/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "map_view.h"
#include "map_scene.h"
#include "tiled/hexagonalrenderer.h"
#include "tiled/isometricrenderer.h"
#include "tiled/orthogonalrenderer.h"
#include "tiled/staggeredrenderer.h"

namespace FastSimDesign {
	MapView::MapView(QWidget* parent /*= 0*/) noexcept
		: QGraphicsView{parent}
	{
	}
}