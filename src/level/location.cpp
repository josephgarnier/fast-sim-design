/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "location.h"
#include "util/math_util.h"

namespace FastSimDesign {

	Location::Location(QWeakPointer<World const> oWorld, VectorLoc const& oVectorCoord) noexcept
		: Parent{oVectorCoord.getCoordX(), oVectorCoord.getCoordY()}
		, m_oWorld{std::move(oWorld)}
		, m_dYaw{oVectorCoord.angle()}
	{
	}

	Location::Location(QWeakPointer<World const> oWorld, double dXCoord, double dYCoord, double dYawDegrees /*= 0.0*/) noexcept
		: Parent{dXCoord, dYCoord}
		, m_oWorld{std::move(oWorld)}
		, m_dYaw{fmod(360.0 + (fmod(dYawDegrees, 360.0)), 360.0)}
	{
	}

	Location::Location(QWeakPointer<World const> oWorld, int dXTile, int dYTile, double dYawDegrees /*= 0.0*/) noexcept
		: Parent{dXTile * oWorld.lock()->getTileWidth() + (signNotNull(dXTile) * oWorld.lock()->getTileWidth() / 2), dYTile * oWorld.lock()->getTileHeight() + (signNotNull(dYTile) * oWorld.lock()->getTileHeight() / 2)}
		, m_oWorld{std::move(oWorld)}
		, m_dYaw{fmod(360.0 + (fmod(dYawDegrees, 360.0)), 360.0)}
	{
	}

	bool Location::isValid() const noexcept
	{
		return Parent::x() >= 0 && Parent::x() < m_oWorld.lock()->getWidth() && Parent::y() >= 0 && Parent::y() < m_oWorld.lock()->getHeight();
	}

	int Location::getTileX() const noexcept
	{
		return Parent::x() / m_oWorld.lock()->getTileWidth();
	}

	int Location::getTileY() const noexcept
	{
		return Parent::y() / m_oWorld.lock()->getTileHeight();
	}

	VectorLoc Location::getDirectionNormalized() const noexcept
	{
		double yawRadian = qDegreesToRadians(m_dYaw);
		double x = qCos(yawRadian);
		double y = qSin(yawRadian);
		if (qAbs(1.0 - x) < 0.0001) x = 1.0;
		if (qAbs(1.0 - y) < 0.0001) y = 1.0;
		if (qAbs(0.0 - x) < 0.0001) x = 0.0;
		if (qAbs(0.0 - y) < 0.0001) y = 0.0;
		if (qAbs(-1.0 - x) < 0.0001) x = -1.0;
		if (qAbs(-1.0 - y) < 0.0001) y = -1.0;
		return VectorLoc{m_oWorld, x, y};
	}

	void Location::setDirection(Location const& oTarget) noexcept
	{
		Location diffLoc = oTarget - *this;
		double x = diffLoc.getCoordX();
		double y = diffLoc.getCoordY();
		double theta = qAtan2(y, x);
		m_dYaw = qRadiansToDegrees(std::fmod((theta + M_2PI), M_2PI));
	}

	double Location::length() const noexcept
	{
		return qSqrt((Parent::x() * Parent::x()) + (Parent::y() * Parent::y()));
	}

	double Location::lengthSquared() const noexcept
	{
		return (Parent::x() * Parent::x()) + (Parent::y() * Parent::y());
	}

	double Location::euclideanDistanceTo(Location const& oTarget) const noexcept
	{
		double diffX = oTarget.getCoordX() - Parent::x();
		double diffY = oTarget.getCoordY() - Parent::y();
		return qSqrt((diffX * diffX) + (diffY * diffY));
	}

	int Location::manhattanDistanceTo(Location const& oTarget) const noexcept
	{
		int diffX = oTarget.getTileX() - getTileX();
		int diffY = oTarget.getTileY() - getTileY();
		return qAbs(diffX) + qAbs(diffY);
	}

	VectorLoc Location::relativeVectorTo(Location const& oTarget) const noexcept
	{
		Location diffLoc = oTarget - *this;
		return VectorLoc{m_oWorld, diffLoc};
	}
}