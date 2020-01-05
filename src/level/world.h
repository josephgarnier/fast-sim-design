/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_WORLD_H
#define FAST_SIM_DESIGN_WORLD_H

#include "tield/map.h"
#include "entity/entity.h"
#include "pathfinding/path.h"
#include "world_populator.h"
#include "world_info_model.h"

namespace FastSimDesign {
	class Object;
	class Npc;
	class World final : public QObject, public QEnableSharedFromThis<World>
	{
		Q_OBJECT
	Q_SIGNALS:
		void allEntitiesUpdated();
		void worldUpdated();
		void selectedEntityChanged(Entity const& oSelectedEntity);

	public:
		static QSharedPointer<World> load(QString const& oWorldName); // throw WorldException
		static QTime getTotalTime() noexcept;
		static QTime getWorldTime() noexcept;

	public:
		explicit World(Tiled::MapRenderer const* const pMapRenderer, Tiled::Map* const pMap, Tiled::TileLayer* const pCollisionLayer, QObject* pParent = Q_NULLPTR) noexcept;
		World(World&&) = default; // Move constructor
		World& operator=(World&&) = default; // Move assignment operator
		virtual ~World() noexcept; // Destructor

		void init() noexcept;
		void update(QTime const& oDeltaTime) noexcept;
		void term() noexcept;

		inline WorldInfoModel* getWorldInfoModel() const noexcept { return m_oWorldInfoModel.data(); }
		inline Tiled::MapRenderer const* const getMapRender() const noexcept { return m_pMapRenderer; }
		inline Tiled::Map const* const getMap() const noexcept { return m_pMap; }
		inline int getHeight() const noexcept { return m_pMap->height() * m_pMap->tileHeight(); }
		inline int getWidth() const noexcept { return m_pMap->width() * m_pMap->tileWidth(); }
		inline int getTileHeight() const noexcept { return m_pMap->tileHeight(); }
		inline int getTileWidth() const noexcept { return m_pMap->tileWidth(); }

		inline Tiled::TileLayer const* const getCollisionLayer() const noexcept { return m_pCollisionLayer; }
		inline void setVisibleCollisionLayer(bool bVisible) noexcept { m_pCollisionLayer->setVisible(bVisible); }

		bool isCoordExists(Location const& oCoord) const noexcept;
		bool isCoordExists(double dX, double dY) const noexcept;
		bool isTileExists(Location const& oTile) const noexcept;
		bool isTileExists(int iX, int iY) const noexcept;

		bool isCollide(Entity const& oEntity) const noexcept;
		bool isWalkableTileLocation(Location const& oLocation) const noexcept;
		bool isWalkableTileLocation(Entity const& oEntity) const noexcept;
		bool isWalkableTileDestination(Location const& oLocation, VectorLoc const& oDistanceOffset) const noexcept;
		bool isWalkableTileDestination(Entity const& oEntity, VectorLoc const& oDistanceOffset) const noexcept;

		void setSelectedSprite(Tiled::MapObject const& oSelectedSprite) noexcept;
		inline QVector<QSharedPointer<Entity>> const& getEntities() const noexcept { return m_oEntities; }
		QVector<QSharedPointer<Entity>> getEntitiesInFront(Entity const& oEntity, double dRadius) const noexcept;
		QSharedPointer<Entity> getEntityByName(QString const& sName) const noexcept;

		template<typename T>
		QSharedPointer<T> getEntityById(int iId) const noexcept
		{
			for (auto entity : m_oEntities)
			{
				if (entity->getId() == iId)
				{
					QSharedPointer<T> castedEntity = entity.dynamicCast<T>();
					Q_ASSERT_X(!castedEntity.isNull(), "", "This entity cannot be cast to T type");
					return castedEntity;
				}
			}
			Q_ASSERT_X(false, "", "Requested entity does not exists");
			return QSharedPointer<T>{};
		}

		template<typename T>
		QSharedPointer<T> getEntityBySprite(Tiled::MapObject const& oSprite) const noexcept
		{
			for (auto entity : m_oEntities)
			{
				if (entity->isOwnSprite(oSprite))
				{
					QSharedPointer<T> castedEntity = entity.dynamicCast<T>();
					Q_ASSERT_X(!castedEntity.isNull(), "", "This entity cannot be cast to T type");
					return castedEntity;
				}
			}
			Q_ASSERT_X(false, "", "Requested entity does not exists");
			return QSharedPointer<T>{};
		}

		void addEntity(QSharedPointer<Entity> oEntity) noexcept;
		void removeAllEntities() noexcept;

		Location getRandomWalkableLocation() const noexcept;
		Path findPath(Entity const& oFrom, Entity const& oTo) const noexcept;
		Path findPath(Entity const& oFrom, Location const& oTo) const noexcept;

	protected:
		QScopedPointer<WorldInfoModel> m_oWorldInfoModel;
		Tiled::MapRenderer const* m_pMapRenderer;
		Tiled::Map* m_pMap;
		Tiled::TileLayer* m_pCollisionLayer;
		QVector<QSharedPointer<Entity>> m_oEntities;

	private:
	};
}
#endif
