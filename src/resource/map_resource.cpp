/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "map_resource.h"
#include "tield/mapreader.h"
#include "tield/mapwriter.h"
#include "tield/objectgroup.h"
#include "resource_exception.h"
#include "tield/hexagonalrenderer.h"
#include "tield/isometricrenderer.h"
#include "tield/orthogonalrenderer.h"
#include "tield/staggeredrenderer.h"

namespace FastSimDesign {
	/*****************************************************************************
	Statics
	*****************************************************************************/
	QMetaEnum MapResource::getMetaEnum(QString const& oName) noexcept
	{
		QMetaObject const& mo = MapResource::staticMetaObject;
		int index = mo.indexOfEnumerator(oName.toUtf8().constData());
		Q_ASSERT_X(index != -1, "", "Meta enum does not exists");
		return mo.enumerator(index);
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	MapResource::MapResource(QFileInfo oFilePath, QObject* pParent /*= 0*/)
		: Resource{std::move(oFilePath), pParent}
		, m_oMap{Q_NULLPTR}
		, m_pCollisionLayer{Q_NULLPTR}
		, m_oTiledObjects{}
		, m_oRenderer{Q_NULLPTR}
		, m_sBeingProjectName{}
	{
		try
		{
			setStatus(Resource::Status::Loading);
			loadMapFile();
			qInfo() << "Map file found on disk.";
			if (getStatus() != Resource::Status::Error)
			{
				loadCollisionLayer();
				qInfo() << "\"Collision\" layer checked and loaded.";
			}
			if (getStatus() != Resource::Status::Error)
			{
				loadEntitiesLayer();
				qInfo() << "\"Entity\" layer checked and loaded.";
			}
			if (getStatus() != Resource::Status::Error)
			{
				createMapRender();
				qInfo() << "Map render created.";
			}
			checkReady();
		} catch (...)
		{
			throw;
		}
	}

	void MapResource::saveMapFile() const
	{
		Q_ASSERT_X(m_oAbsoluteFilePath.exists() && m_oAbsoluteFilePath.isReadable(), "", "Filepath does not exists or is unreadable");
		Tiled::MapWriter writer;
		if (!writer.writeMap(m_oMap.data(), m_oAbsoluteFilePath.filePath()))
		{
			qCritical() << "Failed to save map:" << writer.errorString() << "!";
			throw ResourceException{QLatin1String{"Failed to save map: "} % writer.errorString()};
		}
	}

	void MapResource::loadMapFile()
	{
		if (!m_oAbsoluteFilePath.exists() || !m_oAbsoluteFilePath.isReadable())
		{
			setStatus(Resource::Status::Error);
			qCritical() << "The file path" << m_oAbsoluteFilePath.filePath() << "does not exists or is unreadable!";
			throw ResourceException{QLatin1String{"The file path "} % m_oAbsoluteFilePath.filePath() % QLatin1String{" does not exists or is unreadable"}};
		}

		Tiled::MapReader reader;
		m_oMap.reset(reader.readMap(m_oAbsoluteFilePath.filePath()).get());
		if (m_oMap.isNull())
		{
			setStatus(Resource::Status::Error);
			qCritical() << "Failed to read the map file:" << reader.errorString() << "!";
			throw ResourceException{QLatin1String{"Failed to read the map file: "} % reader.errorString()};
		}
		if (m_oMap->property(QStringLiteral("being_project_name")).isNull())
		{
			setStatus(Resource::Status::Error);
			qCritical() << "The property 'being_project_name' is missing!";
			throw ResourceException{QLatin1String{"The property 'being_project_name' is missing"}};
		}
		m_sBeingProjectName = m_oMap->property(QStringLiteral("being_project_name")).toString();
	}

	void MapResource::loadCollisionLayer()
	{
		Q_ASSERT_X(!m_oMap.isNull(), "", "Map is null");
		int collisionLayerIndex = m_oMap->indexOfLayer(QStringLiteral("collision"));
		if (collisionLayerIndex == -1)
		{
			setStatus(Resource::Status::Error);
			qCritical() << "No collision layer found (need a layer with the name \"collision\")!";
			throw ResourceException{QLatin1String{"No collision layer found (need a layer with the name \"collision\")"}};
		}
		m_pCollisionLayer = dynamic_cast<Tiled::TileLayer*>(m_oMap->layerAt(collisionLayerIndex));
	}

	void MapResource::loadEntitiesLayer()
	{
		Q_ASSERT_X(!m_oMap.isNull(), "", "Map is null");
		int entityLayerIndex = m_oMap->indexOfLayer(QStringLiteral("entity"));
		if (entityLayerIndex == -1)
		{
			setStatus(Resource::Status::Error);
			qCritical() << "No entity layer found (need a layer with the name \"entity\")!";
			throw ResourceException{QLatin1String{"No entity layer found (need a layer with the name \"entity\")."}};
		}
		Tiled::ObjectGroup* objectGroup = dynamic_cast<Tiled::ObjectGroup*>(m_oMap->layerAt(entityLayerIndex));
		for (Tiled::MapObject* object : objectGroup->objects())
		{
			QMetaEnum tiledObjectTypeEnum = MapResource::getMetaEnum(QStringLiteral("TiledObjectType"));
			int type = tiledObjectTypeEnum.keyToValue(object->type().toUtf8().constData());
			m_oTiledObjects.insert(static_cast<MapResource::TiledObjectType>(type), object);
		}
	}

	void MapResource::createMapRender() noexcept
	{
		Q_ASSERT_X(!m_oMap.isNull(), "", "Map is null");
		switch (m_oMap->orientation())
		{
			case Tiled::Map::Isometric:
				m_oRenderer.reset(new Tiled::IsometricRenderer{m_oMap.data()});
				break;
			case Tiled::Map::Staggered:
				m_oRenderer.reset(new Tiled::StaggeredRenderer{m_oMap.data()});
				break;
			case Tiled::Map::Hexagonal:
				m_oRenderer.reset(new Tiled::HexagonalRenderer{m_oMap.data()});
				break;
			case Tiled::Map::Orthogonal:
			default:
				m_oRenderer.reset(new Tiled::OrthogonalRenderer{m_oMap.data()});
				break;
		}
	}
}