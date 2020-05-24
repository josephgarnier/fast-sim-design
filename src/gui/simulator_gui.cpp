/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "iostream"
#include "QMessageBox"
#include "QCloseEvent"
#include "gui/dock/map/map_scene.h"
#include "command_state_machine.h"
#include "level/world_exception.h"
#include "config/preferences.h"
#include "QLabel"
#include "QElapsedTimer"
#include "QIntValidator"

namespace FastSimDesign {
	/*****************************************************************************
	Slots
	*****************************************************************************/
	void SimulatorGui::onLoad()
	{
		QString worldName = "house";
		try
		{
			// --- Step 1: load and initialize the world ---
			qInfo() << "*** (1/4) Loading world" << worldName << "***";
			m_pWorld = World::load(worldName);
			QObject::connect(m_pWorld.data(), &World::selectedEntityChanged, this, &SimulatorGui::onSelectedEntityChanged);
			qInfo() << "World loaded and initialized.";

			// --- Step 2: initialize the map viewer ---
			qInfo() << "*** (2/4) Initializing map viewer ***";
			MapScene* scene = new MapScene{m_pWorld.data(), m_pWorld->getMapRender(), m_pUi->mapView};
			m_pUi->mapView->setScene(scene);
			m_pUi->mapView->centerOn(0, 0);
			qInfo() << "Map viewer initialized.";

			// --- Step 3: initialize the tick timer ---
			qInfo() << "*** (3/4) Initializing tick timer with" << Time::toMilliseconds(TIME_PER_FRAME) << "ms per tick ***";
			m_pTickTimer.reset(new QTimer{this});
			m_pTickTimer->setSingleShot(false);
			m_pTickTimer->setInterval(Time::toMilliseconds(TIME_PER_FRAME));
			QObject::connect(this, &SimulatorGui::stopTick, m_pTickTimer.data(), &QTimer::stop);
			QObject::connect<void (SimulatorGui::*)(), void (QTimer::*)()>(this, &SimulatorGui::startTick, m_pTickTimer.data(), &QTimer::start);
			QObject::connect(m_pTickTimer.data(), &QTimer::timeout, this, [this]() {
				qInfo() << "***** Previous frame render duration is" << m_pRenderFrameDuration->elapsed() << "ms *****";
				qInfo() << "***** Start frame" << m_pWorld->getWorldInfoModel()->getCurrentTick() << "*****";
				m_pRealFrameDuration->restart();
				emit frameBegin();
				m_pWorld->update(m_oDeltaTimeScaled);
				emit frameEnded();
				qInfo() << "***** Frame duration is" << m_pRealFrameDuration->elapsed() << "ms *****";
				m_pRenderFrameDuration->restart();
			});
			qInfo() << "Tick timer initialized.";

			// --- Step 4: initialize the status in the tool bar ---
			qInfo() << "*** (4/4) Initializing gui status ***";
			m_pWorldInfoView->setOrientation(Qt::Vertical);
			m_pWorldInfoView->setModel(m_pWorld->getWorldInfoModel());
			m_pWorldInfoView->addMapping(m_pCurrentTickValue.data(), toUnderlyingType(WorldInfoModel::Label::CURRENT_TICK), "text");
			m_pWorldInfoView->addMapping(m_pTotalTimeValue.data(), toUnderlyingType(WorldInfoModel::Label::TOTAL_TIME), "text");
			m_pWorldInfoView->addMapping(m_pWorldTimeValue.data(), toUnderlyingType(WorldInfoModel::Label::WORLD_TIME), "text");
			m_pWorldInfoView->toFirst();
			qInfo() << "Gui status initialized.";

			emit simulationLoaded();
		} catch (WorldException& e)
		{
			qCritical() << "WorldException encountered when loading the simulation" << worldName << ":" << e.what() << "!";
			m_pWorld.reset();
			emit simulationLoadingFailed(e.what());
			QMessageBox::critical(this, QStringLiteral("Simulation Loading Error"), e.what());
		}
	}

	void SimulatorGui::onUnload() noexcept
	{
		m_pWorldInfoView->clearMapping();
		m_pTickTimer.reset();
		m_pWorld->term();
		delete m_pUi->emptyExampleView->scene();
		delete m_pUi->mapView->scene();
		m_pWorld.reset();
		emit simulationUnloaded();
	}

