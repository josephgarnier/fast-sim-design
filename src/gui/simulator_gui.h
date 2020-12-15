/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_SIMULATOR_GUI_H
#define FAST_SIM_DESIGN_SIMULATOR_GUI_H

#include "QMainWindow"
#include "QElapsedTimer"
#include "QTimer"
#include "QStateMachine"
#include "QDataWidgetMapper"
#include "QLabel"
#include "QLineEdit"

#include "ui_simulator_gui.h"
#include "util/algorithm_util.h"

namespace FastSimDesign {
	class Entity;
	class World;
	class SimulatorGui : public QMainWindow
	{
		Q_OBJECT
	private Q_SLOTS:
		void onLoad();
		void onUnload() noexcept;
		void onStart() noexcept;
		void onPause() noexcept;
		void onOneTick() noexcept;
		void onMultiTick() noexcept;
		void onStop() noexcept;
		void onStopOneTick() noexcept;
		void onStopMultiTick() noexcept;

		void onSaveLayout() noexcept;
		void onRestoreDefaultLayout() noexcept;

		void onTimeScaledOne() noexcept;
		void onTimeScaledTwo() noexcept;
		void onTimeScaledFive() noexcept;

		void onSelectedEntityChanged(Entity const& oSelectedEntity) noexcept;
	Q_SIGNALS:
		// --- These signals are used by the state machine ---
		void endSimulationRequested();
		void simulationUnloaded();
		void simulationLoadingFailed(QString const& oMessage);
		void simulationLoaded();
		void simulationStarted();
		void simulationPaused();
		void simulationStopped();

		void startTick();
		void stopTick();

		void frameBegin();
		void frameEnded();

	public:
		explicit SimulatorGui(QWidget* pParent = nullptr) noexcept;
		virtual ~SimulatorGui() = default; // Destructor

	protected:
		friend class UnloadedState;
		friend class LoadingState;
		friend class LoadedState;
		friend class StartedState;
		friend class OneTickStartedState;
		friend class MultiTickStartedState;
		friend class PausedState;
		friend class StoppedState;
		friend class UnloadingState;
		friend class LoadTransition;
		friend class StartTransition;
		friend class PauseTransition;
		friend class StartOneTickTransition;
		friend class StartMultiTickTransition;
		friend class StopTransition;
		friend class StopOneTickTransition;
		friend class StopMultiTickTransition;

	private:
		void setupUi() noexcept;
		void setupToolbar() noexcept;
		void setupMenuBar() noexcept;
		void setupModels() noexcept;
		void setupStateMachine() noexcept;

		virtual void closeEvent(QCloseEvent* pEvent) noexcept override;

	protected:
		int const FRAME_PER_SECONDS = 30;
		QTime const TIME_PER_FRAME = Time::fromSeconds(1.0f / FRAME_PER_SECONDS);
		QTime m_oDeltaTimeScaled;

		Ui::SimulatorGui* m_pUi;
		QScopedPointer<QLineEdit> m_pMultiTickValue;
		QScopedPointer<QLabel> m_pTimeScaleLabel;
		QScopedPointer<QAction> m_pTimeScaleOneAction;
		QScopedPointer<QAction> m_pTimeScaleTwoAction;
		QScopedPointer<QAction> m_pTimeScaleFiveAction;
		QScopedPointer<QLabel> m_pCurrentSpeedLabel;
		QScopedPointer<QLabel> m_pCurrentSpeedValue;
		QScopedPointer<QLabel> m_pCurrentTickLabel;
		QScopedPointer<QLabel> m_pCurrentTickValue;
		QScopedPointer<QLabel> m_pFixedFpsLabel;
		QScopedPointer<QLabel> m_pFixedFpsValue;
		QScopedPointer<QLabel> m_pTotalTimeLabel;
		QScopedPointer<QLabel> m_pTotalTimeValue;
		QScopedPointer<QLabel> m_pWorldTimeLabel;
		QScopedPointer<QLabel> m_pWorldTimeValue;

		QScopedPointer<QDataWidgetMapper> m_pWorldInfoView;
		QScopedPointer<QTimer> m_pTickTimer;
		QScopedPointer<QElapsedTimer> m_pRealFrameDuration;
		QScopedPointer<QElapsedTimer> m_pRenderFrameDuration;
		QSharedPointer<World> m_pWorld;
		QScopedPointer<QStateMachine> m_pSimulationMachine;
	};
}
#endif
