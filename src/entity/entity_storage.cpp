/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "entity_storage.h"

#include "util/algorithm_util.h"

namespace FastSimDesign {
	/*****************************************************************************
	EntityIdPool::Methods
	*****************************************************************************/
	EntityStorage::EntityIdPool::EntityIdPool(std::size_t poolSize)
		: m_nextIdValue{0}
		, m_allocatedIds{poolSize}
		, m_freeIndexes{}
	{
	}

	Entity::Id EntityStorage::EntityIdPool::createId() noexcept
	{
		Entity::Id newId{m_nextIdValue, static_cast<Entity::Id::index_type>(m_allocatedIds.size())};
		if (!m_freeIndexes.empty())
		{
			newId.m_index = m_freeIndexes.top();
			m_freeIndexes.pop();
		}
		Q_ASSERT_X(std::none_of(std::begin(m_allocatedIds), std::end(m_allocatedIds), [newId](Entity::Id const& element) {
			return newId.m_index == element.m_index;
		}), "EntityStorage::EntityIdPool::create", "Id is not disallocated, you have to remove it before adding a new one");
		m_allocatedIds.push_back(newId);
		m_nextIdValue++;
		return newId;
	}

	void EntityStorage::EntityIdPool::destroyId(Entity::Id const& id) noexcept
	{
		auto found = std::find(std::begin(m_allocatedIds), std::end(m_allocatedIds), id);
		Q_ASSERT_X(found != std::end(m_allocatedIds), "EntityStorage::EntityIdPool::destroy", "Id does not exists");
		m_allocatedIds.erase(found);
		m_freeIndexes.push(found->m_index);
	}

	bool EntityStorage::EntityIdPool::isValid(Entity::Id const& id) const noexcept
	{
		bool isAllocted = std::any_of(std::begin(m_allocatedIds), std::end(m_allocatedIds), [id](Entity::Id const& element) {
			return id.m_index == element.m_index;
		});
		return isAllocted && id.m_id < m_nextIdValue && id.m_id >= 0 && id.m_index < allocatedIdCount() && id.m_index >= 0;
	}

	void EntityStorage::EntityIdPool::clear() noexcept
	{
		m_allocatedIds.clear();
		m_freeIndexes = std::stack<Entity::Id::index_type>{};
		m_nextIdValue = 0;
	}

	/*****************************************************************************
	EntityStorage::Methods
	*****************************************************************************/
	EntityStorage::EntityStorage(QObject* parent /*= nullptr*/) noexcept
		: QAbstractListModel{parent}
		, m_entities{}
		, m_entitiesToDestroy{}
		, m_idPool{0}
	{
	}

	EntityStorage::~EntityStorage() noexcept
	{
		term();
	}

	int EntityStorage::rowCount(QModelIndex const& parent /*= QModelIndex()*/) const noexcept
	{
		if (parent.isValid())
			return 0;
		return m_idPool.allocatedIdCount();
	}

	QVariant EntityStorage::data(QModelIndex const& index, int role /*= Qt::DisplayRole*/) const noexcept
	{
		int row = index.row();
		if (!index.isValid())
			return QVariant{};

		auto entityRow = find_nth_if_not(std::begin(m_entities), std::end(m_entities), row + 1, [](std::unique_ptr<Entity> const& element) {
			return element == nullptr;
		});
		switch (role)
		{
			case Qt::DisplayRole:
				return QVariant{(*entityRow)->name()};
				break;
		}
		return QVariant{};
	}

	void EntityStorage::init() noexcept
	{
		for (std::unique_ptr<Entity> const& entity : m_entities)
		{
			qInfo("Initializing entity \"%llu\"...", entity->id().m_id);
			entity->init();
			qInfo("Entity \"%llu\" initialized successfully!", entity->id().m_id);
		}
	}

	void EntityStorage::update(QTime const& deltaTime) noexcept
	{
		// --- Destruction of entities present in the list of indexes. ---
		qInfo("~~~ Destroying of entities present in the list of indexes ~~~");
		if (!m_entitiesToDestroy.empty())
		{
			for (Entity::Id const& id : m_entitiesToDestroy)
			{
				beginRemoveRows(QModelIndex{}, id.m_index, id.m_index);
				std::unique_ptr<Entity> entityExtracted{m_entities[id.m_index].release()};
				entityExtracted->term();
				entityExtracted->setParent(nullptr);
				entityExtracted.reset();
				m_idPool.destroyId(id);
				endRemoveRows();
				qInfo("Entity \"%llu\" destroyed successfully!", id.m_id);
			}
			m_entitiesToDestroy.clear();
		}
		qInfo("~~~ End of destruction of entities ~~~");

		// --- Update of entities. ---
		qInfo("~~~ Updating of entities ~~~");
		for (std::unique_ptr<Entity> const& entity : m_entities)
			entity->update(deltaTime);
		qInfo("~~~ End of updating of entities ~~~");
		emit allEntitiesUpdated();
	}

	void EntityStorage::term() noexcept
	{
		for (std::unique_ptr<Entity> const& entity : m_entities)
		{
			qInfo("Terminating entity \"%llu\"...", entity->id().m_id);
			entity->term();
			qInfo("Entity \"%llu\" terminated successfully!", entity->id().m_id);
		}
		clear();
	}

	EntityStorage::EntityContainerPtr EntityStorage::getAllPtr() const noexcept
	{
		EntityContainerPtr entities{};
		for (std::unique_ptr<Entity> const& entity : m_entities)
		{
			if (entity.get() != nullptr)
			{
				entities.push_back(entity.get());
			}
		}
		Q_ASSERT_X(entities.size() == count(), "EntityStorage::getAllPtr", "Can't get all entity, something wrong happened");
		return entities;
	}

	EntityStorage::EntityContainerConstPtr EntityStorage::getAllConstPtr() const noexcept
	{
		EntityContainerConstPtr entities{};
		for (std::unique_ptr<Entity> const& entity : m_entities)
		{
			if (entity.get() != nullptr)
			{
				entities.push_back(entity.get());
			}
		}
		Q_ASSERT_X(entities.size() == count(), "EntityStorage::getAllConstPtr", "Can't get all entity, something wrong happened");
		return entities;
	}

	void EntityStorage::destroy(Entity::Id const& id) noexcept
	{
		Q_ASSERT_X(isValid(id), "EntityStorage::destroy", "Entity does not exists");
		m_entitiesToDestroy.push_back(id);
		emit entityDestroyed(id);
	}

	void EntityStorage::clear() noexcept
	{
		beginResetModel();
		for (std::unique_ptr<Entity>& entity : m_entities)
		{
			entity->setParent(nullptr);
			entity.reset();
		}
		m_entitiesToDestroy.clear();
		m_idPool.clear();
		m_entities.clear();
		endResetModel();
		emit allEntitiesDestroyed();
	}

	Entity::Id EntityStorage::createEntityId() noexcept
	{
		return m_idPool.createId();
	}

	bool EntityStorage::isValid(Entity::Id const& id) const noexcept
	{
		if (m_idPool.isValid(id))
		{
			if (id.m_index >= 0 && id.m_index < static_cast<Entity::Id::index_type>(m_entities.size()) && m_entities[id.m_index] != nullptr)
				return true;
		}
		return false;
	}
}
