/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "object.h"

namespace FastSimDesign {
	Object::Object(QWeakPointer<World> oWorld, Tiled::MapObject* const pSpriteEntity, QObject* pParent /*= Q_NULLPTR*/) noexcept
		: Entity{std::move(oWorld), pSpriteEntity, pParent}
		, m_oSlots{}
	{
	}
	
	void Object::update(QTime const& oDeltaTime) noexcept
	{
	}

	bool Object::isCollideWith(Entity const& oOther) const noexcept
	{
		Q_ASSERT_X(oOther.getId() != getId(), "", "The collision test has to concern two different entities");
		if (CollisionHelper::areColliding(getBoundingBox(), oOther.getBoundingBox()))
		{
			for (auto& slot : m_oSlots) // Collisions with a slot are allowed, the entity can go on.
			{
				if (CollisionHelper::areColliding(slot.getAreaBoundingBox(), oOther.getBoundingBox()))
					return false;
			}
			return true;
		} else
		{
			return false;
		}
	}

	QVector<Tiled::MapObject const*> Object::getAllOwnedSprites() const noexcept
	{
		QVector<Tiled::MapObject const*> allSprite{};
		allSprite.push_back(m_pSpriteEntity);
		for (Slot& slot : m_oSlots.values())
		{
			allSprite.push_back(slot.getArea());
		}
		return allSprite;
	}

	void Object::createSlot(Tiled::MapObject* const pArea) noexcept
	{
		if (!m_oSlots.contains(pArea->id()))
		{
			Slot newSlot{m_oWorld, pArea};
			Q_ASSERT_X(m_oWorld.lock()->isWalkableTileLocation(newSlot.getLocation()), "", "Cannot add the slot. Location destination is not walkable");
			m_oSlots.insert(pArea->id(), std::move(newSlot));
		}
	}

	bool Object::hasAvailableFreeSlot() const noexcept
	{
		for (auto& slot : m_oSlots)
		{
			if (slot.isFree()) return true;
		}
		return false;
	}

	Slot const& Object::getFreeSlot() const noexcept
	{
		for (auto& slot : m_oSlots)
		{
			if (slot.isFree()) return slot;
		}
		Q_ASSERT_X(false, "", "You are trying to get a no free slot");
		return m_oSlots.values().first();
	}

	QHash<int, Slot> Object::getAllFreeSlots() const noexcept
	{
		QHash<int, Slot> freeSlots;
		for (auto& slot : m_oSlots)
		{
			if (slot.isFree()) freeSlots.insert(slot.getId(), slot);
		}
		return freeSlots;
	}

	QHash<int, Slot> Object::getAllHeldSlots() const noexcept
	{
		QHash<int, Slot> heldSlots;
		for (auto& slot : m_oSlots)
		{
			if (!slot.isFree()) heldSlots.insert(slot.getId(), slot);
		}
		return heldSlots;
	}

	void Object::holdSlot(int oFreeSlotId, QWeakPointer<Entity const> oHolder) noexcept
	{
		Q_ASSERT_X(m_oSlots.contains(oFreeSlotId), "", "No slot exists with this id");
		Slot& slot = m_oSlots[oFreeSlotId];
		Q_ASSERT_X(slot.isFree(), "", "You cannot hold this slot, it is not free");
		Q_ASSERT_X(slot.getLocation().toCoordPoint() == oHolder.lock()->getLocation().toCoordPoint(), "", "The future holder have to be at the same location as the slot");
		slot.hold(std::move(oHolder));
	}

	void Object::releaseSlot(int oHoldSlotId, Entity const& oCurrentSlotHolder) noexcept
	{
		Q_ASSERT_X(m_oSlots.contains(oHoldSlotId), "", "No slot exists with this id");
		Slot& slot = m_oSlots[oHoldSlotId];
		Q_ASSERT_X(!slot.isFree(), "", "You cannot free this slot, it is not held");
		Q_ASSERT_X(slot.getHolder()->getId() == oCurrentSlotHolder.getId(), "", "You try to release this slot with an entity not being the holder");
		Q_ASSERT_X(slot.getLocation().toCoordPoint() == oCurrentSlotHolder.getLocation().toCoordPoint(), "", "The holder have to be at the same location as the slot");
		slot.release();
	}
}