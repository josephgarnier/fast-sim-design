/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SPRITE_LABEL_VIEW_H
#define FAST_SIM_DESIGN_SPRITE_LABEL_VIEW_H

#include "QGraphicsItem"
#include "sprite_label_layer_view.h"
#include "tiled/tiled.h"

namespace FastSimDesign {
	class SpriteLabelView final : public QGraphicsItem
	{
	public:
		static QPointF alignmentOffset(QRectF& oRect, Tiled::Alignment oAlignment) noexcept;
		static void align(QRectF& oRect, Tiled::Alignment oAlignment) noexcept;
		static QRectF objectBounds(Tiled::MapObject const* const pMapObject, Tiled::MapRenderer const* const pRenderer) noexcept;

	public:
		explicit SpriteLabelView(Tiled::MapObject const* const pMapObject, Tiled::MapRenderer const* const pRenderer, QWeakPointer<SpriteLabelLayerView> oParent = QWeakPointer<SpriteLabelLayerView>{}) noexcept;
		virtual ~SpriteLabelView() = default; // Destructor

		virtual QRectF boundingRect() const noexcept override;
		virtual void paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget = 0) noexcept override;

		void refreshScene() noexcept;

	protected:
		Tiled::MapObject const* const m_pMapObject;
		Tiled::MapRenderer const* const m_pRenderer;
		QRectF m_oBoundingRect;
		double m_iLabelDistance;
		double m_iLabelMargin;

	private:
	};
}
#endif
