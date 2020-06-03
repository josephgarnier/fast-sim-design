/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_VECTOR_LOC_H
#define FAST_SIM_DESIGN_VECTOR_LOC_H

#include "QVector2D"
#include "QWeakPointer"

namespace FastSimDesign {
	class World;
	class Location;
	class VectorLoc final : private QVector2D
	{
	private:
		using Parent = QVector2D;

	public:
		explicit VectorLoc() = default; // Default constructor
		explicit VectorLoc(QWeakPointer<World const> oWorld, double dXCoord, double dYCoord) noexcept;
		explicit VectorLoc(QWeakPointer<World const> oWorld, QVector2D const& oVect) noexcept;
		explicit VectorLoc(QWeakPointer<World const> oWorld, Location const& oLoc) noexcept;
		VectorLoc(VectorLoc const&) = default; // Copy constructor
		VectorLoc(VectorLoc&&) = default; // Move constructor
		VectorLoc& operator=(VectorLoc const&) = default; // Copy assignment operator
		VectorLoc& operator=(VectorLoc&&) = default; // Move assignment operator
		virtual ~VectorLoc() = default; // Destructor

		friend inline void swap(VectorLoc& oLeft, VectorLoc& oRight) noexcept;

		bool isValid() const noexcept;

		inline QSharedPointer<World const> getWorld() const noexcept { return m_oWorld.lock(); }
		inline double getCoordX() const noexcept { return Parent::x(); }
		inline double getCoordY() const noexcept { return Parent::y(); }

		inline QVector2D toVector() const noexcept { return QVector2D{Parent::x(), Parent::y()}; }
		Location toLocation() const noexcept;

		inline void setCoordX(double dX) noexcept { Parent::setX(dX); }
		inline void setCoordY(double dY) noexcept { Parent::setY(dY); }

		/*!
		 * Gets the magnitude (also called Euclidean norm) of the vector.
		 */
		double length() const noexcept;

		/*!
		* Gets the magnitude of the vector squared.
		*/
		double lengthSquared() const noexcept;

		/*!
		 * The angle of this vector in degrees.
		 * The return value will be in the range of values from 0.0 up to but not including 360.0.
		 * The angles are measured in the trigonometric sens from the x-axis to this vector.
		 */
		double angle() const noexcept;

		/*!
		 * Returns the angle in degrees between this vector and another one.
		 * The return value will be in the range of values from 0.0 up to but not including 360.0.
		 * The angles are measured in the trigonometric sens from this vector to the other vector.
		 */
		double angleTo(VectorLoc const& oVector) const noexcept;

		/*!
		 * Calculates the dot product of this vector with another. The dot product is defined as x1*x2 + y1*y2.
		 */
		double dotProduct(VectorLoc const& oVector) const noexcept;

		/*!
		 * Calculate the cross production of this vector with another. The cross product is defined as x1*y2 - y1*x2.
		 */
		double crossProduct(VectorLoc const& oVector) const noexcept;

		/*!
		 * Converts this vector to a unit vector (a vector with length of 1). Normalization is defined as x = px/length, y = py/length.
		 */
		void normalize() noexcept;

		/*!
		 * Return a unit vector. 
		 */
		VectorLoc unitVector() const noexcept;

		/*!
		 *	Turn the vector direction to the target
		 */
		void setDirection(Location const& oTarget) noexcept;

		friend inline VectorLoc& operator+=(VectorLoc& oLeft, VectorLoc const& oRight) noexcept;
		friend inline VectorLoc& operator-=(VectorLoc& oLeft, VectorLoc const& oRight) noexcept;
		friend inline VectorLoc& operator*=(VectorLoc& oLeft, double dFactor) noexcept;
		friend inline VectorLoc& operator/=(VectorLoc& oLeft, double dFactor) noexcept;

		friend inline bool operator==(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept;
		friend inline bool operator!=(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept;
		friend inline VectorLoc operator+(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept;
		friend inline VectorLoc operator-(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept;
		friend inline VectorLoc operator*(VectorLoc const& oLeft, double dFactor) noexcept;
		friend inline VectorLoc operator/(VectorLoc const& oLeft, double dFactor) noexcept;

	protected:
		QWeakPointer<World const> m_oWorld;

	private:
	};

	/*****************************************************************************
	Operator functions
	*****************************************************************************/
	inline void swap(VectorLoc& oLeft, VectorLoc& oRight) noexcept
	{
		using std::swap;
		swap(static_cast<QVector2D&>(oLeft), static_cast<QVector2D&>(oRight));
		swap(oLeft.m_oWorld, oRight.m_oWorld);
	}

	inline VectorLoc& operator+=(VectorLoc& oLeft, VectorLoc const& oRight) noexcept
	{
		oLeft.operator+=(oRight);
		return oLeft;
	}

	inline VectorLoc& operator-=(VectorLoc& oLeft, VectorLoc const& oRight) noexcept
	{
		oLeft.operator-=(oRight);
		return oLeft;
	}

	inline VectorLoc& operator*=(VectorLoc& oLeft, double dFactor) noexcept
	{
		oLeft.operator*=(dFactor);
		return oLeft;
	}

	inline VectorLoc& operator/=(VectorLoc& oLeft, double dFactor) noexcept
	{
		oLeft.operator/=(dFactor);
		return oLeft;
	}

	inline bool operator==(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept
	{
		return static_cast<QVector2D>(oLeft) == static_cast<QVector2D>(oRight);
	}

	inline bool operator!=(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept
	{
		return static_cast<QVector2D>(oLeft) != static_cast<QVector2D>(oRight);
	}

	inline VectorLoc operator+(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept
	{
		return VectorLoc{oLeft.getWorld(), oLeft.x() + oRight.x(), oLeft.y() + oRight.y()};
	}

	inline VectorLoc operator-(VectorLoc const& oLeft, VectorLoc const& oRight) noexcept
	{
		return VectorLoc{oLeft.getWorld(), oLeft.x() - oRight.x(), oLeft.y() - oRight.y()};
	}

	inline VectorLoc operator*(VectorLoc const& oLeft, double dFactor) noexcept
	{
		return VectorLoc{oLeft.getWorld(), oLeft.x() * dFactor, oLeft.y() * dFactor};
	}

	inline VectorLoc operator/(VectorLoc const& oLeft, double dFactor) noexcept
	{
		return VectorLoc{oLeft.getWorld(), oLeft.x() / dFactor, oLeft.y() / dFactor};
	}
}
#endif
