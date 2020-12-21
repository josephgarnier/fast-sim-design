/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world.h"

namespace FastSimDesign {
	/*****************************************************************************
	Methods
	*****************************************************************************/
	template<typename EntityType, typename... EntityArgs>
	Entity::Id World::createEntity(EntityArgs&&... args) noexcept
	{
		Entity::Id newEntityId = m_oEntities->create<EntityType>(sharedFromThis(), m_oEntities.get(), m_oEntities->createEntityId(), std::forward<EntityArgs>(args)...);
		Q_ASSERT_X(!isCollide(m_oEntities->get<EntityType>(newEntityId)), "", "Location destination is not free");
		return newEntityId;
	}
}