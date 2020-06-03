/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "vector_loc.h"
#include "QtMath"
#include "location.h"
#include "world.h"

namespace FastSimDesign {

	VectorLoc::VectorLoc(QWeakPointer<World const> oWorld, double dXCoord, double dYCoord) noexcept
		: Parent{static_cast<float>(dXCoord), static_cast<float>(dYCoord)}
		, m_oWorld{std::move(oWorld)}
	{
	}

	VectorLoc::VectorLoc(QWeakPointer<World const> oWorld, QVector2D const& oVect) noexcept
		: Parent{oVect.x(), oVect.y()}
		, m_oWorld{std::move(oWorld)}
	{
	}

	VectorLoc::VectorLoc(QWeakPointer<World const> oWorld, Location const& oLoc) noexcept
		: Parent{static_cast<float>(oLoc.getCoordX()), static_cast<float>(oLoc.getCoordY())}
		, m_oWorld{std::move(oWorld)}
	{
	}

	bool VectorLoc::isValid() const noexcept
	{
		return Parent::x() >= 0 && Parent::x() < m_oWorld.lock()->getWidth() && Parent::y() >= 0 && Parent::y() < m_oWorld.lock()->getHeight();
	}

	Location VectorLoc::toLocation() const noexcept
	{
		return Location{m_oWorld, Parent::x(), Parent::y(), angle()};
	}

	double VectorLoc::length() const noexcept
	{
		return qSqrt((Parent::x() * Parent::x()) + (Parent::y() * Parent::y()));
	}

	double VectorLoc::lengthSquared() const noexcept
	{
		return (Parent::x() * Parent::x()) + (Parent::y() * Parent::y());
	}

	double VectorLoc::angle() const noexcept
	{
		double doublePI = M_PI * 2.0;
		double x = Parent::x();
		double y = Parent::y();
		double theta = qAtan2(y, x);
		double thetaDeg = qRadiansToDegrees(std::fmod((theta + doublePI), doublePI));
		return thetaDeg;
	}

	double VectorLoc::angleTo(VectorLoc const& oVector) const noexcept
	{
		if (isNull() || oVector.isNull())
			return 0.0;

		double a1 = angle();
		double a2 = oVector.angle();
		double delta = a2 - a1;
		double deltaNormalized = delta < 0 ? delta + 360.0 : delta;
		if (qFuzzyCompare(delta, 360.0))
			return 0.0;
		else
			return deltaNormalized;
	}

	double VectorLoc::dotProduct(VectorLoc const& oVector) const noexcept
	{
		return Parent::x() * oVector.getCoordX() + Parent::y() * oVector.getCoordY();
	}

	double VectorLoc::crossProduct(VectorLoc const& oVector) const noexcept
	{
		return Parent::x() * oVector.getCoordY() - Parent::y() * oVector.getCoordX();
	}

	void VectorLoc::normalize() noexcept
	{
		Parent::normalize();
	}

	VectorLoc VectorLoc::unitVector() const noexcept
	{
		QVector2D vec = Parent::normalized();
		return VectorLoc{m_oWorld, vec.x(), vec.y()};
	}

	void VectorLoc::setDirection(Location const& oTarget) noexcept
	{
		VectorLoc oUnitTargetVec = oTarget.toVector().unitVector();
		double tarX = oUnitTargetVec.getCoordX();
		double tarY = oUnitTargetVec.getCoordY();
		double l = length();
		double dx = tarX * l;
		double dy = tarY * l;
		setX(dx);
		setY(dy);
	}
}
