/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_WORLD_POPULATOR_H
#define FAST_SIM_DESIGN_WORLD_POPULATOR_H

#include "resource/map_resource.h"

namespace FastSimDesign {
	class WorldPopulator final
	{
	public:
		explicit WorldPopulator() = delete; // Default constructor
		explicit WorldPopulator(QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> oTiledObjects) noexcept;
		WorldPopulator(WorldPopulator const&) = delete; // Copy constructor
		WorldPopulator(WorldPopulator&&) = default; // Move constructor
		WorldPopulator& operator=(WorldPopulator const&) = delete; // Copy assignment operator
		WorldPopulator& operator=(WorldPopulator&&) = default; // Move assignment operator
		virtual ~WorldPopulator() = default; // Destructor

		void populateWorld(QSharedPointer<World> oWorld); // throw WorldException
	protected:
		void createNpcs(); // throw WorldException
		void createObjects(); // throw WorldException
	protected:
		QSharedPointer<World> m_oWorld;
		QMultiMap<MapResource::TiledObjectType, Tiled::MapObject*> m_oTiledObjects;

		bool m_bIsWorldPopulated;

	private:
	};
}
#endif
