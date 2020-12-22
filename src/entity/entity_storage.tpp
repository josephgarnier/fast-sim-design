
/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "entity_storage.h"

namespace FastSimDesign {
	/*****************************************************************************
	Methods
	*****************************************************************************/
	template<typename EntityType>
	EntityType const& EntityStorage::get(Entity::Id const& id) const noexcept
	{
		Q_ASSERT_X(isValid(id), "EntityStorage::get", "Entity does not exists");
		EntityType const* castedEntity = dynamic_cast<EntityType const*>(m_entities[id.m_index].get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::get", "This entity cannot be cast to T type");
		return *castedEntity;
	}

	template<typename EntityType>
	EntityType& EntityStorage::get(Entity::Id const& id) noexcept
	{
		Q_ASSERT_X(isValid(id), "EntityStorage::get", "Entity does not exists");
		EntityType* castedEntity = dynamic_cast<EntityType*>(m_entities[id.m_index].get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::get", "This entity cannot be cast to T type");
		return *castedEntity;
	}

	template<typename EntityType>
	EntityType const& EntityStorage::get(Tiled::MapObject const& sprite) const noexcept
	{
		auto found = std::find_if(std::begin(m_entities), std::end(m_entities), [sprite](std::unique_ptr<Entity> const& element) {
			return element->isOwnSprite(sprite);
		});
		Q_ASSERT_X(found != std::end(m_entities), "EntityStorage::get", "Entity does not exists");

		EntityType const* castedEntity = dynamic_cast<EntityType const*>(found->get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::get", "This entity cannot be cast to T type");
		return *castedEntity;
	}

	template<typename EntityType>
	EntityType& EntityStorage::get(Tiled::MapObject const& sprite) noexcept
	{
		auto found = std::find_if(std::begin(m_entities), std::end(m_entities), [sprite](std::unique_ptr<Entity> const& element) {
			return element->isOwnSprite(sprite);
		});
		Q_ASSERT_X(found != std::end(m_entities), "EntityStorage::get", "Entity does not exists");

		EntityType* castedEntity = dynamic_cast<EntityType*>(found->get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::get", "This entity cannot be cast to T type");
		return *castedEntity;
	}

	template<typename EntityType>
	EntityType const* EntityStorage::getPtr(Entity::Id const& id) const noexcept
	{
		Q_ASSERT_X(isValid(id), "EntityStorage::getPtr", "Entity does not exists");
		EntityType* castedEntity = dynamic_cast<EntityType*>(m_entities[id.m_index].get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::getPtr", "This entity cannot be cast to T type");
		return castedEntity;
	}

	template<typename EntityType>
	EntityType* EntityStorage::getPtr(Entity::Id const& id) noexcept
	{
		Q_ASSERT_X(isValid(id), "EntityStorage::getPtr", "Entity does not exists");
		EntityType* castedEntity = dynamic_cast<EntityType*>(m_entities[id.m_index].get());
		Q_ASSERT_X(castedEntity != nullptr, "EntityStorage::getPtr", "This entity cannot be cast to T type");
		return castedEntity;
	}

	template<typename EntityType, typename... EntityArgs>
	Entity::Id EntityStorage::create(EntityArgs&&... args) noexcept
	{
		std::unique_ptr<Entity> newEntity = std::make_unique<EntityType>(std::forward<EntityArgs>(args)...);
		Q_ASSERT_X(!std::count(std::begin(m_entities), std::end(m_entities), newEntity), "EntityStorage::create", "Entity already exists, you have to erase it before push it again");
		Q_ASSERT_X(m_idPool.isValid(newEntity->id()), "EntityStorage::create", "Entity id is not valid");

		beginInsertRows(QModelIndex{}, m_idPool.allocatedIdCount(), m_idPool.allocatedIdCount());
		Entity::Id const& idNewEntity = newEntity->id();
		if (idNewEntity.m_index >= static_cast<Entity::Id::index_type>(m_entities.size())) // resize the container of entities
		{
			m_entities.resize(idNewEntity.m_index + 1);
		}
		Q_ASSERT_X(m_entities[idNewEntity.m_index] == nullptr, "EntityStorage::create", "Index should be free");
		newEntity->setParent(this);
		m_entities[idNewEntity.m_index].swap(newEntity);
		endInsertRows();

		emit entityCreated(idNewEntity);
		return idNewEntity;
	}
}
