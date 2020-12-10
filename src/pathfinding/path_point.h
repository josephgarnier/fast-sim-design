/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_PATH_POINT_H
#define FAST_SIM_DESIGN_PATH_POINT_H

#include "QPoint"
#include "QSharedPointer"
#include "QDebug"
#include "fstream"

namespace FastSimDesign {
	class PathPoint final : private QPoint
	{
	private:
		using Parent = QPoint;

	public:
		using Parent::operator*=;
		using Parent::operator+=;
		using Parent::operator-=;
		using Parent::operator/=;
		using Parent::x;
		using Parent::y;

	public:
		explicit PathPoint() noexcept; // Default constructor
		explicit PathPoint(int iX, int iY) noexcept;
		explicit PathPoint(int iX, int iY, QSharedPointer<PathPoint const> oParent) noexcept;
		PathPoint(PathPoint const& oCopy) noexcept; // Copy constructor
		PathPoint(PathPoint&& oCopy) noexcept; // Move constructor
		PathPoint& operator=(PathPoint oCopy) noexcept; // Copy-and-swap idiom
		virtual ~PathPoint() = default; // Destructor

		friend inline void swap(PathPoint& oLeft, PathPoint& oRight) noexcept;

		inline QSharedPointer<PathPoint const> getParent() const noexcept { return m_oParent; }

		/*!
		* Distance (heuristic) between the current point and the end point (destination)
		*/
		inline int getH() const noexcept { return m_iH; }

		/*!
		* Distance between the start point and the current point
		*/
		inline int getG() const noexcept { return m_iG; }

		/*!
		* Sum of G and H
		*/
		inline int getF() const noexcept { return m_iH + m_iG; }

		void calculateF(PathPoint const& oStartPoint, PathPoint const& oEndPoint) noexcept;
		bool updateFIfBetter(PathPoint const& oStartPoint, PathPoint const& oEndPoint) noexcept;

		inline void setParent(QSharedPointer<PathPoint const> oParent) noexcept
		{
			Q_ASSERT_X(oParent.isNull() || *this != *oParent.data(), "", "Parent and this current point cannot be the same");
			m_oParent = std::move(oParent);
		}

		inline QPoint toPoint() const noexcept { return QPoint{*this}; }

		friend inline std::uint32_t qHash(PathPoint const& oKey, std::uint32_t uiSeed) noexcept;
		friend inline std::uint32_t qHash(PathPoint const& oKey) noexcept;

		friend inline std::ostream& operator<<(std::ostream& oStream, PathPoint const& oLeft) noexcept;
		friend inline QDebug operator<<(QDebug oStream, PathPoint const& oLeft) noexcept;

		friend inline bool operator==(PathPoint const& oLeft, PathPoint const& oRight) noexcept;
		friend inline bool operator!=(PathPoint const& oLeft, PathPoint const& oRight) noexcept;
		friend inline bool operator>(PathPoint const& oLeft, PathPoint const& oRight) noexcept;
		friend inline bool operator<(PathPoint const& oLeft, PathPoint const& oRight) noexcept;

	protected:
		int calculateH(PathPoint const& oEndPoint) const noexcept;
		int calculateG(PathPoint const& oStartPoint) const noexcept;
		int manhattanDistanceTo(PathPoint const& oTarget) const noexcept;

	protected:
		QSharedPointer<PathPoint const> m_oParent;
		int m_iH;
		int m_iG;

	private:
	};

	/*****************************************************************************
	Operator functions
	*****************************************************************************/
	inline void swap(PathPoint& oLeft, PathPoint& oRight) noexcept
	{
		using std::swap;
		swap(static_cast<QPoint&>(oLeft), static_cast<QPoint&>(oRight));
		swap(oLeft.m_oParent, oRight.m_oParent);
		//oLeft.m_oParent.swap(oRight.m_oParent);
		swap(oLeft.m_iH, oRight.m_iH);
		swap(oLeft.m_iG, oRight.m_iG);
	}

	inline std::uint32_t qHash(PathPoint const& oKey, std::uint32_t uiSeed) noexcept
	{
		return uiSeed ^ oKey.getH() ^ oKey.getG() ^ oKey.x() ^ oKey.y();
	}

	inline std::uint32_t qHash(PathPoint const& oKey) noexcept
	{
		return oKey.getH() ^ oKey.getG() ^ oKey.x() ^ oKey.y();
	}

	inline std::ostream& operator<<(std::ostream& oStream, PathPoint const& oLeft) noexcept
	{
		QString output = QLatin1String{"[?("} % QString::number(oLeft.x()) % QLatin1String{");"} % QLatin1String{"?("} % QString::number(oLeft.y()) % QLatin1String{")]"};
		oStream << output.toUtf8().constData();
		return oStream;
	}

	inline QDebug operator<<(QDebug oStream, PathPoint const& oLeft) noexcept
	{
		QString output = QLatin1String{"[?("} % QString::number(oLeft.x()) % QLatin1String{");"} % QLatin1String{"?("} % QString::number(oLeft.y()) % QLatin1String{")]"};
		oStream << output;
		return oStream;
	}

	inline bool operator==(PathPoint const& oLeft, PathPoint const& oRight) noexcept
	{
		return (oLeft.x() == oRight.x()) && (oLeft.y() == oRight.y());
	}

	inline bool operator!=(PathPoint const& oLeft, PathPoint const& oRight) noexcept
	{
		return !(oLeft == oRight);
	}

	inline bool operator>(PathPoint const& oLeft, PathPoint const& oRight) noexcept
	{
		return oLeft.getF() > oRight.getF();
	}

	inline bool operator<(PathPoint const& oLeft, PathPoint const& oRight) noexcept
	{
		return oLeft.getF() < oRight.getF();
	}
}
#endif
