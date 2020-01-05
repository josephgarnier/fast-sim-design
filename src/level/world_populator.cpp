/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "world_populator.h"
#include "objectgroup.h"
#include "entity/npc.h"
#include "entity/object.h"
#include "world_exception.h"

namespace FastSimDesign {
	WorldPopulator::WorldPopulator(QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> oTiledObjects) noexcept
		: m_oWorld{}
		, m_oTiledObjects{std::move(oTiledObjects)}
		, m_bIsWorldPopulated{false}
	{
		Q_ASSERT_X(!m_oTiledObjects.empty(), "", "No tiled object to add in the world");
	}

	void WorldPopulator::populateWorld(QSharedPointer<World> oWorld)
	{
		try
		{
			Q_ASSERT_X(!oWorld.isNull(), "", "The world si null");
			if (m_bIsWorldPopulated == false)
			{
				m_oWorld = std::move(oWorld);
				createNpcs();
				createObjects();
				m_oWorld.reset();
				m_bIsWorldPopulated = true;
			}
		} catch (...)
		{
			m_bIsWorldPopulated = false;
			oWorld->removeAllEntities();
			throw;
		}
	}

	void WorldPopulator::createNpcs()
	{
		QList<Tiled::MapObject*> npcSprites = m_oTiledObjects.values(MapResource::TiledObjectType::NPC);
		for (Tiled::MapObject* npcSprite : npcSprites)
		{
			int id = npcSprite->id();
			int walkSpeed = npcSprite->property(QStringLiteral("walk_speed")).toInt();
			QSharedPointer<Npc> newNpc = QSharedPointer<Npc>::create(m_oWorld, npcSprite, m_oWorld.data());
			newNpc->setWalkSpeed(walkSpeed);
			m_oWorld->addEntity(newNpc);
			qInfo().nospace() << "Npc \"" << newNpc->GetId() << "\" added.";
		}
	}

	void WorldPopulator::createObjects()
	{
		QList<Tiled::MapObject*> objectSprites = m_oTiledObjects.values(MapResource::TiledObjectType::OBJECT);
		QList<Tiled::MapObject*> slotSprites = m_oTiledObjects.values(MapResource::TiledObjectType::SLOT);
		for (Tiled::MapObject* objectSprite : objectSprites)
		{
			int id = objectSprite->id();
			QSharedPointer<Object> newObject = QSharedPointer<Object>::create(m_oWorld, objectSprite, m_oWorld.data());
			m_oWorld->addEntity(newObject);
			qInfo().nospace() << "Object \"" << newObject->getId() << "\" added.";
		}

		for (Tiled::MapObject* slotSprite : slotSprites)
		{
			int relatedObject = slotSprite->property(QStringLiteral("related_object_id")).toInt();
			QSharedPointer<Object> entity = m_oWorld->GetEntityById<Object>(relatedObject);
			entity->createSlot(slotSprite);
		}
	}
}