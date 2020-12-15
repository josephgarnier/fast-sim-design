/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_WORLD_INFO_MODEL_H
#define FAST_SIM_DESIGN_WORLD_INFO_MODEL_H

#include "util/algorithm_util.h"
#include "QAbstractListModel"

namespace FastSimDesign {
	class WorldInfoModel final : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class Label : int
		{
			TOTAL_TIME = 0,
			WORLD_TIME,
			CURRENT_TICK,

			LABEL_COUNT,
			LABEL_FIRST = TOTAL_TIME,
			LABEL_LAST = CURRENT_TICK
		};

		explicit WorldInfoModel(QObject* pParent = nullptr) noexcept; // Default constructor
		virtual ~WorldInfoModel() = default; // Destructor

		virtual int rowCount(QModelIndex const& oParent = QModelIndex{}) const noexcept override;
		virtual int columnCount(QModelIndex const& oParent = QModelIndex{}) const noexcept override;
		virtual QVariant headerData(int iSection, Qt::Orientation oOrientation, int iRole = Qt::DisplayRole) const noexcept override;
		virtual QVariant data(QModelIndex const& oIndex, int iRole = Qt::DisplayRole) const noexcept override;

		QVariant valueOf(WorldInfoModel::Label const& oLabel) const noexcept;

		void setTotalTime(QTime oTotalTime) noexcept;
		inline QTime getTotalTime() const noexcept { return m_oTotalTime; }

		void setWorldTime(QTime oWorldTime) noexcept;
		inline QTime const& getWorldTime() const noexcept { return m_oWorldTime; }

		void setCurrentTick(unsigned long int lCurrentTick) noexcept;
		inline unsigned long int getCurrentTick() const noexcept { return m_lCurrentTick; }
		void incrementCurrentTick() noexcept;

	protected:
	private:
		QTime m_oTotalTime;
		QTime m_oWorldTime;
		unsigned long int m_lCurrentTick;
	};
}
#endif
