/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "world_info_model.h"
#include "QFont"

namespace FastSimDesign {
	WorldInfoModel::WorldInfoModel(QObject* pParent /*= Q_NULLPTR*/) noexcept
		: QAbstractListModel{pParent}
		, m_oTotalTime{}
		, m_oWorldTime{}
		, m_lCurrentTick{0}
	{
	}

	int WorldInfoModel::rowCount(QModelIndex const& oParent /*= QModelIndex()*/) const noexcept
	{
		if (oParent.isValid())
			return 0;
		return toUnderlyingType(WorldInfoModel::Label::LABEL_COUNT);
	}

	int WorldInfoModel::columnCount(QModelIndex const& oParent /*= QModelIndex()*/) const noexcept
	{
		if (oParent.isValid())
			return 0;
		return 1;
	}

	QVariant WorldInfoModel::headerData(int iSection, Qt::Orientation oOrientation, int iRole /*= Qt::DisplayRole*/) const noexcept
	{
		switch (iRole)
		{
			case Qt::DisplayRole:
				if (oOrientation == Qt::Horizontal)
				{
					WorldInfoModel::Label label = static_cast<WorldInfoModel::Label>(iSection);
					switch (label)
					{
						case WorldInfoModel::Label::TOTAL_TIME:
							return QLatin1String{"Total time"};
						case WorldInfoModel::Label::WORLD_TIME:
							return QLatin1String{"World time"};
						case WorldInfoModel::Label::CURRENT_TICK:
							return QLatin1String{"Current tick"};
						default:
							return QVariant{};
					}
				}
				break;
			case Qt::FontRole:
			{
				QFont boldFont{};
				boldFont.setBold(true);
				return boldFont;
			}
			break;
			case Qt::TextAlignmentRole:
				return Qt::AlignCenter;
				break;
		}
		return QVariant{};
	}

	QVariant WorldInfoModel::data(QModelIndex const& oIndex, int iRole /*= Qt::DisplayRole*/) const noexcept
	{
		int row = oIndex.row();
		int col = oIndex.column();
		if (!oIndex.isValid())
			return QVariant{};

		switch (iRole)
		{
			case Qt::EditRole:
			case Qt::DisplayRole:
				return valueOf(static_cast<WorldInfoModel::Label>(row));
				break;
		}
		return QVariant{};
	}

	QVariant WorldInfoModel::valueOf(WorldInfoModel::Label const& oLabel) const noexcept
	{
		switch (oLabel)
		{
			case WorldInfoModel::Label::TOTAL_TIME:
				return m_oTotalTime.toString("hh:mm:ss''zzz");
			case WorldInfoModel::Label::WORLD_TIME:
				return m_oWorldTime.toString("hh:mm:ss''zzz");
			case WorldInfoModel::Label::CURRENT_TICK:
				return static_cast<qulonglong>(m_lCurrentTick);
			default:
				break;
		}
		return QVariant{};
	}

	void WorldInfoModel::setTotalTime(QTime oTotalTime) noexcept
	{
		m_oTotalTime = std::move(oTotalTime);
		QModelIndex infoIndex = index(toUnderlyingType(WorldInfoModel::Label::TOTAL_TIME), 0, QModelIndex{});
		emit dataChanged(infoIndex, infoIndex, {Qt::DisplayRole});
	}

	void WorldInfoModel::setWorldTime(QTime oWorldTime) noexcept
	{
		m_oWorldTime = std::move(oWorldTime);
		QModelIndex infoIndex = index(toUnderlyingType(WorldInfoModel::Label::WORLD_TIME), 0, QModelIndex{});
		emit dataChanged(infoIndex, infoIndex, {Qt::DisplayRole});
	}

	void WorldInfoModel::setCurrentTick(unsigned long int lCurrentTick) noexcept
	{
		m_lCurrentTick = lCurrentTick;
		QModelIndex infoIndex = index(toUnderlyingType(WorldInfoModel::Label::CURRENT_TICK), 0, QModelIndex{});
		emit dataChanged(infoIndex, infoIndex, {Qt::DisplayRole});
	}

	void WorldInfoModel::incrementCurrentTick() noexcept
	{
		unsigned long int nextCurrentTick = getCurrentTick() + 1;
		setCurrentTick(nextCurrentTick);
	}

}
