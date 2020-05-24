/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SLOT_H
#define FAST_SIM_DESIGN_SLOT_H

namespace FastSimDesign {
	class FAST_SIM_DESIGN_API Slot final
	{
	public:
		explicit Slot(); // Default constructor
		explicit Slot(QWeakPointer<World const> oWorld, Tiled::MapObject* const pArea) noexcept;
		Slot(Slot const&) = default; // Copy constructor
		Slot(Slot&&) = default; // Move constructor
		Slot& operator=(Slot const&) = default; // Copy assignment operator
		Slot& operator=(Slot&&) = default; // Move assignment operator
		virtual ~Slot() = default; // Destructor

		inline int getId() const noexcept { return m_pArea->id(); }
		inline Tiled::MapObject const* const getArea() const noexcept { return m_pArea; }
		inline AxisAlignedBB getAreaBoundingBox() const noexcept { return AxisAlignedBB::getBoundingBox(m_pArea); }
		Location getLocation() const noexcept;

		inline void hold(QWeakPointer<Entity const> oHolder) noexcept
		{
			m_bIsFree = false;
			m_oHolder = std::move(oHolder);
		}
		inline QSharedPointer<Entity const> getHolder() const noexcept { return m_oHolder.lock(); }
		inline bool isFree() const noexcept { return m_bIsFree; }
		inline void release() noexcept
		{
			m_bIsFree = true;
			m_oHolder.clear();
		}

		friend inline uint qHash(Slot const& oKey, uint uiSeed) noexcept;
		friend inline uint qHash(Slot const& oKey) noexcept;
		friend inline bool operator==(Slot const& oLeft, Slot const& oRight) noexcept;
		friend inline bool operator!=(Slot const& oLeft, Slot const& oRight) noexcept;

	protected:
	private:
		QWeakPointer<World const> m_oWorld;
		Tiled::MapObject* m_pArea;
		bool m_bIsFree;
		QWeakPointer<Entity const> m_oHolder;
	};

	/*****************************************************************************
	Operator functions
	*****************************************************************************/
	inline uint qHash(Slot const& oKey, uint uiSeed) noexcept
	{
		return uiSeed ^ static_cast<int>(oKey.m_pArea->id()) ^ static_cast<int>(oKey.m_bIsFree);
	}

	inline uint qHash(Slot const& oKey) noexcept
	{
		return static_cast<int>(oKey.m_pArea->id()) ^ static_cast<int>(oKey.m_bIsFree);
	}

	inline bool operator==(Slot const& oLeft, Slot const& oRight) noexcept
	{
		return oLeft.m_pArea->id() == oRight.m_pArea->id();
	}

	inline bool operator!=(Slot const& oLeft, Slot const& oRight) noexcept
	{
		return !(oLeft == oRight);
	}
}
#endif
