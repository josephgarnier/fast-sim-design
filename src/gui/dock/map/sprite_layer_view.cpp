/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "sprite_layer_view.h"
#include "tiled/objectgroup.h"
#include "sprite_view.h"

namespace FastSimDesign {
	SpriteLayerView::SpriteLayerView(Tiled::ObjectGroup const* const pLayer, Tiled::MapRenderer const* const pRenderer, QGraphicsItem* pParent /*= 0*/) noexcept
		: QGraphicsItem{pParent}
		, m_pRenderer{pRenderer}
		, m_pLayer{pLayer}
	{
		setFlag(QGraphicsItem::ItemHasNoContents);
		setOpacity(m_pLayer->opacity());
		setPos(m_pLayer->offset());
	}

	QRectF SpriteLayerView::boundingRect() const noexcept
	{
		return QRectF{};
	}

	void SpriteLayerView::paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget /*= 0*/) noexcept
	{
	}
}