	void SimulatorGui::onStart() noexcept
	{
		m_pTickTimer->setSingleShot(false);
		emit startTick();
		emit simulationStarted();
	}

	void SimulatorGui::onPause() noexcept
	{
		emit stopTick();
		emit simulationPaused();
	}

	void SimulatorGui::onOneTick() noexcept
	{
		emit stopTick();
		QSharedPointer<QMetaObject::Connection> connection = QSharedPointer<QMetaObject::Connection>::create();
		*connection = QObject::connect(
			this, &SimulatorGui::frameEnded, this, [this, connection]()
			{
				// control if the tick to reach is achieve at the end of each frame, it stops the timer if currentTick is equals to tick to reach
				emit stopTick();
				emit simulationPaused();
				QObject::disconnect(*connection.data());
			},
			Qt::QueuedConnection);
		m_pTickTimer->setSingleShot(true);
		emit startTick();
		emit simulationStarted();
	}

	void SimulatorGui::onMultiTick() noexcept
	{
		emit stopTick();
		unsigned long int currentTick = m_pWorld->getWorldInfoModel()->getCurrentTick();
		unsigned long int tickToReach = currentTick + m_pMultiTickValue->text().toInt();
		QSharedPointer<QMetaObject::Connection> connection = QSharedPointer<QMetaObject::Connection>::create();
		*connection = QObject::connect(
			this, &SimulatorGui::frameEnded, this, [this, tickToReach, connection]()
			{
				// control if the tick to reach is achieve at the end of each frame, it stops the timer if currentTick is equals to tick to reach
				if (m_pWorld->getWorldInfoModel()->getCurrentTick() >= tickToReach)
				{
					emit stopTick();
					emit simulationPaused();
					QObject::disconnect(*connection.data());
				}
			},
			Qt::QueuedConnection);
		m_pTickTimer->setSingleShot(false);
		emit startTick();
		emit simulationStarted();
	}

	void SimulatorGui::onStop() noexcept
	{
		emit stopTick();
		emit simulationStopped();
	}

	void SimulatorGui::onStopOneTick() noexcept
	{
		emit stopTick();
		QObject::disconnect(this, &SimulatorGui::frameEnded, this, Q_NULLPTR);
		emit simulationStopped();
	}

	void SimulatorGui::onStopMultiTick() noexcept
	{
		emit stopTick();
		QObject::disconnect(this, &SimulatorGui::frameEnded, this, Q_NULLPTR);
		emit simulationStopped();
	}

	void SimulatorGui::onSaveLayout() noexcept
	{
		Preferences& preference = Preferences::instance();
		preference.setUserLayoutGeometry(saveGeometry());
		preference.setUserLayoutState(saveState());
	}

	void SimulatorGui::onRestoreDefaultLayout() noexcept
	{
		Preferences& preference = Preferences::instance();
		restoreGeometry(preference.defaultLayoutGeometry());
		restoreState(preference.defaultLayoutState());
	}

	void SimulatorGui::onTimeScaledOne() noexcept
	{
		int deltaTime = Time::toMilliseconds(TIME_PER_FRAME);
		int scaledDeltaTime = deltaTime * 1.0f;
		m_oDeltaTimeScaled = Time::fromMilliseconds(scaledDeltaTime);
		m_pCurrentSpeedValue->setText(QString::number(1.0f));
	}

	void SimulatorGui::onTimeScaledTwo() noexcept
	{
		int deltaTime = Time::toMilliseconds(TIME_PER_FRAME);
		int scaledDeltaTime = deltaTime * 2.0f;
		m_oDeltaTimeScaled = Time::fromMilliseconds(scaledDeltaTime);
		m_pCurrentSpeedValue->setText(QString::number(2.0f));
	}

	void SimulatorGui::onTimeScaledFive() noexcept
	{
		int deltaTime = Time::toMilliseconds(TIME_PER_FRAME);
		int scaledDeltaTime = deltaTime * 5.0f;
		m_oDeltaTimeScaled = Time::fromMilliseconds(scaledDeltaTime);
		m_pCurrentSpeedValue->setText(QString::number(5.0f));
	}

	void SimulatorGui::onSelectedEntityChanged(Entity const& oSelectedEntity) noexcept
	{
	}

