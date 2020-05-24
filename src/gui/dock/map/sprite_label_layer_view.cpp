/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "sprite_label_layer_view.h"

namespace FastSimDesign {
	SpriteLabelLayerView::SpriteLabelLayerView(Tiled::MapRenderer const* const pRenderer, QGraphicsItem* pParent /*= Q_NULLPTR*/) noexcept
		: QGraphicsItem{pParent}
		, m_pRenderer{pRenderer}
	{
		setFlag(QGraphicsItem::ItemHasNoContents);
	}

	QRectF SpriteLabelLayerView::boundingRect() const noexcept
	{
		return QRectF{};
	}

	void SpriteLabelLayerView::paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget /*= Q_NULLPTR*/) noexcept
	{
	}
}