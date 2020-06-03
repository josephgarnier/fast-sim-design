/******************************************************************************
* Copyright 2019-present, Joseph Garnier
* All rights reserved.
*
* This source code is licensed under the license found in the
* LICENSE file in the root directory of this source tree.
******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_COMMAND_STATE_MACHINE_H
#define FAST_SIM_DESIGN_COMMAND_STATE_MACHINE_H

#include "QState"
#include "QSignalTransition"
#include "QFinalState"

namespace FastSimDesign {
	/*****************************************************************************
	States
	*****************************************************************************/
	class SimulatorGui;
	class SimulationState : public QState
	{
		Q_OBJECT
	public:
		explicit SimulationState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
		virtual void onEntry(QEvent* pEvent) noexcept override;
		virtual void onExit(QEvent* pEvent) noexcept override;

	protected:
		SimulatorGui* m_pSimulation;

	private:
	};

	class UnloadedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit UnloadedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class LoadingState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit LoadingState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
		virtual void onEntry(QEvent* pEvent) noexcept override;

	private:
	};

	class LoadedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit LoadedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class StartedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit StartedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class OneTickStartedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit OneTickStartedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class MultiTickStartedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit MultiTickStartedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class PausedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit PausedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class StoppedState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit StoppedState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
	private:
	};

	class UnloadingState final : public SimulationState
	{
		Q_OBJECT
	public:
		explicit UnloadingState(SimulatorGui* pSimulation, QState* pParent = Q_NULLPTR) noexcept;

	protected:
		virtual void onEntry(QEvent* pEvent) noexcept override;

	private:
	};

	/*****************************************************************************
	Transitions
	*****************************************************************************/
	class SimulationSignalTransition : public QSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit SimulationSignalTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: QSignalTransition{pTrigger, std::forward<Func>(oSignal), pSourceState}
			, m_pSimulation{pSimulation}
		{
			setTargetState(pTargetState);
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	protected:
		SimulatorGui* m_pSimulation;

	private:
	};

	class LoadTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit LoadTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
	private:
	};

	class LoadingFailedTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit LoadingFailedTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
	private:
	};

	class LoadingSuccessTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit LoadingSuccessTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
	private:
	};

	class StartTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StartTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class PauseTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit PauseTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class StartOneTickTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StartOneTickTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class StartMultiTickTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StartMultiTickTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class StopTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StopTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class StopOneTickTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StopOneTickTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class StopMultiTickTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit StopMultiTickTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
		virtual void onTransition(QEvent* pEvent) noexcept override;

	private:
	};

	class EndSimulationTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit EndSimulationTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
	private:
	};

	class UnloadingSuccessTransition final : public SimulationSignalTransition
	{
		Q_OBJECT
	public:
		template<typename Func>
		explicit UnloadingSuccessTransition(SimulatorGui* pSimulation, typename QtPrivate::FunctionPointer<Func>::Object const* pTrigger, Func oSignal, QState* pSourceState, QAbstractState* pTargetState) noexcept
			: SimulationSignalTransition{pSimulation, pTrigger, std::forward<Func>(oSignal), pSourceState, pTargetState}
		{
		}

	protected:
	private:
	};
}
#endif
