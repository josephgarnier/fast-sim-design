/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "path_finder.h"

namespace FastSimDesign {
	PathFinder::PathFinder(QWeakPointer<World const> oWorld) noexcept
		: m_oWorld{std::move(oWorld)}
		, m_oOpenNode{}
		, m_oClosedNode{}
		, m_oStartPoint{}
		, m_oEndPoint{}
	{
	}

	Path PathFinder::createEntityPathToPoint(Entity const & oFrom, Entity const & oTo) noexcept
	{
		Q_ASSERT_X(oFrom.getBoundingBoxTile().area() <= 1, "", "Path finding works only with bounding box with an area lower than 1");
		return aStar(oFrom.getLocation(), oTo.getLocation());
	}

	Path PathFinder::createEntityPathToPoint(Entity const & oFrom, Location const & oTo) noexcept
	{
		Q_ASSERT_X(oFrom.getBoundingBoxTile().area() <= 1, "", "Path finding works only with bounding box with an area lower than 1");
		return aStar(oFrom.getLocation(), oTo);
	}

	Path PathFinder::aStar(Location const & oStartTile, Location const & oEndTile) noexcept
	{
		Q_ASSERT_X(m_oWorld.lock()->isCoordExists(oStartTile) && m_oWorld.lock()->isCoordExists(oEndTile), "", "These coordinates do not exist");
		m_oStartPoint = QSharedPointer<PathPoint>::create(oStartTile.getTileX(), oStartTile.getTileY());
		m_oEndPoint = QSharedPointer<PathPoint>::create(oEndTile.getTileX(), oEndTile.getTileY());
		Path resultPath{};
		if (!m_oWorld.lock()->isWalkableTileLocation(oStartTile) || !m_oWorld.lock()->isWalkableTileLocation(oEndTile))
		{
			qWarning() << "No path can be generated, starting point or ending point is not walkable.";
			return resultPath;
		}
		if (*m_oStartPoint.data() == *m_oEndPoint.data())
		{
			qWarning() << "No path will be generated, starting tile point and ending tile point are the same.";
			return resultPath;
		}
		m_oClosedNode.clear(); // close: nodes visited, belonging to solution
		m_oOpenNode.clear(); // open: nodes to explore
		m_oOpenNode.enqueue(m_oStartPoint, less_shared_value<PathPoint>());
		while (!isOver())
		{
			// --- Get the lowest F cost on open list ---
			QSharedPointer<PathPoint> current = getLowestOpenPoint();
			// --- Add current to the closed list ---
			if(contains_if_shared_value(m_oClosedNode.begin(), m_oClosedNode.end(), current))
				Q_ASSERT_X(false, "", "This Point ever exists in closed list");
			m_oClosedNode.enqueue(current);
			// --- Process all adjacent points ---
			QQueue<QSharedPointer<PathPoint>> neighbours = getAdjacentPoints(current);
			for (QSharedPointer<PathPoint> neighbour : neighbours)
			{
				processAdjacentPoint(std::move(neighbour), current);
			}
		}
		if (contains_if_shared_value(m_oClosedNode.begin(), m_oClosedNode.end(), m_oEndPoint))
			resultPath = buildPath(find_if_shared_value(m_oClosedNode.begin(), m_oClosedNode.end(), m_oEndPoint));
		else
			qWarning() << "The ending point cannot be reached.";
		
		return resultPath;
	}

	bool PathFinder::isOver() const noexcept
	{
		if (m_oOpenNode.isEmpty() || contains_if_shared_value(m_oClosedNode.begin(), m_oClosedNode.end(), m_oEndPoint))
			return true;
		return false;
	}

	QSharedPointer<PathPoint> PathFinder::getLowestOpenPoint() noexcept
	{
		if (!m_oOpenNode.isEmpty())
		{
			return m_oOpenNode.dequeue();
		}
		Q_ASSERT_X(false, "", "No point exists");
		return QSharedPointer<PathPoint>{};
	}

	QQueue<QSharedPointer<PathPoint>> PathFinder::getAdjacentPoints(QSharedPointer<PathPoint const> oPoint) const noexcept
	{
		QQueue<QSharedPointer<PathPoint>> neighbours{};
		if (m_oWorld.lock()->isTileExists(oPoint->x(), oPoint->y() - 1)) neighbours.enqueue(QSharedPointer<PathPoint>::create(oPoint->x(), oPoint->y() - 1, oPoint)); // Top
		if (m_oWorld.lock()->isTileExists(oPoint->x() + 1, oPoint->y())) neighbours.enqueue(QSharedPointer<PathPoint>::create(oPoint->x() + 1, oPoint->y(), oPoint)); // Right
		if (m_oWorld.lock()->isTileExists(oPoint->x(), oPoint->y() + 1)) neighbours.enqueue(QSharedPointer<PathPoint>::create(oPoint->x(), oPoint->y() + 1, oPoint)); // Bottom
		if (m_oWorld.lock()->isTileExists(oPoint->x() - 1, oPoint->y())) neighbours.enqueue(QSharedPointer<PathPoint>::create(oPoint->x() - 1, oPoint->y(), oPoint)); // Left
		return neighbours;
	}

	void PathFinder::processAdjacentPoint(QSharedPointer<PathPoint> oNeighbour, QSharedPointer<PathPoint const> oParent) noexcept
	{
		Q_ASSERT_X(*oNeighbour.data() != *oParent.data(), "", "Parent point and neighbour point cannot be the same");
		Location neighbourLoc{m_oWorld, oNeighbour->x(), oNeighbour->y()};
		if (!m_oWorld.lock()->isWalkableTileLocation(neighbourLoc) || contains_if_shared_value(m_oClosedNode.begin(), m_oClosedNode.end(), oNeighbour))
		{
			return;
		}
		// --- Calculate F for a neighbour ---
		if (contains_if_shared_value(m_oOpenNode.begin(), m_oOpenNode.end(), oNeighbour))
		{
			QSharedPointer<PathPoint>& existingNeighbour = find_if_shared_value(m_oOpenNode.begin(), m_oOpenNode.end(), oNeighbour);
			if(existingNeighbour->updateFIfBetter(*m_oStartPoint.data(), *m_oEndPoint.data()))
				existingNeighbour->setParent(std::move(oParent));
		} else
		{
			oNeighbour->calculateF(*m_oStartPoint.data(), *m_oEndPoint.data());
			oNeighbour->setParent(std::move(oParent));
			m_oOpenNode.enqueue(std::move(oNeighbour), less_shared_value<PathPoint>());
		}
	}

	Path PathFinder::buildPath(QSharedPointer<PathPoint const> oFromPoint) const noexcept
	{
		Path resultPath{};
		QSharedPointer<PathPoint const> currentPoint{std::move(oFromPoint)};
		resultPath.emplace(currentPoint->x(), currentPoint->y());
		while (!currentPoint->getParent().isNull())
		{
			currentPoint = currentPoint->getParent();
			resultPath.emplace(currentPoint->x(), currentPoint->y());
		}
		return resultPath;
	}
}