	/*****************************************************************************
	Methods
	*****************************************************************************/
	SimulatorGui::SimulatorGui(QWidget* pParent /*= 0*/) noexcept
		: QMainWindow{pParent}
		, m_oDeltaTimeScaled{TIME_PER_FRAME}
		, m_pUi{new Ui::SimulatorGui{}}
		, m_pMultiTickValue{new QLineEdit{this}}
		, m_pTimeScaleLabel{new QLabel{this}}
		, m_pTimeScaleOneAction{new QAction{this}}
		, m_pTimeScaleTwoAction{new QAction{this}}
		, m_pTimeScaleFiveAction{new QAction{this}}
		, m_pCurrentSpeedLabel{new QLabel{this}}
		, m_pCurrentSpeedValue{new QLabel{this}}
		, m_pCurrentTickLabel{new QLabel{this}}
		, m_pCurrentTickValue{new QLabel{this}}
		, m_pFixedFpsLabel{new QLabel{this}}
		, m_pFixedFpsValue{new QLabel{this}}
		, m_pTotalTimeLabel{new QLabel{this}}
		, m_pTotalTimeValue{new QLabel{this}}
		, m_pWorldTimeLabel{new QLabel{this}}
		, m_pWorldTimeValue{new QLabel{this}}
		, m_pWorldInfoView{new QDataWidgetMapper{this}}
		, m_pTickTimer{Q_NULLPTR}
		, m_pRealFrameDuration{new QElapsedTimer{}}
		, m_pRenderFrameDuration{new QElapsedTimer{}}
		, m_pWorld{Q_NULLPTR}
		, m_pSimulationMachine{new QStateMachine{this}}
	{
		qInfo() << "Creating SimulatorGui...";
		setupUi();
		setupToolbar();
		setupMenuBar();
		setupModel();
		setupStateMachine();
		qInfo() << "SimulatorGui created successfully!\n";
	}

	void SimulatorGui::setupUi() noexcept
	{
		m_pUi->setupUi(this);
	}

	void SimulatorGui::setupToolbar() noexcept
	{
		m_pMultiTickValue->setValidator(new QIntValidator{1, 100, this});
		m_pMultiTickValue->setMaxLength(3);
		m_pMultiTickValue->setMaximumWidth(40);
		m_pMultiTickValue->setFixedWidth(40);
		m_pMultiTickValue->insert(QStringLiteral("1"));
		m_pMultiTickValue->setContentsMargins(0, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pMultiTickValue.data());

		m_pUi->toolBar->addSeparator();

		m_pTimeScaleLabel->setText(QStringLiteral("Speed:"));
		m_pTimeScaleLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pTimeScaleLabel.data());

		m_pTimeScaleOneAction->setText(QApplication::translate("SimulatorGui", "x1", Q_NULLPTR));
		m_pTimeScaleOneAction->setObjectName(QStringLiteral("timeScaleAction"));
		m_pTimeScaleOneAction->setToolTip(QApplication::translate("SimulatorGui", "Speed x1", Q_NULLPTR));
		m_pUi->toolBar->addAction(m_pTimeScaleOneAction.data());
		QObject::connect(m_pTimeScaleOneAction.data(), &QAction::triggered, this, &SimulatorGui::onTimeScaledOne);

		m_pTimeScaleTwoAction->setText(QApplication::translate("SimulatorGui", "x2", Q_NULLPTR));
		m_pTimeScaleTwoAction->setObjectName(QStringLiteral("timeScaleAction"));
		m_pTimeScaleTwoAction->setToolTip(QApplication::translate("SimulatorGui", "Speed x2", Q_NULLPTR));
		m_pUi->toolBar->addAction(m_pTimeScaleTwoAction.data());
		QObject::connect(m_pTimeScaleTwoAction.data(), &QAction::triggered, this, &SimulatorGui::onTimeScaledTwo);

		m_pTimeScaleFiveAction->setText(QApplication::translate("SimulatorGui", "x5", Q_NULLPTR));
		m_pTimeScaleFiveAction->setObjectName(QStringLiteral("timeScaleAction"));
		m_pTimeScaleFiveAction->setToolTip(QApplication::translate("SimulatorGui", "Speed x5", Q_NULLPTR));
		m_pUi->toolBar->addAction(m_pTimeScaleFiveAction.data());
		QObject::connect(m_pTimeScaleFiveAction.data(), &QAction::triggered, this, &SimulatorGui::onTimeScaledFive);

