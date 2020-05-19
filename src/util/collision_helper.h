/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_COLLISION_HELPER_H
#define FAST_SIM_DESIGN_COLLISION_HELPER_H

#include "axis_aligned_bb.h"

namespace FastSimDesign {
	class CollisionHelper final
	{
	public:
		static QVector<Tiled::MapObject*> getCollidingObjects(AxisAlignedBB const& oBoundingBox, Tiled::ObjectGroup const& oObjects) noexcept;
		static bool areColliding(AxisAlignedBB const& oFirstBoundingBox, AxisAlignedBB const& oSecondBoundingBox) noexcept;

	public:
		explicit CollisionHelper() = delete; // Default constructor
		explicit CollisionHelper(Tiled::TileLayer const* const pCollisionLayer) noexcept;
		CollisionHelper(CollisionHelper const&) = delete; // Copy constructor
		CollisionHelper(CollisionHelper&&) = delete; // Move constructor
		CollisionHelper& operator=(CollisionHelper const&) = delete; // Copy assignment operator
		CollisionHelper& operator=(CollisionHelper&&) = delete; // Move assignment operator
		virtual ~CollisionHelper() = default; // Destructor

		bool isWalkableTileDestination(AxisAlignedBB const& oBoundingBox, QVector2D const& oDistanceOffset) const noexcept;
		bool isWalkableTileDestination(QPointF const& oPosition, QVector2D const& oDistanceOffset) const noexcept;
		bool isWalkableTileLocation(AxisAlignedBB const& oBoundingBox) const noexcept;
		bool isWalkableTileLocation(QPoint const& oTilePosition) const noexcept;
		bool isWalkableTileSpace(QPoint const& oTopLeftTile, QPoint const& oBottomRightTile) const noexcept;

	protected:
		Tiled::TileLayer const* const m_pCollisionLayer;

	private:
	};
}
#endif
