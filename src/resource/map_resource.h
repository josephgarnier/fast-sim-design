/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_MAP_RESOURCE_H
#define FAST_SIM_DESIGN_MAP_RESOURCE_H

#include "resource.h"
#include "tield/maprenderer.h"

namespace FastSimDesign {
	class MapResource final : public Resource
	{
		Q_OBJECT
	public:
		static QMetaEnum getMetaEnum(QString const& oName) noexcept;

	public:
		/*
		* Same name that objects type of Tiled Map Editor
		*/
		enum TiledObjectType
		{
			OBJECT = 0,
			NPC,
			SLOT
		};
		Q_ENUM(TiledObjectType)

		explicit MapResource(QFileInfo oFilePath, QObject* pParent = Q_NULLPTR); // throw ResourceException
		MapResource(MapResource&&) = default; // Move constructor
		MapResource& operator=(MapResource&&) = default; // Move assignment operator
		virtual ~MapResource() = default; // Destructor

		inline Tiled::Map* getMap() const noexcept { return m_oMap.data(); }
		inline Tiled::TileLayer* getCollisionLayer() const noexcept { return m_pCollisionLayer; }
		inline QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> const& getTiledObjects() const noexcept { return m_oTiledObjects; }
		inline Tiled::MapRenderer const* getMapRender() const noexcept { return m_oRenderer.data(); }
		inline QString getBeingProjectName() const noexcept { return m_sBeingProjectName; }

		void saveMapFile() const; // throw ResourceException
	protected:
		void loadMapFile(); // throw ResourceException
		void loadCollisionLayer(); // throw ResourceException
		void loadEntitiesLayer(); // throw ResourceException
		void createMapRender() noexcept;

	protected:
		QScopedPointer<Tiled::Map> m_oMap;
		Tiled::TileLayer* m_pCollisionLayer;
		QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> m_oTiledObjects;
		QScopedPointer<Tiled::MapRenderer> m_oRenderer;
		QString m_sBeingProjectName;

	private:
	};
}
#endif
