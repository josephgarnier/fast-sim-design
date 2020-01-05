/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_LOCATION_H
#define FAST_SIM_DESIGN_LOCATION_H

#include "ostream"
#include "QPointF"
#include "vector_loc.h"
#include "QStringBuilder"

namespace FastSimDesign {
	class World;
	class Location final : private QPointF
	{
	private:
		using Parent = QPointF;

	public:
		explicit Location() = default; // Default constructor
		explicit Location(QWeakPointer<World const> oWorld, VectorLoc const& oVectorCoord) noexcept;
		explicit Location(QWeakPointer<World const> oWorld, double dXCoord, double dYCoord, double dYawDegrees = 0.0) noexcept;
		explicit Location(QWeakPointer<World const> oWorld, int dXTile, int dYTile, double dYawDegrees = 0.0) noexcept; // coordinates are initialized with the center of the tile
		Location(Location const&) = default; // Copy constructor
		Location(Location&&) = default; // Move constructor
		Location& operator=(Location const&) = default; // Copy assignment operator
		Location& operator=(Location&&) = default; // Move assignment operator
		virtual ~Location() = default; // Destructor

		friend inline void swap(Location& oLeft, Location& oRight) noexcept;

		bool isValid() const noexcept;

		inline QSharedPointer<World const> getWorld() const noexcept { return m_oWorld.lock(); }
		inline double getCoordX() const noexcept { return Parent::x(); }
		int getTileX() const noexcept;
		inline double getCoordY() const noexcept { return Parent::y(); }
		int getTileY() const noexcept;
		inline double getYaw() const noexcept { return m_dYaw; }
		VectorLoc getDirectionNormalized() const noexcept;

		inline QPointF toCoordPoint() const noexcept { return QPointF{getCoordX(), getCoordY()}; }
		inline QPoint toTilePoint() const noexcept { return QPoint{getTileX(), getTileY()}; }
		inline VectorLoc toVector() const noexcept { return VectorLoc{m_oWorld, getCoordX(), getCoordY()}; }

		inline void setCoordX(double dX) noexcept { Parent::setX(dX); }
		inline void setCoordY(double dY) noexcept { Parent::setY(dY); }
		inline void setYaw(double dYaw) noexcept { m_dYaw = dYaw; }
		void setDirection(Location const& oTarget) noexcept;

		double length() const noexcept;
		double lengthSquared() const noexcept;
		double euclideanDistanceTo(Location const& oTarget) const noexcept;
		int manhattanDistanceTo(Location const& oTarget) const noexcept;
		VectorLoc relativeVectorTo(Location const& oTarget) const noexcept;

		friend inline std::ostream& operator<<(std::ostream& oStream, Location const& oLeft) noexcept;
		friend inline QDebug operator<<(QDebug oStream, Location const& oLeft) noexcept;

		friend inline Location& operator+=(Location& oLeft, Location const& oRight) noexcept;
		friend inline Location& operator+=(Location& oLeft, QPointF const& oP) noexcept;
		friend inline Location& operator-=(Location& oLeft, Location const& oRight) noexcept;
		friend inline Location& operator-=(Location& oLeft, QPointF const& oP) noexcept;
		friend inline Location& operator*=(Location& oLeft, double dFactor) noexcept;
		friend inline Location& operator/=(Location& oLeft, double dFactor) noexcept;

		friend inline bool operator==(Location const& oLeft, Location const& oRight) noexcept;
		friend inline bool operator!=(Location const& oLeft, Location const& oRight) noexcept;
		friend inline Location operator+(Location const& oLeft, Location const& oRight) noexcept;
		friend inline Location operator+(Location const& oLeft, QPointF const& oP) noexcept;
		friend inline Location operator-(Location const& oLeft, Location const& oRight) noexcept;
		friend inline Location operator-(Location const& oLeft, QPointF const& oP) noexcept;
		friend inline Location operator*(Location const& oLeft, double dFactor) noexcept;
		friend inline Location operator/(Location const& oLeft, double dFactor) noexcept;

