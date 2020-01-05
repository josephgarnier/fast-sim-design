/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_PATH_FINDER_H
#define FAST_SIM_DESIGN_PATH_FINDER_H

#include "path.h"
#include "entity/entity.h"
#include "path_point.h"
#include "util/priority_queue.h"
#include "QQueue"

namespace FastSimDesign {
	class PathFinder final
	{
	public:
		explicit PathFinder() = delete; // Default constructor
		explicit PathFinder(QWeakPointer<World const> oWorld) noexcept;
		PathFinder(PathFinder const&) = delete; // Copy constructor
		PathFinder(PathFinder&&) = default; // Move constructor
		PathFinder& operator=(PathFinder const&) = delete; // Copy assignment operator
		PathFinder& operator=(PathFinder&&) = default; // Move assignment operator
		virtual ~PathFinder() = default; // Destructor

		Path createEntityPathToPoint(Entity const& oFrom, Entity const& oTo) noexcept;
		Path createEntityPathToPoint(Entity const& oFrom, Location const& oTo) noexcept;

	protected:
		/*!
		 * A* woks only with AABB having an area less (or equals) than 1. It is insensitive to rotation.
		 */
		Path aStar(Location const& oStart, Location const& oEnd) noexcept;
		bool isOver() const noexcept;
		QSharedPointer<PathPoint> getLowestOpenPoint() noexcept;
		QQueue<QSharedPointer<PathPoint>> getAdjacentPoints(QSharedPointer<PathPoint const> oPoint) const noexcept;
		void processAdjacentPoint(QSharedPointer<PathPoint> oNeighbour, QSharedPointer<PathPoint const> oParent) noexcept;
		Path buildPath(QSharedPointer<PathPoint const> oFromPoint) const noexcept;

	protected:
		QWeakPointer<World const> m_oWorld;
		PriorityQueue<QSharedPointer<PathPoint>> m_oOpenNode;
		QQueue<QSharedPointer<PathPoint>> m_oClosedNode;
		QSharedPointer<PathPoint> m_oStartPoint;
		QSharedPointer<PathPoint> m_oEndPoint;

	private:
	};
}
#endif
