#include "npcguncombatsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	ReloadGunSM::ReloadGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
	}

	ReloadGunSM::~ReloadGunSM()
	{
		MemUtils::CallClassMethod<void, ReloadGunSM*>(0x0716310, this);
	}

	bool ReloadGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ReloadGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07687F0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ReloadGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ReloadGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07684D0, this, SimTime, FrameTime, TransID, TransData);
	}

	void ReloadGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ReloadGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ReloadGunSM::S_ReloadGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ReloadGunSM(InID, InSMParams);
	}

	DrawGunSM::DrawGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
	}

	DrawGunSM::~DrawGunSM()
	{
		MemUtils::CallClassMethod<void, DrawGunSM*>(0x0768C20, this);
	}

	bool DrawGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, DrawGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0768D70, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool DrawGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, DrawGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0768590, this, SimTime, FrameTime, TransID, TransData);
	}

	void DrawGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, DrawGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* DrawGunSM::S_DrawGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new DrawGunSM(InID, InSMParams);
	}

	HolsterGunSM::HolsterGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
	}

	HolsterGunSM::~HolsterGunSM()
	{
		MemUtils::CallClassMethod<void, HolsterGunSM*>(0x04DAF90, this);
	}

	bool HolsterGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, HolsterGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0768A70, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool HolsterGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, HolsterGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0768650, this, SimTime, FrameTime, TransID, TransData);
	}

	void HolsterGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, HolsterGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* HolsterGunSM::S_HolsterGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new HolsterGunSM(InID, InSMParams);
	}

	/* static */
	void ReloadGunSM::BuildReloadGunStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("ReloadGunST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("ReloadGunST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* ReloadState = Builder.AddState("reload", -1);
		ReloadState->AddEnterMessage(MESSAGE_RELOAD_INIT);
		ReloadState->AddUpdateMessage(MESSAGE_RELOAD_UPDATE);
		ReloadState->AddTransition("done", TRANSID_RELOAD_DONE);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_ReloadGunSM, S_ReloadGunSM_FactoryFn, "ReloadGunSM");
	}

	/* static */
	void DrawGunSM::BuildDrawGunStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("DrawGunST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("DrawGunST", get_thread_new_allocator());

		// Unlike holster and reload, drawing can be declined outright: if the NPC has nothing to
		// draw, init falls through to done and the SM terminates without playing anything.
		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddTransition("drawGun", TRANSID_CAN_DRAW);
		InitState->AddTransition("done", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		EARS::Framework::SMBuilderState* DrawGunState = Builder.AddState("drawGun", -1);
		DrawGunState->AddEnterMessage(MESSAGE_DRAWGUN_INIT);
		DrawGunState->AddUpdateMessage(MESSAGE_DRAWGUN_UPDATE);
		DrawGunState->AddTransition("done", TRANSID_ANIMDONE);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_DrawGunSM, S_DrawGunSM_FactoryFn, "DrawGunSM");
	}

	/* static */
	void HolsterGunSM::BuildHolsterStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("HolsterGunST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("HolsterGunST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* HolsterState = Builder.AddState("holster", -1);
		HolsterState->AddEnterMessage(MESSAGE_HOLSTER_INIT);
		// MAYBE_BUG: _UPDATE is registered as a second enter message, so it fires once on entry
		// and never again, where DrawGunSM and ReloadGunSM register their equivalents as update
		// messages. Leaving the holster to advance on TRANSID_HOLSTER_DONE alone.
		HolsterState->AddEnterMessage(MESSAGE_HOLSTER_UPDATE);
		HolsterState->AddTransition("done", TRANSID_HOLSTER_DONE);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_HolsterGunSM, S_HolsterGunSM_FactoryFn, "HolsterGunSM");
	}
}