		m_pUi->toolBar->addSeparator();

		m_pCurrentSpeedLabel->setText(QStringLiteral("Current speed:"));
		m_pCurrentSpeedLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pCurrentSpeedLabel.data());
		m_pCurrentSpeedValue->setText(QString::number(1.0f));
		m_pUi->toolBar->addWidget(m_pCurrentSpeedValue.data());

		m_pCurrentTickLabel->setText(QStringLiteral("Current tick:"));
		m_pCurrentTickLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pCurrentTickLabel.data());
		m_pCurrentTickValue->setText(QString::number(0));
		m_pUi->toolBar->addWidget(m_pCurrentTickValue.data());

		m_pFixedFpsLabel->setText(QStringLiteral("Fixed FPS:"));
		m_pFixedFpsLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pFixedFpsLabel.data());
		m_pFixedFpsValue->setText(QString::number(Time::toMilliseconds(TIME_PER_FRAME)) % " ms");
		m_pUi->toolBar->addWidget(m_pFixedFpsValue.data());

		m_pTotalTimeLabel->setText(QStringLiteral("Total time:"));
		m_pTotalTimeLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pTotalTimeLabel.data());
		m_pTotalTimeValue->setText(QString::number(0));
		m_pUi->toolBar->addWidget(m_pTotalTimeValue.data());

		m_pWorldTimeLabel->setText(QStringLiteral("World time:"));
		m_pWorldTimeLabel->setContentsMargins(12, 0, 3, 0);
		m_pUi->toolBar->addWidget(m_pWorldTimeLabel.data());
		m_pWorldTimeValue->setText(QString::number(0));
		m_pUi->toolBar->addWidget(m_pWorldTimeValue.data());
	}

	void SimulatorGui::setupMenuBar() noexcept
	{
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->treeExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->graphicExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->tabsExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->tableExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->emptyExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertAction(m_pUi->saveLayoutAction, m_pUi->timelineExampleDock->toggleViewAction());
		m_pUi->viewMenu->insertSeparator(m_pUi->saveLayoutAction);
	}

	void SimulatorGui::setupModel() noexcept
	{
		// --- Retrieve preferences ---
		Preferences& preference = Preferences::instance();
		m_pUi->showGridAction->setChecked(preference.showGrid());
		m_pUi->showCollisionLayerAction->setChecked(preference.showCollisionLayer());
		m_pUi->showEntityLabelsAction->setChecked(preference.showEntityLabels());

		preference.setDefaultLayoutGeometry(saveGeometry());
		preference.setDefaultLayoutState(saveState());
		restoreGeometry(preference.userLayoutGeometry());
		restoreState(preference.userLayoutState());

		// --- Connect models ---
		QObject::connect(m_pUi->showGridAction, &QAction::toggled, &preference, &Preferences::setShowGrid);
		QObject::connect(m_pUi->showCollisionLayerAction, &QAction::toggled, &preference, &Preferences::setShowCollisionLayer);
		QObject::connect(m_pUi->showEntityLabelsAction, &QAction::toggled, &preference, &Preferences::setShowEntityLabels);
		QObject::connect(m_pUi->defaultLayoutAction, &QAction::triggered, this, &SimulatorGui::onRestoreDefaultLayout);
		QObject::connect(m_pUi->saveLayoutAction, &QAction::triggered, this, &SimulatorGui::onSaveLayout);
	}

	void SimulatorGui::setupStateMachine() noexcept
	{
		QState* unloaded = new UnloadedState{this, m_pSimulationMachine.data()};
		QState* loading = new LoadingState{this, m_pSimulationMachine.data()};
		QState* loaded = new LoadedState{this, m_pSimulationMachine.data()};
		QState* started = new StartedState{this, m_pSimulationMachine.data()};
		QState* oneTickStarted = new OneTickStartedState{this, m_pSimulationMachine.data()};
		QState* multiTickStarted = new MultiTickStartedState{this, m_pSimulationMachine.data()};
		QState* paused = new PausedState{this, m_pSimulationMachine.data()};
		QState* stopped = new StoppedState{this, m_pSimulationMachine.data()};
		QState* unloading = new UnloadingState{this, m_pSimulationMachine.data()};
		QFinalState* end = new QFinalState{m_pSimulationMachine.data()};
		m_pSimulationMachine->setInitialState(unloaded);

		unloaded->addTransition(new LoadTransition{this, m_pUi->loadSimulationAction, &QAction::triggered, unloaded, loading});
		unloaded->addTransition(new EndSimulationTransition{this, this, &SimulatorGui::endSimulationRequested, unloaded, end});
		loading->addTransition(new LoadingFailedTransition{this, this, &SimulatorGui::simulationLoadingFailed, loading, unloaded});
		loading->addTransition(new LoadingSuccessTransition{this, this, &SimulatorGui::simulationLoaded, loading, loaded});
		loaded->addTransition(new EndSimulationTransition{this, this, &SimulatorGui::endSimulationRequested, loaded, unloading});
		loaded->addTransition(new LoadTransition{this, m_pUi->loadSimulationAction, &QAction::triggered, loaded, unloading});
		loaded->addTransition(new StartTransition{this, m_pUi->startSimulationAction, &QAction::triggered, loaded, started});
		loaded->addTransition(new StartOneTickTransition{this, m_pUi->oneStickSimulationAction, &QAction::triggered, loaded, oneTickStarted});
		loaded->addTransition(new StartMultiTickTransition{this, m_pUi->multiTickSimulationAction, &QAction::triggered, loaded, multiTickStarted});
		oneTickStarted->addTransition(new PauseTransition{this, this, &SimulatorGui::simulationPaused, oneTickStarted, paused});
		oneTickStarted->addTransition(new StopOneTickTransition{this, this, &SimulatorGui::endSimulationRequested, oneTickStarted, stopped});
		multiTickStarted->addTransition(new PauseTransition{this, this, &SimulatorGui::simulationPaused, multiTickStarted, paused});
		multiTickStarted->addTransition(new StopMultiTickTransition{this, m_pUi->stopSimulationAction, &QAction::triggered, multiTickStarted, stopped});
		multiTickStarted->addTransition(new StopMultiTickTransition{this, this, &SimulatorGui::endSimulationRequested, multiTickStarted, stopped});
		started->addTransition(new PauseTransition{this, m_pUi->pauseSimulationAction, &QAction::triggered, started, paused});
		started->addTransition(new StopTransition{this, m_pUi->stopSimulationAction, &QAction::triggered, started, stopped});
		started->addTransition(new StopTransition{this, this, &SimulatorGui::endSimulationRequested, started, stopped});
		paused->addTransition(new StartOneTickTransition{this, m_pUi->oneStickSimulationAction, &QAction::triggered, paused, oneTickStarted});
		paused->addTransition(new StartMultiTickTransition{this, m_pUi->multiTickSimulationAction, &QAction::triggered, paused, multiTickStarted});
		paused->addTransition(new StartTransition{this, m_pUi->startSimulationAction, &QAction::triggered, paused, started});
		paused->addTransition(new StopTransition{this, m_pUi->stopSimulationAction, &QAction::triggered, paused, stopped});
		paused->addTransition(new StopTransition{this, this, &SimulatorGui::endSimulationRequested, paused, stopped});
		stopped->addTransition(new LoadTransition{this, m_pUi->loadSimulationAction, &QAction::triggered, stopped, unloading});
		stopped->addTransition(new EndSimulationTransition{this, this, &SimulatorGui::endSimulationRequested, stopped, unloading});
		unloading->addTransition(new UnloadingSuccessTransition{this, this, &SimulatorGui::simulationUnloaded, unloading, unloaded});

		qInfo() << "State machine initialized.";
		m_pSimulationMachine->start();
		qInfo() << "State machine started.";
	}

	void SimulatorGui::closeEvent(QCloseEvent* pEvent) noexcept
	{
		QMainWindow::closeEvent(pEvent);
		QMessageBox::StandardButton closeButton = QMessageBox::question(this, QMainWindow::windowTitle(), QStringLiteral("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
		if (closeButton != QMessageBox::Yes)
		{
			pEvent->ignore();
		} else
		{
			do
			{
				emit endSimulationRequested();
			} while (m_pSimulationMachine->isRunning());
			pEvent->accept();
		}
	}
}
