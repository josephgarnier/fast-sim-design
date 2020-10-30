/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "tile_layer_view.h"
#include "QStyleOptionGraphicsItem"

namespace FastSimDesign {
	TileLayerView::TileLayerView(Tiled::TileLayer const* const pLayer, Tiled::MapRenderer const* const pRenderer, QGraphicsItem* pParent /*= 0*/) noexcept
		: QGraphicsItem{pParent}
		, m_pLayer{pLayer}
		, m_pRenderer{pRenderer}
	{
		setFlag(QGraphicsItem::ItemUsesExtendedStyleOption);
		setOpacity(m_pLayer->opacity());
		setPos(m_pLayer->offset());
	}

	QRectF TileLayerView::boundingRect() const noexcept
	{
		return m_pRenderer->boundingRect(m_pLayer->bounds());
	}

	void TileLayerView::paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget /*= 0*/) noexcept
	{
		m_pRenderer->drawTileLayer(pPainter, m_pLayer, pOption->rect);
	}
}
