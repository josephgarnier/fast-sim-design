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

#include "QEnableSharedFromThis"
#include "tiled/map.h"

#include "entity/object.h"
#include "entity/npc.h"
#include "entity/entity_storage.h"
#include "level/world_info_model.h"
#include "pathfinding/path.h"

namespace FastSimDesign {
	class World final : public QObject
		, public QEnableSharedFromThis<World>
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
		explicit World(Tiled::MapRenderer const* const pMapRenderer, Tiled::Map* const pMap, Tiled::TileLayer* const pCollisionLayer, QObject* pParent = nullptr) noexcept;
		virtual ~World() noexcept; // Destructor

		void init() noexcept;
		void update(QTime const& oDeltaTime) noexcept;
		void term() noexcept;

		inline WorldInfoModel* getWorldInfoModel() const noexcept { return m_oWorldInfoModel.data(); }
		inline Tiled::MapRenderer const* getMapRender() const noexcept { return m_pMapRenderer; }
		inline Tiled::Map const* getMap() const noexcept { return m_pMap; }
		inline EntityStorage& getEntityStorage() noexcept { return *m_oEntities.get(); }
		inline int getHeight() const noexcept { return m_pMap->height() * m_pMap->tileHeight(); }
		inline int getWidth() const noexcept { return m_pMap->width() * m_pMap->tileWidth(); }
		inline int getTileHeight() const noexcept { return m_pMap->tileHeight(); }
		inline int getTileWidth() const noexcept { return m_pMap->tileWidth(); }

		inline Tiled::TileLayer const* getCollisionLayer() const noexcept { return m_pCollisionLayer; }
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

		inline std::size_t entityCount() const noexcept { return m_oEntities->count(); }
		inline EntityStorage::EntityContainerPtr getEntities() const noexcept { return m_oEntities->getAllPtr(); }
		EntityStorage::EntityContainerPtr getEntitiesInFront(Entity const& oEntity, double dRadius) const noexcept;
		template<typename EntityType>
		inline EntityType& getEntity(QString const& sName) const noexcept { return m_oEntities->get<EntityType>(sName); }
		template<typename EntityType>
		inline EntityType& getEntity(Entity::Id const& id) const noexcept { return m_oEntities->get<EntityType>(id); }
		template<typename EntityType>
		inline EntityType& getEntity(Tiled::MapObject const& sprite) const noexcept { return m_oEntities->get<EntityType>(sprite); }

		template<typename EntityType, typename... EntityArgs>
		Entity::Id createEntity(EntityArgs&&... args) noexcept;
		void destroyAllEntities() noexcept;

		Location getRandomWalkableLocation() const noexcept;
		Path findPath(Entity const& oFrom, Entity const& oTo) const noexcept;
		Path findPath(Entity const& oFrom, Location const& oTo) const noexcept;

	protected:
		QScopedPointer<WorldInfoModel> m_oWorldInfoModel;
		Tiled::MapRenderer const* m_pMapRenderer;
		Tiled::Map* m_pMap;
		Tiled::TileLayer* m_pCollisionLayer;
		std::unique_ptr<EntityStorage> m_oEntities;

	private:
	};
}
#include "world.tpp"
#endif
