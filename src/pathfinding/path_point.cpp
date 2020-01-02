/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "path_point.h"

namespace FastSimDesign {
	PathPoint::PathPoint() noexcept
		: Parent{0, 0}
		, m_oParent{}
		, m_iH{}
		, m_iG{}
	{
	}

	PathPoint::PathPoint(int iX, int iY) noexcept
		: Parent(iX, iY)
		, m_oParent{}
		, m_iH{}
		, m_iG{}
	{
	}

	PathPoint::PathPoint(int iX, int iY, QSharedPointer<PathPoint const> oParent) noexcept
		: Parent(iX, iY)
		, m_oParent{std::move(oParent)}
		, m_iH{}
		, m_iG{}
	{
		Q_ASSERT_X(*this != *m_oParent.data(), "", "Parent and this current point cannot be the same");
	}

	PathPoint::PathPoint(PathPoint const& oCopy) noexcept
		: Parent{oCopy.x(), oCopy.y()}
		, m_iH{oCopy.m_iH}
		, m_iG{oCopy.m_iG}
	{
		if (!oCopy.m_oParent.isNull())
			m_oParent = QSharedPointer<PathPoint>::create(*oCopy.m_oParent.data());
	}

	PathPoint::PathPoint(PathPoint&& oCopy) noexcept
		: Parent{std::move(oCopy.x()), std::move(oCopy.y())}
		, m_iH{std::move(oCopy.m_iH)}
		, m_iG{std::move(oCopy.m_iG)}
	{
		if (!oCopy.m_oParent.isNull())
			m_oParent = std::move(oCopy.m_oParent);
	}

	PathPoint& PathPoint::operator=(PathPoint oCopy) noexcept
	{
		swap(*this, oCopy);
		return *this;
	}

	void PathPoint::calculateF(PathPoint const& oStartPoint, PathPoint const& oEndPoint) noexcept
	{
		m_iG = calculateG(oStartPoint);
		m_iH = calculateH(oEndPoint);
	}

	int PathPoint::calculateH(PathPoint const& oEndPoint) const noexcept
	{
		return manhattanDistanceTo(oEndPoint);
	}

	int PathPoint::calculateG(PathPoint const& oStartPoint) const noexcept
	{
		return manhattanDistanceTo(oStartPoint);
	}

	int PathPoint::manhattanDistanceTo(PathPoint const& oTarget) const noexcept
	{
		QPoint point = *this - oTarget;
		return point.manhattanLength();
	}

	bool PathPoint::updateFIfBetter(PathPoint const& oStartPoint, PathPoint const& oEndPoint) noexcept
	{
		int oldG = m_iG;
		int oldH = m_iH;
		int oldF = getF();
		calculateF(oStartPoint, oEndPoint);
		if (oldF < getF()) // if the old F value was better, we restore the old data.
		{
			m_iG = oldG;
			m_iH = oldH;
			return false;
		}
		return true;
	}
}