/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "entity.h"

#include "level/world.h"
#include "util/axis_aligned_bb.h"
#include "pathfinding/path.h"

#include "entity_storage.h"

namespace FastSimDesign {
	/*****************************************************************************
	Id::Methods
	*****************************************************************************/
	Entity::Id::Id() noexcept
		: m_id{0}
		, m_index{0}
	{
	}
	
	Entity::Id::Id(Entity::Id::value_type id, Entity::Id::index_type index) noexcept
		: m_id{id}
		, m_index{index}
	{
	}
	
	/*****************************************************************************
	Entity::Statics
	*****************************************************************************/
	QPointF Entity::spriteAlignmentOffset(Tiled::MapObject const* const pSpriteEntity, Tiled::Alignment oAlignment) noexcept
	{
		QPointF offset{};
		switch (oAlignment)
		{
			case Tiled::Alignment::Center: break;
			case Tiled::Alignment::TopLeft: offset = QPointF{-pSpriteEntity->width() / 2, -pSpriteEntity->height() / 2}; break;
			case Tiled::Alignment::Top: offset = QPointF{0.0f, -pSpriteEntity->height() / 2}; break;
			case Tiled::Alignment::TopRight: offset = QPointF{pSpriteEntity->width() / 2, -pSpriteEntity->height() / 2}; break;
			case Tiled::Alignment::Left: offset = QPointF{-pSpriteEntity->width() / 2, 0.0f}; break;
			case Tiled::Alignment::Right: offset = QPointF{pSpriteEntity->width() / 2, 0.0f}; break;
			case Tiled::Alignment::BottomLeft: offset = QPointF{-pSpriteEntity->width() / 2, pSpriteEntity->height() / 2}; break;
			case Tiled::Alignment::Bottom: offset = QPointF{0.0f, pSpriteEntity->height() / 2}; break;
			case Tiled::Alignment::BottomRight: offset = QPointF{pSpriteEntity->width() / 2, pSpriteEntity->height() / 2}; break;
			default: break;
		}
		QTransform matrixOffset{};
		matrixOffset.rotate(pSpriteEntity->rotation());
		return matrixOffset.map(QPointF{offset.x(), offset.y()});
	}

	QPointF Entity::spriteCenterPosition(Tiled::MapObject const* const pSpriteEntity, Tiled::Alignment oAlignment) noexcept
	{
		QPointF offset = Entity::spriteAlignmentOffset(pSpriteEntity, oAlignment);
		QPointF center = pSpriteEntity->position() - offset;
		return QPointF{float(qRound(center.x())), float(qRound(center.y()))};
	}

	/*****************************************************************************
	Entity::Methods
	*****************************************************************************/
	Entity::Entity(QWeakPointer<World> oWorld, EntityStorage const* const entityStorage, Entity::Id oId, Tiled::MapObject* const pSpriteEntity, QObject* pParent /*= 0*/) noexcept
		: QObject{pParent}
		, m_oWorld{std::move(oWorld)}
		, m_entityStorage{entityStorage}
		, m_pSpriteEntity{pSpriteEntity}
		, m_oId{oId}
		, m_oCurrentActionStatus{Entity::ActionFeedback::INACTIVE}
		, m_bEnableModelUpdating{false}
	{
	}

	void Entity::init() noexcept
	{
		onInit();
	}

	void Entity::term() noexcept
	{
		onTerm();
	}
	
	bool Entity::isValid() const noexcept
	{
		return m_entityStorage->isValid(m_oId);
	}

	bool Entity::canSeeInFront(Entity const& oOther, double dRadius) const noexcept
	{
		Q_ASSERT_X(dRadius >= getBoundingBox().radius(), "", "Radius have to be at less equal to the bounding box");
		Q_ASSERT_X(oOther.getId() != getId(), "", "The can see test has to concern two different entities");
		double distanceBetweenCenters = getLocation().euclideanDistanceTo(oOther.getLocation());
		double radii = dRadius + qMax(oOther.getBoundingBox().width() / 2.0, oOther.getBoundingBox().height() / 2.0);
		if (distanceBetweenCenters <= radii)
		{
			VectorLoc vectorToOther = getLocation().relativeVectorTo(oOther.getLocation());
			VectorLoc direction = getDirectionNormalized();
			double diffAngle = 180.0 - abs(abs(direction.angleTo(vectorToOther)) - 180.0);
			if (diffAngle <= 90)
				return true;
		}
		return false;
	}

	Path Entity::findPathTo(Entity const& oToEntity) const noexcept
	{
		return m_oWorld.lock()->findPath(*this, oToEntity);
	}

	Path Entity::findPathTo(Location const& oToLocation) const noexcept
	{
		return m_oWorld.lock()->findPath(*this, oToLocation);
	}

	bool Entity::isOwnSprite(Tiled::MapObject const& oSprite) const noexcept
	{
		QVector<Tiled::MapObject const*> allSprites = getAllOwnedSprites();
		qInfo() << &allSprites[0];
		qInfo() << &oSprite;
		return allSprites.contains(&oSprite);
	}

	AxisAlignedBB Entity::getBoundingBox() const noexcept
	{
		return AxisAlignedBB::getBoundingBox(m_pSpriteEntity);
	}

