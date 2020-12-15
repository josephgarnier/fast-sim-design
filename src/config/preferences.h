/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_PREFERENCES_H
#define FAST_SIM_DESIGN_PREFERENCES_H

#include "QObject"
#include "QSettings"

namespace FastSimDesign {
	class Preferences : public QObject
	{
		Q_OBJECT
	public Q_SLOTS:
		void setShowGrid(bool bVisible) noexcept;
		void setShowCollisionLayer(bool bVisible) noexcept;
		void setShowEntityLabels(bool bVisible) noexcept;
		void setDefaultLayoutGeometry(QByteArray const& oLayoutGeometry) noexcept;
		void setUserLayoutGeometry(QByteArray const& oLayoutGeometry) noexcept;
		void setDefaultLayoutState(QByteArray const& oLayoutState) noexcept;
		void setUserLayoutState(QByteArray const& oLayoutState) noexcept;
	Q_SIGNALS:
		void showGridChanged(bool bVisible);
		void showCollisionLayerChanged(bool bVisible);
		void showEntityLabelsChanged(bool bVisible);
		void layoutDefaultGeometryChanged(QByteArray const& oLayoutGeometry);
		void layoutUserGeometryChanged(QByteArray const& oLayoutGeometry);
		void layoutDefaultStateChanged(QByteArray const& oLayoutState);
		void layoutUserStateChanged(QByteArray const& oLayoutState);

	public:
		static Preferences& instance() noexcept;

	public:
		inline bool showGrid() const noexcept { return m_bShowGrid; }
		inline bool showCollisionLayer() const noexcept { return m_bShowCollisionLayer; }
		inline bool showEntityLabels() const noexcept { return m_bShowEntityLabels; }
		inline QByteArray const& defaultLayoutGeometry() const noexcept { return m_oDefaultLayoutGeometry; }
		inline QByteArray const& userLayoutGeometry() const noexcept { return m_oUserLayoutGeometry; }
		inline QByteArray const& defaultLayoutState() const noexcept { return m_oDefaultLayoutState; }
		inline QByteArray const& userLayoutState() const noexcept { return m_oUserLayoutState; }

	protected:
		template<typename T>
		T valueOf(QString sKey, T&& oDefaultValue) const noexcept
		{
			return m_oSettings->value(std::move(sKey), std::forward<T>(oDefaultValue)).template value<T>();
		}

	private:
		explicit Preferences(QObject* pParent = nullptr) noexcept;
		virtual ~Preferences() = default; // Destructor

	protected:
		QScopedPointer<QSettings> m_oSettings;

		bool m_bShowGrid;
		bool m_bShowCollisionLayer;
		bool m_bShowEntityLabels;
		QByteArray m_oDefaultLayoutGeometry;
		QByteArray m_oUserLayoutGeometry;
		QByteArray m_oDefaultLayoutState;
		QByteArray m_oUserLayoutState;

	private:
	};
}
#endif
