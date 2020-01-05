/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_AXIS_ALIGNED_BB_H
#define FAST_SIM_DESIGN_AXIS_ALIGNED_BB_H

#include "QRectF"
#include "tield/mapobject.h"

namespace FastSimDesign {
	class AxisAlignedBB final : private QRectF
	{
	private:
		using Parent = QRectF;

	public:
		static AxisAlignedBB getBoundingBox(Tiled::MapObject const* const pSprite) noexcept;

	public:
		using Parent::bottom;
		using Parent::bottomLeft;
		using Parent::bottomRight;
		using Parent::center;
		using Parent::height;
		using Parent::left;
		using Parent::moveCenter;
		using Parent::right;
		using Parent::size;
		using Parent::top;
		using Parent::topLeft;
		using Parent::topRight;
		using Parent::width;
		using Parent::x;
		using Parent::y;

	public:
		explicit AxisAlignedBB() = delete; // Default constructor
		explicit AxisAlignedBB(QPointF oTopLeft, QSizeF oSize, double dActualYaw) noexcept;
		explicit AxisAlignedBB(QPointF oTopLeft, QPointF oBottomRight, double dActualYaw) noexcept;
		explicit AxisAlignedBB(double dX, double dY, double dWidth, double dHeight, double dActualYaw) noexcept;
		AxisAlignedBB(AxisAlignedBB const&) = default; // Copy constructor
		AxisAlignedBB(AxisAlignedBB&&) = default; // Move constructor
		AxisAlignedBB& operator=(AxisAlignedBB const&) = default; // Copy assignment operator
		AxisAlignedBB& operator=(AxisAlignedBB&&) = default; // Move assignment operator
		virtual ~AxisAlignedBB() = default; // Destructor

		inline double getYaw() const noexcept { return m_dYaw; }
		inline double radius() const noexcept { return (qSqrt(height() * height() + width() * width())) / 2; }

		AxisAlignedBB boundUseTile(int iTileWidth, int iTileHeight) const noexcept;
		bool intersects(AxisAlignedBB const& oAabb) const noexcept;
		AxisAlignedBB translate(double dX, double dY) const noexcept;
		AxisAlignedBB translate(QPointF const& oOffset) const noexcept;
		AxisAlignedBB rotateAroundCenter(double dYaw);

		/*!
		 * It's not recommanded to use this function because of QRect implementation.
		 * 
		 * QRect documentation says :
		 * Note that for historical reasons the values returned by the bottom() and right() functions deviate from the true bottom-right corner of the rectangle:
		 * The right() function returns left() + width() - 1 and the bottom() function returns top() + height() - 1.
		 * The same is the case for the point returned by the bottomRight() convenience function. In addition, the x and y coordinate of the topRight()
		 * and bottomLeft() functions, respectively, contain the same deviation from the true right and bottom edges.
		 */
		QRect toTileRect() const noexcept;

		double area() const noexcept;

		friend inline bool operator==(AxisAlignedBB const& oLeft, AxisAlignedBB const& oRight) noexcept;
		friend inline bool operator!=(AxisAlignedBB const& oLeft, AxisAlignedBB const& oRight) noexcept;

	private:
	protected:
		double m_dYaw;
	};

	/*****************************************************************************
	Operator functions
	*****************************************************************************/
	inline bool operator==(AxisAlignedBB const& oLeft, AxisAlignedBB const& oRight) noexcept
	{
		return static_cast<QRectF>(oLeft) == static_cast<QRectF>(oRight) && oLeft.m_dYaw == oRight.m_dYaw;
	}

	inline bool operator!=(AxisAlignedBB const& oLeft, AxisAlignedBB const& oRight) noexcept
	{
		return !(oLeft == oRight);
	}
}
#endif
