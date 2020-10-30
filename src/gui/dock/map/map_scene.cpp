/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "map_scene.h"
#include "QGraphicsView"
#include "tile_layer_view.h"
#include "QGraphicsSceneMouseEvent"
#include "config/preferences.h"
#include "level/world.h"

namespace FastSimDesign {
	/*****************************************************************************
	Slots
	*****************************************************************************/
	void MapScene::setGridVisible(bool bVisible)
	{
		if (m_bGridVisible != bVisible)
		{
			m_bGridVisible = bVisible;
			update();
		}
	}

	void MapScene::setCollisionLayerVisible(bool bVisible)
	{
		if (m_bCollisionLayerVisible != bVisible)
		{
			m_bCollisionLayerVisible = bVisible;
			m_pWorld->setVisibleCollisionLayer(bVisible);
			Q_ASSERT_X(m_oLayerViews.contains(m_pWorld->getCollisionLayer()), "", "No collision layer found");
			QSharedPointer<QGraphicsItem> collisionLayer = m_oLayerViews[m_pWorld->getCollisionLayer()];
			collisionLayer->setVisible(bVisible);
			updateSceneRect();
			update();
		}
	}

	void MapScene::setEntityLabelsVisible(bool bVisible)
	{
		if (m_bEntityLabelsVisible != bVisible)
		{
			m_bEntityLabelsVisible = bVisible;
			m_oSpriteLabelLayerView->setVisible(bVisible);
			updateSceneRect();
			update();
		}
	}

	void MapScene::onAllEntitiesUpdated() noexcept
	{
		for (QSharedPointer<SpriteView>& spriteView : m_oSpriteViews)
			spriteView->refreshScene();
		if (m_oSpriteLabelLayerView->isVisible())
			for (QSharedPointer<SpriteLabelView>& spriteLabelView : m_oSpriteLabelViews)
				spriteLabelView->refreshScene();
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	MapScene::MapScene(World* const pWorld, Tiled::MapRenderer const* const pRenderer, QObject* pParent /*= Q_NULLPTR*/)
		: QGraphicsScene{pParent}
		, m_pWorld{pWorld}
		, m_oLayerViews{}
		, m_oSpriteViews{}
		, m_oSpriteLabelLayerView{}
		, m_pRenderer{pRenderer}
	{
		setBackgroundBrush(Qt::darkGray);

		Preferences& preference = Preferences::instance();
		QObject::connect(&preference, &Preferences::showGridChanged, this, &MapScene::setGridVisible);
		QObject::connect(&preference, &Preferences::showCollisionLayerChanged, this, &MapScene::setCollisionLayerVisible);
		QObject::connect(&preference, &Preferences::showEntityLabelsChanged, this, &MapScene::setEntityLabelsVisible);
		QObject::connect(m_pWorld, &World::allEntitiesUpdated, this, &MapScene::onAllEntitiesUpdated);

		m_bGridVisible = preference.showGrid();
		m_bCollisionLayerVisible = preference.showCollisionLayer();
		m_bEntityLabelsVisible = preference.showEntityLabels();

		refreshScene();
	}

	void MapScene::drawForeground(QPainter* pPainter, QRectF const& oRect) noexcept
	{
		if (!m_pWorld || !m_bGridVisible)
			return;

		QPointF offset{};
		m_pRenderer->drawGrid(pPainter, oRect.translated(-offset), Qt::black);
	}

	void MapScene::mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent) noexcept
	{
		switch (pMouseEvent->button())
		{
			case Qt::LeftButton:
			{
				QGraphicsView& view = *views().first();
				QGraphicsItem* clickedItem = itemAt(pMouseEvent->scenePos(), view.transform());

				if (SpriteView* spriteView = qgraphicsitem_cast<SpriteView*>(clickedItem))
				{
					m_pWorld->setSelectedSprite(*(spriteView->getSprite()));
				}
				break;
			}
			default:
				QGraphicsScene::mousePressEvent(pMouseEvent);
				break;
		}
	}

	void MapScene::refreshScene() noexcept
	{
		m_oLayerViews.clear();
		m_oSpriteViews.clear();
		clear();
		updateSceneRect();

		m_pWorld->setVisibleCollisionLayer(m_bCollisionLayerVisible);
		Tiled::Map const* const map = m_pWorld->getMap();
		int layerIndex = 0;
		for (Tiled::Layer* layer : map->layers())
		{
			QSharedPointer<QGraphicsItem> newLayerView = createLayerView(layer);
			newLayerView->setZValue(layerIndex);
			addItem(newLayerView.data());
			m_oLayerViews.insert(layer, newLayerView);
			layerIndex++;
		}

		m_oSpriteLabelLayerView = createSpriteLabelLayerView();
		m_oSpriteLabelLayerView->setZValue(10000 - 1);
		addItem(m_oSpriteLabelLayerView.data());
	}

	void MapScene::updateSceneRect() noexcept
	{
		QSize mapSize = m_pRenderer->mapBoundingRect().size();
		QRectF sceneRect{0.0f, 0.0f, static_cast<float>(mapSize.width()), static_cast<float>(mapSize.height())};
		setSceneRect(sceneRect);
	}

	QSharedPointer<QGraphicsItem> MapScene::createLayerView(Tiled::Layer* pLayer) noexcept
	{
		QSharedPointer<QGraphicsItem> layerView;
		if (Tiled::TileLayer* tileLayer = pLayer->asTileLayer())
		{
			layerView = QSharedPointer<TileLayerView>::create(tileLayer, m_pRenderer);

		} else if (Tiled::ObjectGroup* objectGroup = pLayer->asObjectGroup())
		{
			Tiled::ObjectGroup::DrawOrder drawOrder = objectGroup->drawOrder();
			QSharedPointer<SpriteLayerView> spriteLayerView = QSharedPointer<SpriteLayerView>::create(objectGroup, m_pRenderer);
			int viewIndex = 0;
			for (Tiled::MapObject* mapObject : objectGroup->objects())
			{
				QSharedPointer<SpriteView> spriteView = QSharedPointer<SpriteView>::create(mapObject, m_pRenderer, spriteLayerView);
				if (drawOrder == Tiled::ObjectGroup::TopDownOrder)
					spriteView->setZValue(spriteView->y());
				else
					spriteView->setZValue(viewIndex);
				spriteView->setVisible(mapObject->isVisible());
				m_oSpriteViews.insert(mapObject, spriteView);
				viewIndex++;
			}
			layerView = spriteLayerView;
		}
		Q_ASSERT_X(layerView, "", "Layer cannot be null");
		layerView->setVisible(pLayer->isVisible());
		return layerView;
	}

	QSharedPointer<SpriteLabelLayerView> MapScene::createSpriteLabelLayerView() noexcept
	{
		QSharedPointer<SpriteLabelLayerView> layerView = QSharedPointer<SpriteLabelLayerView>::create(m_pRenderer);
		int viewIndex = 0;
		for (QSharedPointer<SpriteView const> const& spriteView : m_oSpriteViews)
		{
			QSharedPointer<SpriteLabelView> spritelabelView = QSharedPointer<SpriteLabelView>::create(spriteView->getSprite(), m_pRenderer, layerView);
			spritelabelView->setZValue(viewIndex);
			spritelabelView->setVisible(spriteView->getSprite()->isVisible());
			m_oSpriteLabelViews.push_back(spritelabelView);
		}
		Q_ASSERT_X(layerView, "", "Layer cannot be null");
		layerView->setVisible(m_bEntityLabelsVisible);
		return layerView;
	}
}
