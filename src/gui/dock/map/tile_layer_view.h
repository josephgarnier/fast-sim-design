/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_TILE_LAYER_VIEW_H
#define FAST_SIM_DESIGN_TILE_LAYER_VIEW_H

#include "QGraphicsItem"
#include "tiled/tilelayer.h"
#include "tiled/maprenderer.h"

namespace FastSimDesign {
	/*!
	Item that represents a tile layer.
	*/
	class TileLayerView final : public QGraphicsItem
	{
	public:
		explicit TileLayerView(Tiled::TileLayer const * const pLayer, Tiled::MapRenderer const * const pRenderer, QGraphicsItem* pParent = Q_NULLPTR) noexcept;
		virtual ~TileLayerView() = default; // Destructor

		virtual QRectF boundingRect() const noexcept override;
		virtual void paint(QPainter* pPainter, QStyleOptionGraphicsItem const * pOption, QWidget* pWidget = Q_NULLPTR) noexcept override;
	protected:
		Tiled::TileLayer const * const m_pLayer;
		Tiled::MapRenderer const * const m_pRenderer;
	private:
	};
}
#endif
