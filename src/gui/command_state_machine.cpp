/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#include "fast_sim_design_pch.h"
#include "command_state_machine.h"
#include "util/views_util.h"

namespace FastSimDesign {
	/************************************************************************/
	/* States                                                               */
	/************************************************************************/
	SimulationState::SimulationState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: QState{pParent}
		, m_pSimulation{pSimulation}
	{
	}

	void SimulationState::onEntry(QEvent* pEvent) noexcept
	{
		qInfo().nospace() << "Enter in state: " << metaObject()->className() << ".";
	}

	void SimulationState::onExit(QEvent* pEvent) noexcept
	{
		qInfo().nospace() << "Exit from state: " << metaObject()->className() << ".\n";
	}

	UnloadedState::UnloadedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", false);
	}

	LoadingState::LoadingState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
	}

	void LoadingState::onEntry(QEvent* pEvent) noexcept
	{
		SimulationState::onEntry(pEvent);
		m_pSimulation->onLoad();
	}

	LoadedState::LoadedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", true);
	}

	StartedState::StartedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", false);
	}

	OneTickStartedState::OneTickStartedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", false);
	}

	MultiTickStartedState::MultiTickStartedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", false);
	}

	PausedState::PausedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", true);
	}

	StoppedState::StoppedState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
		assignProperty(pSimulation->m_pUi->loadSimulationAction, "enabled", true);
		assignProperty(pSimulation->m_pUi->startSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->stopSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->pauseSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->oneStickSimulationAction, "enabled", false);
		assignProperty(pSimulation->m_pUi->multiTickSimulationAction, "enabled", false);
	}

	UnloadingState::UnloadingState(SimulatorGui* pSimulation, QState* pParent /*= Q_NULLPTR*/) noexcept
		: SimulationState{pSimulation, pParent}
	{
	}

	void UnloadingState::onEntry(QEvent* pEvent) noexcept
	{
		SimulationState::onEntry(pEvent);
		m_pSimulation->onUnload();
	}

	/************************************************************************/
	/* Transitions                                                          */
	/************************************************************************/
	void SimulationSignalTransition::onTransition(QEvent* pEvent) noexcept
	{
		qInfo().nospace() << "Transition: " << sourceState()->metaObject()->className() << " ---> " << metaObject()->className() << " ---> " << targetState()->metaObject()->className() << ".\n";
	}

	void StartTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onStart();
	}

	void PauseTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onPause();
	}

	void StartOneTickTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onOneTick();
	}

	void StartMultiTickTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onMultiTick();
	}

	void StopTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onStop();
	}

	void StopOneTickTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onStopOneTick();
	}

	void StopMultiTickTransition::onTransition(QEvent* pEvent) noexcept
	{
		SimulationSignalTransition::onTransition(pEvent);
		m_pSimulation->onStopMultiTick();
	}
}