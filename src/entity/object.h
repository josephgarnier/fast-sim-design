/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_OBJECT_H
#define FAST_SIM_DESIGN_OBJECT_H

#include "slot.h"
#include "entity.h"

namespace FastSimDesign {
	class Object final : public Entity
	{
		Q_OBJECT
	public:
		explicit Object(QWeakPointer<World> oWorld, Tiled::MapObject* const pSpriteEntity, QObject* pParent = Q_NULLPTR) noexcept;
		virtual ~Object() = default; // Destructor

		virtual void update(QTime const& oDeltaTime) noexcept override;

		virtual bool isCollideWith(Entity const& oOther) const noexcept override;
		virtual QVector<Tiled::MapObject const*> getAllOwnedSprites() const noexcept override;

		void createSlot(Tiled::MapObject* const pArea) noexcept;
		bool hasAvailableFreeSlot() const noexcept;
		Slot const& getFreeSlot() const noexcept;
		QHash<int, Slot> getAllFreeSlots() const noexcept;
		QHash<int, Slot> getAllHeldSlots() const noexcept;
		void holdSlot(int oFreeSlotId, QWeakPointer<Entity const> oHolder) noexcept;
		void releaseSlot(int oHoldSlotId, Entity const& oCurrentSlotHolder) noexcept;
		inline QHash<int, Slot> const& getAllSlots() const noexcept { return m_oSlots; }

	protected:
		QHash<int, Slot> m_oSlots;

	private:
	};
}
#endif
