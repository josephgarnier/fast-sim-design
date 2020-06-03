/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "collision_helper.h"
#include "axis_aligned_bb.h"
#include "tiled/objectgroup.h"
#include "tiled/map.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	QVector<Tiled::MapObject*> CollisionHelper::getCollidingObjects(AxisAlignedBB const& oBoundingBox, Tiled::ObjectGroup const& oObjects) noexcept
	{
		QVector<Tiled::MapObject*> collidingObjects{};
		for (Tiled::MapObject* other : oObjects.objects())
		{
			AxisAlignedBB otherBoundBox = AxisAlignedBB::getBoundingBox(other);
			if (CollisionHelper::areColliding(oBoundingBox, otherBoundBox))
				collidingObjects.push_back(other);
		}
		return collidingObjects;
	}

	bool CollisionHelper::areColliding(AxisAlignedBB const& oFirstBoundingBox, AxisAlignedBB const& oSecondBoundingBox) noexcept
	{
		return oFirstBoundingBox.intersects(oSecondBoundingBox);
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	CollisionHelper::CollisionHelper(Tiled::TileLayer const* const pCollisionLayer) noexcept
		: m_pCollisionLayer{pCollisionLayer}
	{
	}

	bool CollisionHelper::isWalkableTileDestination(AxisAlignedBB const& oBoundingBox, QVector2D const& oDistanceOffset) const noexcept
	{
		AxisAlignedBB newBoundingBox = oBoundingBox.translate(oDistanceOffset.toPointF());
		return isWalkableTileLocation(newBoundingBox);
	}

	bool CollisionHelper::isWalkableTileDestination(QPointF const& oPosition, QVector2D const& oDistanceOffset) const noexcept
	{
		Tiled::Map const& map = *m_pCollisionLayer->map();
		int tileWidth = map.tileWidth();
		int tileHeight = map.tileHeight();
		QPointF destination = oPosition + oDistanceOffset.toPointF();
		QPointF tileDestination = QPointF{destination.x() / tileWidth, destination.y() / tileHeight};
		return isWalkableTileLocation(QPoint{qFloor(tileDestination.x()), qFloor(tileDestination.y())});
	}

	bool CollisionHelper::isWalkableTileLocation(AxisAlignedBB const& oBoundingBox) const noexcept
	{
		Tiled::Map const& map = *m_pCollisionLayer->map();
		int tileWidth = map.tileWidth();
		int tileHeight = map.tileHeight();

		AxisAlignedBB boundingBoxTile = oBoundingBox.boundUseTile(tileWidth, tileHeight);
		if (!isWalkableTileSpace(boundingBoxTile.topLeft().toPoint(), boundingBoxTile.bottomRight().toPoint()))
			return false;

		return true;
	}

	bool CollisionHelper::isWalkableTileLocation(QPoint const& oTilePosition) const noexcept
	{
		return m_pCollisionLayer->contains(oTilePosition) && m_pCollisionLayer->cellAt(oTilePosition.x(), oTilePosition.y()).isEmpty();
	}

	bool CollisionHelper::isWalkableTileSpace(QPoint const& oTopLeftTile, QPoint const& oBottomRightTile) const noexcept
	{
		Q_ASSERT_X(oBottomRightTile.x() >= oTopLeftTile.x(), "", "MaxX have to be equals or higher than MinX");
		Q_ASSERT_X(oBottomRightTile.y() >= oTopLeftTile.y(), "", "MaxY have to be equals or higher than MinY");
		for (int x = oTopLeftTile.x(); x <= oBottomRightTile.x(); x++)
			for (int y = oTopLeftTile.y(); y <= oBottomRightTile.y(); y++)
				if (!isWalkableTileLocation(QPoint{x, y}))
					return false;

		return true;
	}
}
