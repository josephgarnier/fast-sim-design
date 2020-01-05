/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "axis_aligned_bb.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	AxisAlignedBB AxisAlignedBB::getBoundingBox(Tiled::MapObject const* const pSprite) noexcept
	{
		/** BoundingBox zero centered **/
		QPointF topLeftBB{-(pSprite->width() / 2), -(pSprite->height() / 2)};
		QPointF bottomRightBB{pSprite->width() / 2, pSprite->height() / 2};
		QRectF boundingBox{topLeftBB, bottomRightBB};

		/** Rotate the Bounding Box **/
		QTransform matrixRotation{};
		matrixRotation.rotate(pSprite->rotation());
		QRectF boundingBoxRotated = matrixRotation.mapRect(boundingBox);

		/** Fix topLeft and bottomRight positions **/
		QPointF topLeftBbRotated{qMin(boundingBoxRotated.left(), boundingBoxRotated.right()), qMin(boundingBoxRotated.top(), boundingBoxRotated.bottom())};
		QPointF bottomRightBbRotated{qMax(boundingBoxRotated.left(), boundingBoxRotated.right()), qMax(boundingBoxRotated.top(), boundingBoxRotated.bottom())};
		boundingBoxRotated = QRectF{topLeftBbRotated, bottomRightBbRotated};

		/** Move the boundingBox to the sprite position **/
		QPointF offset = Entity::spriteAlignmentOffset(pSprite, pSprite->alignment());
		QPointF center = pSprite->position() - offset;
		boundingBoxRotated.moveCenter(center);

		AxisAlignedBB aabb{boundingBoxRotated.topLeft(), boundingBoxRotated.bottomRight(), static_cast<float>(pSprite->rotation())};
		return aabb;
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	AxisAlignedBB::AxisAlignedBB(QPointF oTopLeft, QSizeF oSize, double dActualYaw) noexcept
		: QRectF{std::move(oTopLeft), std::move(oSize)}
		, m_dYaw{fmod(360.0 + (fmod(dActualYaw, 360.0)), 360.0)}
	{
		Q_ASSERT_X(m_dYaw == 0.0f || m_dYaw == 90.0f || m_dYaw == 180.0f || m_dYaw == 270.0f, "", "Yaw has to be 0, 90, 180, or 270 degrees");
	}

	AxisAlignedBB::AxisAlignedBB(QPointF oTopLeft, QPointF oBottomRight, double dActualYaw) noexcept
		: QRectF{std::move(oTopLeft), std::move(oBottomRight)}
		, m_dYaw{fmod(360.0 + (fmod(dActualYaw, 360.0)), 360.0)}
	{
		Q_ASSERT_X(m_dYaw == 0.0f || m_dYaw == 90.0f || m_dYaw == 180.0f || m_dYaw == 270.0f, "", "Yaw has to be 0, 90, 180, or 270 degrees");
	}

	AxisAlignedBB::AxisAlignedBB(double dX, double dY, double dWidth, double dHeight, double dActualYaw) noexcept
		: QRectF{dX, dY, dWidth, dHeight}
		, m_dYaw{fmod(360.0 + (fmod(dActualYaw, 360.0)), 360.0)}
	{
		Q_ASSERT_X(m_dYaw == 0.0f || m_dYaw == 90.0f || m_dYaw == 180.0f || m_dYaw == 270.0f, "", "Yaw has to be 0, 90, 180, or 270 degrees");
	}

	AxisAlignedBB AxisAlignedBB::boundUseTile(int iTileWidth, int iTileHeight) const noexcept
	{
		Q_ASSERT_X(iTileWidth > 0 && iTileHeight > 0, "", "Tile size have to be higher than 0");
		double minTileX = qFloor(left() / iTileWidth);
		double maxTileX = qCeil(right() / iTileWidth) - 1; // tile borders are included
		double minTileY = qFloor(top() / iTileHeight);
		double maxTileY = qCeil(bottom() / iTileHeight) - 1; // tile borders are included

		AxisAlignedBB aabb{QPointF{minTileX, minTileY}, QPointF{maxTileX, maxTileY}, m_dYaw};
		return aabb;
	}

	bool AxisAlignedBB::intersects(AxisAlignedBB const& oAabb) const noexcept
	{
		return QRectF::intersects(oAabb);
	}

	AxisAlignedBB AxisAlignedBB::translate(double dX, double dY) const noexcept
	{
		QRectF newRect = Parent::translated(dX, dY);
		AxisAlignedBB newAabb{newRect.topLeft(), newRect.bottomRight(), m_dYaw};
		return newAabb;
	}

	AxisAlignedBB AxisAlignedBB::translate(QPointF const& oOffset) const noexcept
	{
		QRectF newRect = Parent::translated(oOffset);
		AxisAlignedBB newAabb{newRect.topLeft(), newRect.bottomRight(), m_dYaw};
		return newAabb;
	}

	AxisAlignedBB AxisAlignedBB::rotateAroundCenter(double dYaw)
	{
		/** BoundingBox zero centered **/
		QRectF boundingBox{topLeft(), bottomRight()};
		boundingBox.moveCenter(QPointF{0.0f, 0.0f});

		/** Rotate the Bounding Box **/
		QTransform matrixRotation{};
		matrixRotation.rotate(dYaw);
		QRectF boundingBoxRotated = matrixRotation.mapRect(boundingBox);

		/** Fix topLeft and bottomRight positions **/
		QPointF topLeftBbRotated{qMin(boundingBoxRotated.left(), boundingBoxRotated.right()), qMin(boundingBoxRotated.top(), boundingBoxRotated.bottom())};
		QPointF bottomRightBbRotated{qMax(boundingBoxRotated.left(), boundingBoxRotated.right()), qMax(boundingBoxRotated.top(), boundingBoxRotated.bottom())};
		boundingBoxRotated = QRectF{topLeftBbRotated, bottomRightBbRotated};

		/** Move the boundingBox to the original position **/
		boundingBoxRotated.moveCenter(center());

		AxisAlignedBB aabb{boundingBoxRotated.topLeft(), boundingBoxRotated.bottomRight(), dYaw};
		return aabb;
	}

	QRect AxisAlignedBB::toTileRect() const noexcept
	{
		return QRect{topLeft().toPoint(), size().toSize()};
	}

	double AxisAlignedBB::area() const noexcept
	{
		return height() * width();
	}

}