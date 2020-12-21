/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world_populator.h"
#include "entity/npc.h"
#include "entity/object.h"
#include "world.h"

namespace FastSimDesign {
	WorldPopulator::WorldPopulator(QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> oTiledObjects) noexcept
		: m_oWorld{}
		, m_oTiledObjects{std::move(oTiledObjects)}
		, m_bIsWorldPopulated{false}
	{
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
			oWorld->destroyAllEntities();
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
			Entity::Id newEntity = m_oWorld->createEntity<Npc>(npcSprite, m_oWorld.data());
			m_oWorld->getEntity<Npc>(newEntity).setWalkSpeed(walkSpeed);
			qInfo().nospace() << "Npc \"" << newEntity.m_id << "\" added.";
		}
	}

	void WorldPopulator::createObjects()
	{
		QList<Tiled::MapObject*> objectSprites = m_oTiledObjects.values(MapResource::TiledObjectType::OBJECT);
		QList<Tiled::MapObject*> slotSprites = m_oTiledObjects.values(MapResource::TiledObjectType::SLOT);
		for (Tiled::MapObject* objectSprite : objectSprites)
		{
			int id = objectSprite->id();
			Entity::Id newEntity = m_oWorld->createEntity<Object>(objectSprite, m_oWorld.data());
			qInfo().nospace() << "Object \"" << newEntity.m_id << "\" added.";
		}

		for (Tiled::MapObject* slotSprite : slotSprites)
		{
			int relatedObject = slotSprite->property(QStringLiteral("related_object_id")).toInt();
			// QSharedPointer<Object> entity = m_oWorld->getEntity<Object>(relatedObject);
			// entity->createSlot(slotSprite);
		}
	}
}
