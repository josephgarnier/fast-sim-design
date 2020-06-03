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
			qInfo() << "World has been populated with" << oWorld->getEntities().size() << "entities.";

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
	World::World(Tiled::MapRenderer const* pMapRenderer, Tiled::Map* const pMap, Tiled::TileLayer* pCollisionLayer, QObject* const pParent /*= Q_NULLPTR*/) noexcept
		: QObject{pParent}
		, m_oWorldInfoModel{new WorldInfoModel{this}}
		, m_pMapRenderer{pMapRenderer}
		, m_pMap{pMap}
		, m_pCollisionLayer{pCollisionLayer}
		, m_oEntities{}
	{
	}

	World::~World() noexcept
	{
		term();
		ResourceManager::instance().clearAllResources();
	}

	void World::init() noexcept
	{
		Q_ASSERT_X(!m_oEntities.empty(), "", "No entities in the world. It has to be populated");
		m_oWorldInfoModel->setTotalTime(World::getTotalTime());
		m_oWorldInfoModel->setWorldTime(World::getWorldTime());
		m_oWorldInfoModel->setCurrentTick(0);
		for (QSharedPointer<Entity>& entity : m_oEntities)
		{
			qInfo().nospace() << "Initializing entity \"" << entity->getId() << "\"...";
			entity->init();
			qInfo().nospace() << "Entity \"" << entity->getId() << "\" initialized successfully!";
		}
	}

	void World::update(QTime const& oDeltaTime) noexcept
	{
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
		for (QSharedPointer<Entity>& entity : m_oEntities)
		{
			qInfo().nospace() << "Terminating entity \"" << entity->getId() << "\"...";
			entity->term();
			qInfo().nospace() << "Entity \"" << entity->getId() << "\" terminated successfully!";
		}
		removeAllEntities();
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
		for (QSharedPointer<Entity> other : m_oEntities)
			if (other->getId() != oEntity.getId() && other->isCollideWith(oEntity))
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
		for (auto entity : m_oEntities)
			entity->setEnableModelUpdating(false);
		QSharedPointer<Entity> selectedEntity = getEntityBySprite<Entity>(oSelectedSprite);
		selectedEntity->setEnableModelUpdating(true);
		emit selectedEntityChanged(*selectedEntity.data());
	}

	QVector<QSharedPointer<Entity>> World::getEntitiesInFront(Entity const& oEntity, double dRadius) const noexcept
	{
		QVector<QSharedPointer<Entity>> entitiesAround{};
		for (QSharedPointer<Entity> other : m_oEntities)
		{
			if (other->getId() != oEntity.getId() && oEntity.canSeeInFront(*other.data(), dRadius))
				entitiesAround.push_back(other);
		}
		return entitiesAround;
	}

	QSharedPointer<Entity> World::getEntityByName(QString const& sName) const noexcept
	{
		for (auto entity : m_oEntities)
		{
			if (entity->getName() == sName) return entity;
		}
		Q_ASSERT_X(false, "", "The requested entity does not exists");
		return QSharedPointer<Entity>{};
	}

	void World::addEntity(QSharedPointer<Entity> oEntity) noexcept
	{
		Q_ASSERT_X(!isCollide(*oEntity.data()), "", "Location destination is not free");
		m_oEntities.push_back(std::move(oEntity));
	}

	void World::removeAllEntities() noexcept
	{
		for (QSharedPointer<Entity>& entity : m_oEntities)
		{
			entity.reset();
		}
		m_oEntities.clear();
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