	private:
	protected:
		QWeakPointer<World const> m_oWorld;
		double m_dYaw;
	};

	/*****************************************************************************
	Operator functions
	*****************************************************************************/
	inline void swap(Location& oLeft, Location& oRight) noexcept
	{
		using std::swap;
		swap(static_cast<QPointF&>(oLeft), static_cast<QPointF&>(oRight));
		swap(oLeft.m_oWorld, oRight.m_oWorld);
		swap(oLeft.m_dYaw, oRight.m_dYaw);
	}

	inline std::ostream& operator<<(std::ostream& oStream, Location const& oLeft) noexcept
	{
		QString output = QLatin1String{"["} % QString::number(oLeft.getCoordX()) % QLatin1String{"("} % QString::number(oLeft.getTileX()) % QLatin1String{");"} % QString::number(oLeft.getCoordY()) % QLatin1String{"("} % QString::number(oLeft.getTileY()) % QLatin1String{")]"};
		oStream << output.toUtf8().constData();
		return oStream;
	}

	inline QDebug operator<<(QDebug oStream, Location const& oLeft) noexcept
	{
		QString output = QLatin1String{"["} % QString::number(oLeft.getCoordX()) % QLatin1String{"("} % QString::number(oLeft.getTileX()) % QLatin1String{");"} % QString::number(oLeft.getCoordY()) % QLatin1String{"("} % QString::number(oLeft.getTileY()) % QLatin1String{")]"};
		oStream.noquote() << output;
		return oStream;
	}

	inline Location& operator+=(Location& oLeft, Location const& oRight) noexcept
	{
		oLeft.operator+=(oRight);
		return oLeft;
	}

	inline Location& operator+=(Location& oLeft, QPointF const& oP) noexcept
	{
		oLeft.operator+=(oP);
		return oLeft;
	}

	inline Location& operator-=(Location& oLeft, Location const& oRight) noexcept
	{
		oLeft.operator-=(oRight);
		return oLeft;
	}

	inline Location& operator-=(Location& oLeft, QPointF const& oP) noexcept
	{
		oLeft.operator-=(oP);
		return oLeft;
	}

	inline Location& operator*=(Location& oLeft, double dFactor) noexcept
	{
		oLeft.operator*=(dFactor);
		return oLeft;
	}

	inline Location& operator/=(Location& oLeft, double dFactor) noexcept
	{
		oLeft.operator/=(dFactor);
		return oLeft;
	}

	inline bool operator==(Location const& oLeft, Location const& oRight) noexcept
	{
		return static_cast<QPointF>(oLeft) == static_cast<QPointF>(oRight) && oLeft.m_dYaw == oRight.m_dYaw;
	}

	inline bool operator!=(Location const& oLeft, Location const& oRight) noexcept
	{
		return static_cast<QPointF>(oLeft) != static_cast<QPointF>(oRight) || oLeft.m_dYaw != oRight.m_dYaw;
	}

	inline Location operator+(Location const& oLeft, Location const& oRight) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() + oRight.x(), oLeft.y() + oRight.y()};
	}

	inline Location operator+(Location const& oLeft, QPointF const& oP) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() + oP.x(), oLeft.y() + oP.y()};
	}

	inline Location operator-(Location const& oLeft, Location const& oRight) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() - oRight.x(), oLeft.y() - oRight.y()};
	}

	inline Location operator-(Location const& oLeft, QPointF const& oP) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() - oP.x(), oLeft.y() - oP.y()};
	}

	inline Location operator*(Location const& oLeft, double dFactor) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() * dFactor, oLeft.y() * dFactor};
	}

	inline Location operator/(Location const& oLeft, double dFactor) noexcept
	{
		return Location{oLeft.getWorld(), oLeft.x() / dFactor, oLeft.y() / dFactor};
	}
}
#endif
