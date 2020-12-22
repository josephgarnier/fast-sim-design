/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world.h"
#include "resource/resource_manager.h"
#include "world_populator.h"
#include "resource/resource_exception.h"
#include "world_exception.h"
#include "util/collision_helper.h"
#include "pathfinding/path_finder.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	QSharedPointer<World> World::load(QString const& oWorldName)
	{
		try
		{
			// --- Initialize and load the resources ---
			qInfo() << "Loading map" << oWorldName << "from disk...";
			MapResource& mapResource = ResourceManager::instance().requestMap(oWorldName);
			qInfo() << "Map loaded successfully!";

			// --- Create and add the entities ---
			qInfo().nospace() << "Creating the world from map " << oWorldName << "...";
			QSharedPointer<World> oWorld = QSharedPointer<World>::create(mapResource.getMapRender(), mapResource.getMap(), mapResource.getCollisionLayer());
			qInfo() << "World created successfully!";
			WorldPopulator worldPopulator{mapResource.getTiledObjects()};
			qInfo() << "Populating world from being entities...";
			worldPopulator.populateWorld(oWorld);
			qInfo() << "World has been populated with" << oWorld->entityCount() << "entities.";

			// --- Init the world ---
			qInfo().nospace() << "Initializing the world " << oWorldName << "...";
			oWorld->init();
			qInfo() << "World initialized successfully!";

			return oWorld;
		} catch (ResourceException& e)
		{
			qCritical() << "ResourceException encountered when loading the world:" << e.what() << "!";
			throw WorldException{e.what()};
			return QSharedPointer<World>{};
		} catch (WorldException& e)
		{
			qCritical() << "WorldException encountered when populating the world:" << e.what() << "!";
			throw WorldException{e.what()};
			return QSharedPointer<World>{};
		}
	}

	QTime World::getTotalTime() noexcept
	{
		//Being::Time totalTime = Being::Clock::GetProgramTime();
		//return QTime{totalTime.HoursFormat(), totalTime.MinutesFormat(), totalTime.SecondsFormat(), totalTime.MilliFormat()};
		return QTime{0, 0, 0, 0};
	}

	QTime World::getWorldTime() noexcept
	{
		//Being::Time worldTime = Being::UpdateManager::GetInstance().GetGameTime();
		//return QTime{worldTime.HoursFormat(), worldTime.MinutesFormat(), worldTime.SecondsFormat(), worldTime.MilliFormat()};
		return QTime{0, 0, 0, 0};
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	World::World(Tiled::MapRenderer const* pMapRenderer, Tiled::Map* const pMap, Tiled::TileLayer* pCollisionLayer, QObject* const pParent /*= nullptr*/) noexcept
		: QObject{pParent}
		, m_oWorldInfoModel{new WorldInfoModel{this}}
		, m_pMapRenderer{pMapRenderer}
		, m_pMap{pMap}
		, m_pCollisionLayer{pCollisionLayer}
		, m_oEntities{new EntityStorage(this)}
	{
	}

	World::~World() noexcept
	{
		term();
		ResourceManager::instance().clearAllResources();
	}

	void World::init() noexcept
	{
		m_oWorldInfoModel->setTotalTime(World::getTotalTime());
		m_oWorldInfoModel->setWorldTime(World::getWorldTime());
		m_oWorldInfoModel->setCurrentTick(0);
		m_oEntities->init();
	}

	void World::update(QTime const& oDeltaTime) noexcept
	{
		// m_oEntities.update();
		/*Being::Time dt{};
		dt.FromMilli(Time::toMilliseconds(oDeltaTime));

		// --- Being is used as a making-decision engine ---
		Being::UpdateManager::GetInstance().Update(dt, dt);

		qInfo() << "~~~ Updating entities from Being ~~~";
		// --- Update the entity from knowledge data ---
		for (QSharedPointer<Entity>& entity : m_oEntities)
			entity->UpdateFromBeingToEntity(oDeltaTime);

		// --- Applies the decisions on all entities ---
		for (QSharedPointer<Entity>& entity : m_oEntities)
			entity->Update(oDeltaTime);

		// --- Update the knowledge of all entities ---
		for (QSharedPointer<Entity>& entity : m_oEntities)
			entity->UpdateFromEntityToBeing(oDeltaTime);

		// --- Update all the models data ---
		for (QSharedPointer<Entity> entity : m_oEntities)
			entity->UpdateModels(oDeltaTime);
		qInfo() << "~~~ Finished updating entities from Being ~~~";

		qInfo() << "~~~ Updating views from entities ~~~";
		emit AllEntitiesUpdated();
		m_oWorldInfoModel->SetTotalTime(World::GetTotalTime());
		m_oWorldInfoModel->SetWorldTime(World::GetWorldTime());
		m_oWorldInfoModel->IncrementCurrentTick();
		emit WorldUpdated();
		qInfo() << "~~~ Finished updating views from entities ~~~";*/
	}

	void World::term() noexcept
	{
		m_oEntities->term();
	}

	bool World::isCoordExists(Location const& oCoord) const noexcept
	{
		return oCoord.isValid();
	}

	bool World::isCoordExists(double dX, double dY) const noexcept
	{
		return dX >= 0 && dX < getWidth() && dY >= 0 && dY < getHeight();
	}

	bool World::isTileExists(Location const& oTile) const noexcept
	{
		return m_pCollisionLayer->contains(oTile.getTileX(), oTile.getTileY());
	}

	bool World::isTileExists(int iX, int iY) const noexcept
	{
		return m_pCollisionLayer->contains(iX, iY);
	}

	bool World::isCollide(Entity const& oEntity) const noexcept
	{
		for (Entity const* other : m_oEntities->getAllConstPtr())
			if (other->id() != oEntity.id() && other->isCollideWith(oEntity))
				return true;
		return false;
	}

	bool World::isWalkableTileLocation(Location const& oLocation) const noexcept
	{
		CollisionHelper helper{m_pCollisionLayer};
		return helper.isWalkableTileLocation(oLocation.toTilePoint());
	}

	bool World::isWalkableTileLocation(Entity const& oEntity) const noexcept
	{
		CollisionHelper helper{m_pCollisionLayer};
		return helper.isWalkableTileLocation(oEntity.getBoundingBox());
	}

	bool World::isWalkableTileDestination(Location const& oLocation, VectorLoc const& oDistanceOffset) const noexcept
	{
		CollisionHelper helper{m_pCollisionLayer};
		return helper.isWalkableTileDestination(oLocation.toTilePoint(), oDistanceOffset.toVector());
	}

	bool World::isWalkableTileDestination(Entity const& oEntity, VectorLoc const& oDistanceOffset) const noexcept
	{
		CollisionHelper helper{m_pCollisionLayer};
		return helper.isWalkableTileDestination(oEntity.getBoundingBox(), oDistanceOffset.toVector());
	}

	void World::setSelectedSprite(Tiled::MapObject const& oSelectedSprite) noexcept
	{
		for (auto entity : m_oEntities->getAllPtr())
			entity->setEnableModelUpdating(false);
		Entity& selectedEntity = m_oEntities->get<Entity>(oSelectedSprite);
		selectedEntity.setEnableModelUpdating(true);
		emit selectedEntityChanged(selectedEntity);
	}

	EntityStorage::EntityContainerPtr World::getEntitiesInFront(Entity const& oEntity, double dRadius) const noexcept
	{
		EntityStorage::EntityContainerPtr entitiesAround{};
		for (Entity* other : m_oEntities->getAllPtr())
		{
			if (other->id() != oEntity.id() && oEntity.canSeeInFront(*other, dRadius))
				entitiesAround.push_back(other);
		}
		return entitiesAround;
	}

	void World::destroyAllEntities() noexcept
	{
		m_oEntities->clear();
	}

	Location World::getRandomWalkableLocation() const noexcept
	{
		Location randomLocation{};
		qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
		do
		{
			double xCoord = qrand() % (getWidth() - 1);
			double yCoord = qrand() % (getHeight() - 1);
			randomLocation = Location{sharedFromThis(), xCoord, yCoord};
			Q_ASSERT_X(randomLocation.isValid(), "", "Random location is not valid");
		} while (!isWalkableTileLocation(randomLocation));
		return randomLocation;
	}

	Path World::findPath(Entity const& oFrom, Entity const& oTo) const noexcept
	{
		PathFinder finder{sharedFromThis()};
		return finder.createEntityPathToPoint(oFrom, oTo);
	}

	Path World::findPath(Entity const& oFrom, Location const& oTo) const noexcept
	{
		PathFinder finder{sharedFromThis()};
		return finder.createEntityPathToPoint(oFrom, oTo);
	}
}
