/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "sprite_view.h"
#include "tiled/objectgroup.h"
#include "tiled/mapobject.h"
#include "QToolTip"

namespace FastSimDesign {
	SpriteView::SpriteView(Tiled::MapObject const* const pMapObject, Tiled::MapRenderer const* const pRenderer, QWeakPointer<SpriteLayerView> oParent /*= QWeakPointer<EntityLayerView>()*/) noexcept
		: QGraphicsItem{oParent.data()}
		, m_pMapObject{pMapObject}
		, m_pRenderer{pRenderer}
		, m_oBoundingRect{}
	{
		refreshScene();
	}

	QRectF SpriteView::boundingRect() const noexcept
	{
		return m_oBoundingRect;
	}

	QPainterPath SpriteView::shape() const noexcept
	{
		QPainterPath path = m_pRenderer->shape(m_pMapObject);
		path.translate(-pos());
		return path;
	}

	void SpriteView::paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget /*= 0*/) noexcept
	{
		pPainter->translate(-pos());
		QColor const& color = m_pMapObject->objectGroup()->color();
		m_pRenderer->drawMapObject(pPainter, m_pMapObject, color.isValid() ? color : Qt::darkGray);
	}

	void SpriteView::refreshScene() noexcept
	{
		// --- Set tool tip ---
		QString redColor = "<span style=\" color:#ff0000; \">%1</span>";
		setToolTip(redColor.arg(m_pMapObject->name()));

		// --- Set position and rotation ---
		QPointF pixelPos = m_pRenderer->pixelToScreenCoords(m_pMapObject->position());
		setPos(pixelPos);
		setRotation(m_pMapObject->rotation());

		if (Tiled::ObjectGroup* objectGroup = m_pMapObject->objectGroup())
			if (objectGroup->drawOrder() == Tiled::ObjectGroup::TopDownOrder)
				setZValue(pixelPos.y());

		// --- Set bounding box ---
		QRectF boundingRect = m_pRenderer->boundingRect(m_pMapObject);
		boundingRect.translate(-pixelPos);
		if (m_oBoundingRect != boundingRect)
		{
			prepareGeometryChange();
			m_oBoundingRect = boundingRect;
		}

		setVisible(m_pMapObject->isVisible());
	}
}
