#include "ui_frontend.h"

#include "Platform/MemUtils.h"

// SDK
#include "main/portlockmanager.h"

// C++
#include <assert.h>

namespace EARS::Apt
{
	// s_aStateData: 38 twelve-byte entries of const data, indexed by the
	// FrontendState value rather than by a dense list, so entry 0 is the all-zero
	// placeholder for NONE.
	static const uintptr_t ADDR_S_A_STATE_DATA = 0x00D89448;

	static const uintptr_t ADDR_S_E_NEXT_STATE = 0x00E5672C;
	static const uintptr_t ADDR_S_E_CURRENT_STATE = 0x01130548;
	static const uintptr_t ADDR_S_FLAGS = 0x01130590;

	static const uintptr_t ADDR_INSTANCE = 0x01129928;

	static const uintptr_t ADDR_ADD_NEXT_STATE_SEQUENCE = 0x00931F80;
	static const uintptr_t ADDR_START_NEW_GAME = 0x009315A0;
	static const uintptr_t ADDR_START_BUTTON_PRESSED = 0x00931130;
	static const uintptr_t ADDR_RESTART_FE_LOOP_TO_STATE = 0x009316E0;

	// The mod's copy of the image's table, and the entry functions it held before
	// any PatchState call.
	static UIFrontend::StateData s_StateData[UIFrontend::NUM_STATES];
	static UIFrontend::StateFunc s_OriginalStateFuncs[UIFrontend::NUM_STATES];

	void UIFrontend::ExecuteNextState()
	{
		Flags32& Flags = GetFlags();
		if (Flags.Test((uint32_t)UIFrontendFlags::FE_FLAG_BLOCK_STATE_CHANGES))
		{
			return;
		}

		const FrontendState NextState = GetNextState();
		assert((uint32_t)NextState < NUM_STATES);

		const FrontendState PreviousState = GetCurrentState();
		SetCurrentState(NextState);

		const StateData& State = s_StateData[(uint32_t)NextState];

		EARS::MainLoop::PortLockManager* PortLockMgr = EARS::MainLoop::PortLockManager::GetInstance();
		PortLockMgr->SetIsDialogAllowed((State.m_Flags & (uint32_t)FrontendStateFlags::FE_STATE_FLAG_NO_PORT_DIALOGS) == 0);

		State.m_Func(this, PreviousState);
	}

	void UIFrontend::AddNextStateSequence(int32_t InDelayTimeMS)
	{
		MemUtils::CallClassMethod<void, UIFrontend*, int32_t>(ADDR_ADD_NEXT_STATE_SEQUENCE, this, InDelayTimeMS);
	}

	void UIFrontend::StartNewGame()
	{
		MemUtils::CallClassMethod<void, UIFrontend*>(ADDR_START_NEW_GAME, this);
	}

	void UIFrontend::StartButtonPressed()
	{
		MemUtils::CallCdeclMethod<void>(ADDR_START_BUTTON_PRESSED);
	}

	void UIFrontend::RestartFrontEndLoopToState(FrontendState InState)
	{
		MemUtils::CallClassMethod<void, UIFrontend*, FrontendState>(ADDR_RESTART_FE_LOOP_TO_STATE, this, InState);
	}

	void UIFrontend::EnterState_StartNewGame(EARS::Apt::UIFrontend* InThis, FrontendState InPreviousState)
	{
		assert(InThis);
		InThis->StartNewGame();
	}

	void UIFrontend::SetNextState(FrontendState InState)
	{
		*(FrontendState*)ADDR_S_E_NEXT_STATE = InState;
	}

	UIFrontend::FrontendState UIFrontend::GetNextState()
	{
		return *(FrontendState*)ADDR_S_E_NEXT_STATE;
	}

	UIFrontend::FrontendState UIFrontend::GetCurrentState()
	{
		return *(FrontendState*)ADDR_S_E_CURRENT_STATE;
	}

	Flags32& UIFrontend::GetFlags()
	{
		return *(Flags32*)ADDR_S_FLAGS;
	}

	UIFrontend* UIFrontend::GetInstance()
	{
		return *(UIFrontend**)ADDR_INSTANCE;
	}

	void UIFrontend::PatchState(FrontendState InState, StateFunc InFunction)
	{
		assert((uint32_t)InState < NUM_STATES);
		s_StateData[(uint32_t)InState].m_Func = InFunction;
	}

	UIFrontend::StateFunc UIFrontend::GetOriginalStateFunc(FrontendState InState)
	{
		assert((uint32_t)InState < NUM_STATES);
		return s_OriginalStateFuncs[(uint32_t)InState];
	}

	void UIFrontend::CopyStates()
	{
		const StateData* GameStates = (const StateData*)ADDR_S_A_STATE_DATA;
		for (uint32_t Idx = 0; Idx < NUM_STATES; Idx++)
		{
			s_StateData[Idx] = GameStates[Idx];
			s_OriginalStateFuncs[Idx] = GameStates[Idx].m_Func;
		}
	}

	void UIFrontend::SetCurrentState(FrontendState InState)
	{
		*(FrontendState*)ADDR_S_E_CURRENT_STATE = InState;
	}
}