	AxisAlignedBB Entity::getBoundingBoxTile() const noexcept
	{
		return AxisAlignedBB::getBoundingBox(m_pSpriteEntity).boundUseTile(m_oWorld.lock()->getTileWidth(), m_oWorld.lock()->getTileHeight());
	}

	Location Entity::getLocation() const noexcept
	{
		QPointF center = Entity::spriteCenterPosition(m_pSpriteEntity, m_pSpriteEntity->alignment());
		return Location{m_oWorld, center.x(), center.y(), m_pSpriteEntity->rotation()};
	}

	Location Entity::getClosestAccessibleLocation(int iMaxTileDistance /*= 1*/) const noexcept
	{
		Q_ASSERT_X(iMaxTileDistance > 0, "", "The max tile distance has to be higher than 0");
		// -- use a spiral loop to find a free tile around the entity
		/*
			+-------+
			|       |
			| +---+ |
			| |   | |
			| +-+ | |
			|     | |
			+-----+ |
					|
		  <---------+
		 */
		int xTile = getLocation().getTileX();
		int yTile = getLocation().getTileY();
		uint32_t maxIteration = (iMaxTileDistance * 2) + 1; // worst situation
		int direction = -1; // to left first
		Location accessibleLoc{m_oWorld, xTile, yTile};
		for (std::size_t lengthSide = 1; lengthSide <= maxIteration; lengthSide++)
		{
			for (std::size_t row = 0; row < lengthSide; row++)
			{
				xTile += direction;
				accessibleLoc = Location{m_oWorld, xTile, yTile};
				if (m_oWorld.lock()->isCoordExists(accessibleLoc) && m_oWorld.lock()->isWalkableTileLocation(accessibleLoc))
					return accessibleLoc;
			}
			for (std::size_t height = 0; height < lengthSide; height++)
			{
				yTile += direction;
				accessibleLoc = Location{m_oWorld, xTile, yTile};
				if (m_oWorld.lock()->isCoordExists(accessibleLoc) && m_oWorld.lock()->isWalkableTileLocation(accessibleLoc))
					return accessibleLoc;
			}
			direction = -direction; // direction = direction * -1;
		}
		Q_ASSERT_X(false, "", "No available interaction location at a distance less than 3");
		return accessibleLoc;
	}

	Entity::EntityDirection Entity::getDirection() const noexcept
	{
		if (m_pSpriteEntity->rotation() == static_cast<float>(Entity::EntityDirection::RIGHT)) return Entity::EntityDirection::RIGHT;
		if (m_pSpriteEntity->rotation() == static_cast<float>(Entity::EntityDirection::DOWN)) return Entity::EntityDirection::DOWN;
		if (m_pSpriteEntity->rotation() == static_cast<float>(Entity::EntityDirection::LEFT)) return Entity::EntityDirection::LEFT;
		if (m_pSpriteEntity->rotation() == static_cast<float>(Entity::EntityDirection::UP)) return Entity::EntityDirection::UP;
		Q_ASSERT_X(false, "", "Wrong direction");
		return Entity::EntityDirection::RIGHT;
	}

	VectorLoc Entity::getDirectionNormalized() const noexcept
	{
		return getLocation().getDirectionNormalized();
	}

	bool Entity::isDirectedTo(Location const& oToLoc) const noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oToLoc), "", "This location does not exists");
		if (getLocation() != oToLoc)
		{
			Location diff = oToLoc - getLocation();
			double dx = std::abs(diff.getCoordX());
			double dy = std::abs(diff.getCoordY());
			EntityDirection mustDirectedTo;
			if (dx < dy || (dx - dy) / dx < 0.1)
				mustDirectedTo = diff.getCoordY() < 0 ? EntityDirection::UP : EntityDirection::DOWN;
			else
				mustDirectedTo = diff.getCoordX() < 0 ? EntityDirection::LEFT : EntityDirection::RIGHT;

			return getDirection() == mustDirectedTo;
		}
		return true;
	}

	void Entity::setDirection(Entity::EntityDirection oDirection) noexcept
	{
		Location position{getLocation()};
		position.setYaw(static_cast<float>(oDirection));
		m_pSpriteEntity->setRotation(static_cast<float>(oDirection));
		setLocation(position);
	}

	void Entity::setLocation(Location const& oCenter) noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oCenter), "", "This location does not exists");
		QPointF offset = Entity::spriteAlignmentOffset(m_pSpriteEntity, m_pSpriteEntity->alignment());
		m_pSpriteEntity->setPosition(oCenter.toCoordPoint() + offset);
	}

	void Entity::lookAt(Location const& oLoc) noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oLoc), "", "This location does not exists");
		if (getLocation() != oLoc)
		{
			Location diff = oLoc - getLocation();
			double dx = std::abs(diff.getCoordX());
			double dy = std::abs(diff.getCoordY());

			if (dx < dy || (dx - dy) / dx < 0.1)
				setDirection(diff.getCoordY() < 0 ? EntityDirection::UP : EntityDirection::DOWN);
			else
				setDirection(diff.getCoordX() < 0 ? EntityDirection::LEFT : EntityDirection::RIGHT);
		}
	}

	void Entity::setTargetLookAt(Location oTargetLoc) noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oTargetLoc), "", "This location does not exists");
		m_oTargetLooktAt = std::move(oTargetLoc);
	}
}
