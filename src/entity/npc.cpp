/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "npc.h"
#include "util/collision_helper.h"
#include "util/algorithm_util.h"
#include "level/vector_loc.h"
#include "level/world.h"

namespace FastSimDesign {
	Npc::Npc(QWeakPointer<World> oWorld, Tiled::MapObject* const pSpriteEntity, QObject* pParent /*= 0*/) noexcept
		: Parent{std::move(oWorld), pSpriteEntity, pParent}
		, m_dWalkSpeed{128.0}
		, m_dPixWalkedSinceLastDisplay{0.0}
		, m_oTargetLocation{getLocation()}
	{
	}

	void Npc::update(QTime const& oDeltaTime) noexcept
	{
		Location currentloc = getLocation();
		Location targetLoc = getTargetLocation();
		m_oCurrentActionStatus = doWalkingMovement(oDeltaTime, currentloc, targetLoc);
		currentloc = getLocation();
		Location targetLookAt = getTargetLookAt();
		if (m_oCurrentActionStatus == Entity::ActionFeedback::SUCCESS)
			m_oCurrentActionStatus = doTurningMovement(oDeltaTime, currentloc, targetLookAt);
		if (m_oCurrentActionStatus == Entity::ActionFeedback::SUCCESS)
			m_oCurrentActionStatus = Entity::ActionFeedback::INACTIVE;
	}

	bool Npc::isCollideWith(Entity const& oOther) const noexcept
	{
		Q_ASSERT_X(oOther.getId() != getId(), "", "The collision test has to concern two different entities");
		return CollisionHelper::areColliding(getBoundingBox(), oOther.getBoundingBox());
	}

	QVector<Tiled::MapObject const*> Npc::getAllOwnedSprites() const noexcept
	{
		QVector<Tiled::MapObject const*> allSprite{};
		allSprite.push_back(m_pSpriteEntity);
		return allSprite;
	}

	void Npc::setTargetLocation(Location oTargetLoc) noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oTargetLoc), "", "This location does not exists");
		m_oTargetLocation = std::move(oTargetLoc);
	}

	Entity::ActionFeedback Npc::doWalkingMovement(QTime const& oDeltaTime, Location const& oFrom, Location const& oTo) noexcept
	{
		if (oFrom.toCoordPoint() != oTo.toCoordPoint())
		{
			double walkDistance = Time::toSeconds(oDeltaTime) * m_dWalkSpeed;
			VectorLoc pathToTravel = oFrom.relativeVectorTo(oTo);
			if (pathToTravel.lengthSquared() < walkDistance * walkDistance)
			{
				lookAt(oTo);
				setLocation(oTo);
				return Entity::ActionFeedback::SUCCESS;
			}

			pathToTravel.normalize();
			VectorLoc distanceOffset = pathToTravel * walkDistance;
			Location newPos = oFrom + distanceOffset.toLocation();
			newPos.setDirection(oTo);
			// --- Check for map obstacle and check for entity in the way ---
			if (!m_oWorld.lock()->isWalkableTileDestination(*this, distanceOffset))
				return Entity::ActionFeedback::NOT_WALKABLE;
			if (m_oWorld.lock()->isCollide(*this))
				return Entity::ActionFeedback::IS_COLLIDE;
			// --- Move the entity ---
			if (newPos.toCoordPoint() != oFrom.toCoordPoint())
			{
				lookAt(newPos);
				setLocation(newPos);
				return Entity::ActionFeedback::IN_PROGRESS;
			} else
			{
				lookAt(newPos);
			}
		}
		return Entity::ActionFeedback::SUCCESS;
	}

	Entity::ActionFeedback Npc::doTurningMovement(QTime const& oDeltaTime, Location const& oFrom, Location const& oTo) noexcept
	{
		if (oFrom.toCoordPoint() != oTo.toCoordPoint())
		{
			lookAt(oTo);
		}
		return Entity::ActionFeedback::SUCCESS;
	}
}
