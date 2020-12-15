/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_MAP_SCENE_H
#define FAST_SIM_DESIGN_MAP_SCENE_H

#include "QGraphicsScene"
#include "sprite_view.h"
#include "sprite_label_layer_view.h"
#include "sprite_label_view.h"

namespace FastSimDesign {
	class World;
	class MapScene final : public QGraphicsScene
	{
		Q_OBJECT
	private Q_SLOTS:
		void setGridVisible(bool bVisible);
		void setCollisionLayerVisible(bool bVisible);
		void setEntityLabelsVisible(bool bVisible);
		void onAllEntitiesUpdated() noexcept;

	public:
		explicit MapScene(World* const pWorld, Tiled::MapRenderer const* const pRenderer, QObject* pParent = nullptr); // Default constructor
		virtual ~MapScene() = default; // Destructor
	protected:
		virtual void drawForeground(QPainter* pPainter, QRectF const& oRect) noexcept override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent) noexcept override;

		void refreshScene() noexcept;
		void updateSceneRect() noexcept;
		QSharedPointer<QGraphicsItem> createLayerView(Tiled::Layer* pLayer) noexcept;
		QSharedPointer<SpriteLabelLayerView> createSpriteLabelLayerView() noexcept;

	protected:
		World* const m_pWorld;
		Tiled::MapRenderer const* const m_pRenderer;

		QMap<Tiled::Layer const*, QSharedPointer<QGraphicsItem>> m_oLayerViews;
		QMap<Tiled::MapObject const*, QSharedPointer<SpriteView>> m_oSpriteViews;
		QSharedPointer<SpriteLabelLayerView> m_oSpriteLabelLayerView;
		QVector<QSharedPointer<SpriteLabelView>> m_oSpriteLabelViews;

		bool m_bGridVisible;
		bool m_bCollisionLayerVisible;
		bool m_bEntityLabelsVisible;

	private:
	};
}
#endif
