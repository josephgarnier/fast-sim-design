/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SPRITE_VIEW_H
#define FAST_SIM_DESIGN_SPRITE_VIEW_H

#include "sprite_layer_view.h"

namespace FastSimDesign {
	class SpriteView final : public QGraphicsItem
	{
	public:
		explicit SpriteView(Tiled::MapObject const * const pMapObject, Tiled::MapRenderer const * const pRenderer, QWeakPointer<SpriteLayerView> oParent = QWeakPointer<SpriteLayerView>{}) noexcept;
		virtual ~SpriteView() = default; // Destructor

		// Enable the use of qgraphicsitem_cast with this item.
		enum {Type = UserType + 1};
		virtual inline int type() const noexcept override {return Type;}

		virtual QRectF boundingRect() const noexcept override;
		virtual QPainterPath shape() const noexcept override;
		virtual void paint(QPainter* pPainter, QStyleOptionGraphicsItem const * pOption, QWidget* pWidget = 0) noexcept override;

		inline Tiled::MapObject const * getSprite() const noexcept { return m_pMapObject; }
		void refreshScene() noexcept;
	protected:
		Tiled::MapObject const * const m_pMapObject;
		Tiled::MapRenderer const * const m_pRenderer;
		QRectF m_oBoundingRect;
	private:
	};
}
#endif
