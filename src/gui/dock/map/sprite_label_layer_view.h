/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SPRITE_LABEL_LAYER_VIEW_H
#define FAST_SIM_DESIGN_SPRITE_LABEL_LAYER_VIEW_H

#include "QGraphicsItem"
#include "tiled/maprenderer.h"

namespace FastSimDesign {
	class SpriteLabelLayerView final : public QGraphicsItem
	{
	public:
		explicit SpriteLabelLayerView(Tiled::MapRenderer const* const pRenderer, QGraphicsItem* pParent = nullptr) noexcept;
		virtual ~SpriteLabelLayerView() = default; // Destructor

		virtual QRectF boundingRect() const noexcept override;
		virtual void paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget = nullptr) noexcept override;

	protected:
		Tiled::MapRenderer const* const m_pRenderer;

	private:
	};
}
#endif
