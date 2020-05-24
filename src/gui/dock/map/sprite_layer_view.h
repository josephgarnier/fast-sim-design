/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SPRITE_LAYER_VIEW_H
#define FAST_SIM_DESIGN_SPRITE_LAYER_VIEW_H

#include "QGraphicsItem"
// #include "tiled/mapobject.h"

namespace FastSimDesign {
	class SpriteLayerView final : public QGraphicsItem
	{
	public:
		explicit SpriteLayerView(Tiled::ObjectGroup const* const pLayer, Tiled::MapRenderer const* const pRenderer, QGraphicsItem* pParent = Q_NULLPTR) noexcept;
		virtual ~SpriteLayerView() = default; // Destructor

		virtual QRectF boundingRect() const noexcept override;
		virtual void paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget = Q_NULLPTR) noexcept override;

	protected:
		Tiled::MapRenderer const* const m_pRenderer;
		Tiled::ObjectGroup const* const m_pLayer;

	private:
	};
}
#endif