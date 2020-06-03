/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_ENTITY_H
#define FAST_SIM_DESIGN_ENTITY_H

#include "QObject"
#include "tiled/mapobject.h"
#include "util/axis_aligned_bb.h"
#include "level/location.h"
#include "pathfinding/path.h"

namespace FastSimDesign {
	class Word;
	class Entity : public QObject
	{
		Q_OBJECT
	public:
		/*!
		* Offset alignment according to center position reference. Offset is rotation sensitive.
		*/
		static QPointF spriteAlignmentOffset(Tiled::MapObject const* const pSpriteEntity, Tiled::Alignment oAlignment) noexcept;
		static QPointF spriteCenterPosition(Tiled::MapObject const* const pSpriteEntity, Tiled::Alignment oAlignment) noexcept;

	public:
		enum class EntityDirection
		{
			RIGHT = 0,
			DOWN = 90,
			LEFT = 180,
			UP = 270
		};

		enum class ActionFeedback
		{
			SUCCESS, // entity applied successfully the AI engine decision.
			FAILURE, // entity fail to apply the decision coming from the AI engine.
			IN_PROGRESS, // entity is doing something. Muscle are activate.
			INACTIVE, // entity is inactive because all decision coming AI engine has been applied. Muscles are inactive.
			IS_COLLIDE,
			NOT_WALKABLE
		};

		explicit Entity(QWeakPointer<World> oWorld, Tiled::MapObject* const pSpriteEntity, QObject* pParent = Q_NULLPTR) noexcept;
		virtual ~Entity() = default; // Destructor

		inline int getId() const noexcept { return m_pSpriteEntity->id(); }
		inline QString const& getName() const noexcept { return m_pSpriteEntity->name(); }
		inline QSharedPointer<World const> getWorld() const noexcept { return m_oWorld.lock(); }
		inline void setEnableModelUpdating(bool bEnable) noexcept { m_bEnableModelUpdating = bEnable; };

		void init() noexcept;
		virtual void onInit() noexcept {};

		virtual void update(QTime const& oDeltaTime) noexcept = 0;

		void term() noexcept;
		virtual void onTerm() noexcept {};

		virtual bool isCollideWith(Entity const& oOther) const noexcept = 0;
		inline Entity::ActionFeedback getActionFeedback() const noexcept { return m_oCurrentActionStatus; }

		/*!
		* Other can be seen if one of them corners (the corners are compute from the greatest side of its bounding box) is in the perception field.
		* The perception field is an half-circle with a base orthoganl with the direction of this entity. The angle of perception is 180�.
		* The perception radius has to be higher at least equals to its bounding box radius.
		*/
		bool canSeeInFront(Entity const& oOther, double dRadius) const noexcept;
		Path findPathTo(Entity const& oToEntity) const noexcept;
		Path findPathTo(Location const& oToLocation) const noexcept;

		virtual QVector<Tiled::MapObject const*> getAllOwnedSprites() const noexcept = 0;
		bool isOwnSprite(Tiled::MapObject const& oSprite) const noexcept;

		AxisAlignedBB getBoundingBox() const noexcept;
		AxisAlignedBB getBoundingBoxTile() const noexcept;

		Location getLocation() const noexcept;
		Location getClosestAccessibleLocation(int iMaxTileDistance = 1) const noexcept;
		Entity::EntityDirection getDirection() const noexcept;
		VectorLoc getDirectionNormalized() const noexcept;
		bool isDirectedTo(Location const& oToLoc) const noexcept;
		Location getTargetLookAt() const noexcept { return m_oTargetLooktAt; }

		void setDirection(Entity::EntityDirection oDirection) noexcept;
		void setLocation(Location const& oCenter) noexcept; // Center position
		void lookAt(Location const& oLoc) noexcept;
		void setTargetLookAt(Location oTargetLoc) noexcept;

	protected:
		QWeakPointer<World> m_oWorld;
		Tiled::MapObject* const m_pSpriteEntity;
		Location m_oTargetLooktAt;
		Entity::ActionFeedback m_oCurrentActionStatus;

		bool m_bEnableModelUpdating;

	private:
	};
}
#endif
