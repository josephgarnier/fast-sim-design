/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "sprite_label_view.h"
#include "tiled/tile.h"
#include "tiled/objectgroup.h"
#include "sprite_label_layer_view.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	QPointF SpriteLabelView::alignmentOffset(QRectF& oRect, Tiled::Alignment oAlignment) noexcept
	{
		switch (oAlignment)
		{
			case Tiled::Alignment::TopLeft: break;
			case Tiled::Alignment::Top: return QPointF(oRect.width() / 2, 0); break;
			case Tiled::Alignment::TopRight: return QPointF(oRect.width(), 0); break;
			case Tiled::Alignment::Left: return QPointF(0, oRect.height() / 2); break;
			case Tiled::Alignment::Center: return QPointF(oRect.width() / 2, oRect.height() / 2); break;
			case Tiled::Alignment::Right: return QPointF(oRect.width(), oRect.height() / 2); break;
			case Tiled::Alignment::BottomLeft: return QPointF(0, oRect.height()); break;
			case Tiled::Alignment::Bottom: return QPointF(oRect.width() / 2, oRect.height()); break;
			case Tiled::Alignment::BottomRight: return QPointF(oRect.width(), oRect.height()); break;
		}
		return QPointF{};
	}

	void SpriteLabelView::align(QRectF& oRect, Tiled::Alignment oAlignment) noexcept
	{
		oRect.translate(-SpriteLabelView::alignmentOffset(oRect, oAlignment));
	}

	QRectF SpriteLabelView::objectBounds(Tiled::MapObject const* const pMapObject, Tiled::MapRenderer const* const pRenderer) noexcept
	{
		if (!pMapObject->cell().isEmpty())
		{
			// Tile objects can have a tile offset, which is scaled along with the image
			Tiled::Tile* tile = pMapObject->cell().tile();
			QSize imgSize = tile->image().size();
			QPointF position = pRenderer->pixelToScreenCoords(pMapObject->position());

			QPoint tileOffset = tile->tileset()->tileOffset();
			QSizeF objectSize = pMapObject->size();
			qreal scaleX = imgSize.width() > 0 ? objectSize.width() / imgSize.width() : 0;
			qreal scaleY = imgSize.height() > 0 ? objectSize.height() / imgSize.height() : 0;

			QRectF bounds(position.x() + (tileOffset.x() * scaleX),
				position.y() + (tileOffset.y() * scaleY),
				objectSize.width(),
				objectSize.height());

			SpriteLabelView::align(bounds, pMapObject->alignment());

			return bounds;
		} else
		{
			switch (pMapObject->shape())
			{
				case Tiled::MapObject::Ellipse:
				case Tiled::MapObject::Rectangle:
				{
					QRectF bounds(pMapObject->bounds());
					SpriteLabelView::align(bounds, pMapObject->alignment());
					QPolygonF screenPolygon = pRenderer->pixelToScreenCoords(bounds);
					return screenPolygon.boundingRect();
				}
				case Tiled::MapObject::Polygon:
				case Tiled::MapObject::Polyline:
				{
					QPointF const& pos = pMapObject->position();
					QPolygonF polygon = pMapObject->polygon().translated(pos);
					QPolygonF screenPolygon = pRenderer->pixelToScreenCoords(polygon);
					return screenPolygon.boundingRect();
				}
				default:
					break;
			}
		}

		return QRectF{};
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	SpriteLabelView::SpriteLabelView(Tiled::MapObject const* const pMapObject, Tiled::MapRenderer const* const pRenderer, QWeakPointer<SpriteLabelLayerView> oParent /*= QWeakPointer<SpriteLabelLayer>()*/) noexcept
		: QGraphicsItem{oParent.data()}
		, m_pMapObject{pMapObject}
		, m_pRenderer{pRenderer}
		, m_oBoundingRect{}
		, m_iLabelDistance{12}
		, m_iLabelMargin{3}
	{
		setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemIgnoresParentOpacity);
		refreshScene();
	}

	QRectF SpriteLabelView::boundingRect() const noexcept
	{
		return m_oBoundingRect.adjusted(0, 0, 1, 1);
	}

	void SpriteLabelView::paint(QPainter* pPainter, QStyleOptionGraphicsItem const* pOption, QWidget* pWidget /*= 0*/) noexcept
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
		pPainter->setBrush(Qt::black);
		pPainter->setPen(Qt::NoPen);
		pPainter->drawRoundedRect(m_oBoundingRect.translated(1, 1), 4, 4);
		pPainter->setBrush(Qt::red);
		pPainter->drawRoundedRect(m_oBoundingRect, 4, 4);

		QPointF textPos{-(m_oBoundingRect.width() - m_iLabelMargin * 4) / 2, -m_iLabelDistance};

		pPainter->drawRoundedRect(m_oBoundingRect, 4, 4);
		pPainter->setPen(Qt::black);
		pPainter->drawText(textPos + QPointF(1, 1), m_pMapObject->name());
		pPainter->setPen(Qt::white);
		pPainter->drawText(textPos, m_pMapObject->name());
	}

	void SpriteLabelView::refreshScene() noexcept
	{
		bool nameVisible = m_pMapObject->isVisible() && !m_pMapObject->name().isEmpty();
		setVisible(nameVisible);
		if (!nameVisible)
			return;

		QPointF pixelPos = m_pRenderer->pixelToScreenCoords(m_pMapObject->position());
		QRectF bounds = SpriteLabelView::objectBounds(m_pMapObject, m_pRenderer);

		// --- Adjust the bounding box for object rotation ---
		QTransform transform{};
		transform.translate(pixelPos.x(), pixelPos.y());
		transform.rotate(m_pMapObject->rotation());
		transform.translate(-pixelPos.x(), -pixelPos.y());
		bounds = transform.mapRect(bounds);

		// --- Center the object name on the object bounding box ---
		QPointF pos{(bounds.left() + bounds.right()) / 2, bounds.top()};
		setPos(pos + m_pMapObject->objectGroup()->offset());

		// --- Set bounding box ---
		QFontMetricsF metrics{QGuiApplication::font()};
		QRectF boundingRect = metrics.boundingRect(m_pMapObject->name());
		boundingRect.translate(-boundingRect.width() / 2, -m_iLabelDistance);
		boundingRect.adjust(-m_iLabelMargin * 2, -m_iLabelMargin, m_iLabelMargin * 2, m_iLabelMargin);
		if (m_oBoundingRect != boundingRect)
		{
			prepareGeometryChange();
			m_oBoundingRect = boundingRect;
		}
	}

}