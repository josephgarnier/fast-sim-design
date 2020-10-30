/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "slot.h"
#include "entity.h"

namespace FastSimDesign {
	Slot::Slot()
		: m_oWorld{}
		, m_pArea{Q_NULLPTR}
		, m_bIsFree{}
		, m_oHolder{}
	{
	}

	Slot::Slot(QWeakPointer<World const> oWorld, Tiled::MapObject* const pArea) noexcept
		: m_oWorld{std::move(oWorld)}
		, m_pArea{pArea}
		, m_bIsFree{true}
		, m_oHolder{}
	{
	}

	Location Slot::getLocation() const noexcept
	{
		QPointF center = Entity::spriteCenterPosition(m_pArea, m_pArea->alignment());
		return Location{m_oWorld, center.x(), center.y(), float(m_pArea->rotation())};
	}
}
