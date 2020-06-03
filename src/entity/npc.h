/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_NPC_H
#define FAST_SIM_DESIGN_NPC_H

#include "entity.h"

namespace FastSimDesign {
	class Npc final : public Entity
	{
		Q_OBJECT
	private:
		using Parent = Entity;

	public:
		explicit Npc(QWeakPointer<World> oWorld, Tiled::MapObject* const pSpriteEntity, QObject* pParent = Q_NULLPTR) noexcept;
		virtual ~Npc() = default; // Destructor

		virtual void update(QTime const& oDeltaTime) noexcept override;

		virtual bool isCollideWith(Entity const& oOther) const noexcept override;
		virtual QVector<Tiled::MapObject const*> getAllOwnedSprites() const noexcept override;

		inline double getWalkSpeed() const noexcept { return m_dWalkSpeed; }
		inline void setWalkSpeed(double dSpeed) noexcept { m_dWalkSpeed = dSpeed; }

		inline Location getTargetLocation() const noexcept { return m_oTargetLocation; }
		void setTargetLocation(Location oTargetLoc) noexcept;

	protected:
		Entity::ActionFeedback doWalkingMovement(QTime const& oDeltaTime, Location const& oFrom, Location const& oTo) noexcept;
		Entity::ActionFeedback doTurningMovement(QTime const& oDeltaTime, Location const& oFrom, Location const& oTo) noexcept;

	protected:
		double m_dWalkSpeed;
		double m_dPixWalkedSinceLastDisplay;

		Location m_oTargetLocation;

	private:
	};
}
#endif
