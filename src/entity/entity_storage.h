/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_ENTITY_STORAGE_H
#define FAST_SIM_DESIGN_ENTITY_STORAGE_H

#include "stack"
#include "vector"

#include "QAbstractListModel"

#include "entity.h"

namespace FastSimDesign {
	class EntityStorage final : public QAbstractListModel
	{
		Q_OBJECT
	private:
		/*!
		* Inspired from https://github.com/miguelmartin75/anax/blob/master/include/anax/detail/EntityIdPool.hpp.
		*/
		class EntityIdPool final
		{
		public:
			explicit EntityIdPool(std::size_t poolSize); // Default constructor
			EntityIdPool(EntityIdPool const&) = delete; // Copy constructor
			EntityIdPool(EntityIdPool&&) = delete; // Move constructor
			EntityIdPool& operator=(EntityIdPool const&) = delete; // Copy assignment operator
			EntityIdPool& operator=(EntityIdPool&&) = delete; // Move assignment operator
			virtual ~EntityIdPool() = default; // Destructor

			Entity::Id create() noexcept;
			void destroy(Entity::Id const& id) noexcept;
			bool isValid(Entity::Id const& id) const noexcept;
			inline std::vector<Entity::Id> const& allAllocatedIds() const noexcept { return m_allocatedIds; }
			inline std::size_t allocatedIdCount() const noexcept { return m_allocatedIds.size(); }
			inline std::size_t freeIndexeCount() const noexcept { return m_freeIndexes.size(); }
			void clear() noexcept;

		private:
			Entity::Id::value_type m_nextIdValue;
			std::vector<Entity::Id> m_allocatedIds;
			std::stack<Entity::Id::index_type> m_freeIndexes;
		};

	public:
		using EntityContainer = std::vector<std::unique_ptr<Entity>>;
		using EntityContainerPtr = std::vector<Entity*>;
		using EntityContainerConstPtr = std::vector<Entity const*>;

	private:
		using Parent = QAbstractListModel;

	Q_SIGNALS:
		void allEntitiesUpdated();
		void allEntitiesDestroyed();
		void entityCreated(Entity::Id const& entityId);
		void entityDestroyed(Entity::Id const& entityId);

	public:
		explicit EntityStorage(QObject* parent = nullptr) noexcept; // Default constructor
		virtual ~EntityStorage() noexcept; // Destructor

		virtual int rowCount(QModelIndex const& parent = QModelIndex{}) const noexcept override;
		virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const noexcept override;

		void init() noexcept;
		void update(QTime const& deltaTime) noexcept;
		void term() noexcept;

		inline std::size_t count() const noexcept { return m_entityIdPool.allocatedIdCount(); }

		inline EntityContainer::iterator begin() noexcept { return m_entities.begin(); }
		inline EntityContainer::const_iterator begin() const noexcept { return m_entities.cbegin(); }
		inline EntityContainer::iterator end() noexcept { return m_entities.end(); }
		inline EntityContainer::const_iterator end() const noexcept { return m_entities.cend(); }

		template<typename EntityType>
		EntityType const& get(Entity::Id const& id) const noexcept;
		template<typename EntityType>
		EntityType& get(Entity::Id const& id) noexcept;
		template<typename EntityType>
		EntityType const& get(Tiled::MapObject const& sprite) const noexcept;
		template<typename EntityType>
		EntityType& get(Tiled::MapObject const& sprite) noexcept;

		EntityContainerPtr getAllPtr() const noexcept;
		EntityContainerConstPtr getAllConstPtr() const noexcept;
		template<typename EntityType>
		EntityType const* getPtr(Entity::Id const& id) const noexcept;
		template<typename EntityType>
		EntityType* getPtr(Entity::Id const& id) noexcept;

		template<typename EntityType, typename... EntityArgs>
		Entity::Id create(EntityArgs&&... args) noexcept;

		void destroy(Entity::Id const& id) noexcept;
		void clear() noexcept;

		Entity::Id createEntityId() noexcept;
		bool isValid(Entity::Id const& id) const noexcept;

	protected:
	private:
		EntityContainer m_entities;
		std::vector<Entity::Id> m_entitiesToDestroy;
		EntityStorage::EntityIdPool m_entityIdPool;
	};
}
#include "entity_storage.tpp"
#endif
