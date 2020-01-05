/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "preferences.h"

namespace FastSimDesign {
	/*****************************************************************************
	Slots
	*****************************************************************************/
	void Preferences::setShowGrid(bool bVisible) noexcept
	{
		if (m_bShowGrid != bVisible)
		{
			m_bShowGrid = bVisible;
			m_oSettings->setValue(QLatin1String{"Interface/ShowGrid"}, bVisible);
			emit showGridChanged(bVisible);
		}
	}

	void Preferences::setShowCollisionLayer(bool bVisible) noexcept
	{
		if (m_bShowCollisionLayer != bVisible)
		{
			m_bShowCollisionLayer = bVisible;
			m_oSettings->setValue(QLatin1String{"Interface/ShowCollisionLayer"}, bVisible);
			emit showCollisionLayerChanged(bVisible);
		}
	}

	void Preferences::setShowEntityLabels(bool bVisible) noexcept
	{
		if (m_bShowEntityLabels != bVisible)
		{
			m_bShowEntityLabels = bVisible;
			m_oSettings->setValue(QLatin1String{"Interface/ShowEntityLabels"}, bVisible);
			emit showEntityLabelsChanged(bVisible);
		}
	}

	void Preferences::setDefaultLayoutGeometry(QByteArray const& oLayoutGeometry) noexcept
	{
		if (m_oDefaultLayoutGeometry != oLayoutGeometry)
		{
			m_oDefaultLayoutGeometry = oLayoutGeometry;
			m_oSettings->setValue(QLatin1String{"Interface/DefaultGeometry"}, oLayoutGeometry);
			emit layoutDefaultGeometryChanged(oLayoutGeometry);
		}
	}

	void Preferences::setUserLayoutGeometry(QByteArray const& oLayoutGeometry) noexcept
	{
		if (m_oUserLayoutGeometry != oLayoutGeometry)
		{
			m_oUserLayoutGeometry = oLayoutGeometry;
			m_oSettings->setValue(QLatin1String{"Interface/UserGeometry"}, oLayoutGeometry);
			emit layoutUserGeometryChanged(oLayoutGeometry);
		}
	}

	void Preferences::setDefaultLayoutState(QByteArray const& oLayoutState) noexcept
	{
		if (m_oDefaultLayoutState != oLayoutState)
		{
			m_oDefaultLayoutState = oLayoutState;
			m_oSettings->setValue(QLatin1String{"Interface/DefaultState"}, oLayoutState);
			emit layoutDefaultStateChanged(oLayoutState);
		}
	}

	void Preferences::setUserLayoutState(QByteArray const& oLayoutState) noexcept
	{
		if (m_oUserLayoutState != oLayoutState)
		{
			m_oUserLayoutState = oLayoutState;
			m_oSettings->setValue(QLatin1String{"Interface/UserState"}, oLayoutState);
			emit layoutUserStateChanged(oLayoutState);
		}
	}

	/*****************************************************************************
	Statics
	*****************************************************************************/
	Preferences& Preferences::instance() noexcept
	{
		static Preferences instance{};
		return instance;
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	Preferences::Preferences(QObject* pParent /*= Q_NULLPTR*/) noexcept
		: QObject{pParent}
		, m_oSettings{new QSettings{QSettings::IniFormat, QSettings::UserScope, QLatin1String{"Artefacts Studio"}, QLatin1String{"ExperimentationTool"}, this}}
	{
		// --- Retrieve interface settings ---
		m_oSettings->beginGroup(QLatin1String{"Interface"});
		m_bShowGrid = valueOf(QLatin1String{"ShowGrid"}, false);
		m_bShowCollisionLayer = valueOf(QLatin1String{"ShowCollisionLayer"}, false);
		m_bShowEntityLabels = valueOf(QLatin1String{"ShowEntityLabels"}, false);
		m_oDefaultLayoutGeometry = valueOf(QLatin1String{"DefaultGeometry"}, QByteArray{});
		m_oUserLayoutGeometry = valueOf(QLatin1String{"UserGeometry"}, QByteArray{});
		m_oDefaultLayoutState = valueOf(QLatin1String{"DefaultState"}, QByteArray{});
		m_oUserLayoutState = valueOf(QLatin1String{"UserState"}, QByteArray{});
		m_oSettings->endGroup();
	}
}