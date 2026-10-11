#include "npcshootsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	ShootSM::ShootSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ShootSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076D020, this, TableID, SMParams);
	}

	ShootSM::~ShootSM()
	{
		MemUtils::CallClassMethod<void, ShootSM*>(0x072CB10, this);
	}

	bool ShootSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076BC00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ShootSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BCD0, this, SimTime, FrameTime, TransID, TransData);
	}

	void ShootSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ShootSM*, EARS::StateMachineSys::StateMachine*>(0x076C2F0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ShootSM::S_ShootSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ShootSM(InID, InSMParams);
	}

	ContinualBurstFireSM::ContinualBurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076BE70, this, TableID, SMParams);
	}

	ContinualBurstFireSM::~ContinualBurstFireSM()
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*>(0x04DAF90, this);
	}

	bool ContinualBurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076C6A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ContinualBurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BF40, this, SimTime, FrameTime, TransID, TransData);
	}

	void ContinualBurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x076BF80, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ContinualBurstFireSM::S_ContinualBurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ContinualBurstFireSM(InID, InSMParams);
	}

	BurstFireSM::BurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
	}

	BurstFireSM::~BurstFireSM()
	{
		MemUtils::CallClassMethod<void, BurstFireSM*>(0x04DAF90, this);
	}

	bool BurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076CAC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool BurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076CF10, this, SimTime, FrameTime, TransID, TransData);
	}

	void BurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, BurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* BurstFireSM::S_BurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new BurstFireSM(InID, InSMParams);
	}

	/* static */
	void ShootSM::BuildShootStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("ShootST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("ShootST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddTransition("burst", TRANSID_TEST_MINIMUM_DELAY);
		InitState->AddTransition("delayedStart", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// The two idle states run TurnSeekStateMachine's seekTargetTable as a child, so the NPC
		// keeps tracking its target through the gaps between bursts.
		EARS::Framework::SMBuilderState* DelayedStartState = Builder.AddState("delayedStart", -1);
		DelayedStartState->AddEnterMessage(MESSAGE_PLAY_IDLE);
		DelayedStartState->AddChild("seekTargetTable", true);
		DelayedStartState->AddTransition("burst", TRANSID_TEST_MINIMUM_DELAY);

		EARS::Framework::SMBuilderState* BurstState = Builder.AddState("burst", -1);
		BurstState->AddChild("BurstFireST", true);
		BurstState->AddTransition("testDone", EARS::StateMachineSys::Transition::TRANSID_NOCHILDREN);

		EARS::Framework::SMBuilderState* TestDoneState = Builder.AddState("testDone", -1);
		TestDoneState->AddEnterMessage(MESSAGE_INCREMENT_BURSTS);
		TestDoneState->AddTransition("done", TRANSID_EXIT_BASED_ON_GUN);
		TestDoneState->AddTransition("waitForBurst", TRANSID_HAS_MORE_BURSTS);
		TestDoneState->AddTransition("done", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		EARS::Framework::SMBuilderState* WaitForBurstState = Builder.AddState("waitForBurst", -1);
		WaitForBurstState->AddEnterMessage(MESSAGE_PLAY_IDLE);
		WaitForBurstState->AddChild("seekTargetTable", true);
		WaitForBurstState->AddTransition("burst", TRANSID_CAN_BURST);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_ShootSM, S_ShootSM_FactoryFn, "ShootSM");
	}

	/* static */
	void ContinualBurstFireSM::BuildStateMachine()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("ContinualBurstFireST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("ContinualBurstFireST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddTransition("terminate", TRANSID_NO_GUN);
		InitState->AddTransition("noAnimShootUpdate", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		EARS::Framework::SMBuilderState* NoAnimShootUpdateState = Builder.AddState("noAnimShootUpdate", -1);
		NoAnimShootUpdateState->AddUpdateMessage(MESSAGE_FIRE_UPDATE);
		NoAnimShootUpdateState->AddTransition("terminate", TRANSID_NO_GUN);

		EARS::Framework::SMBuilderState* TerminateState = Builder.AddState("terminate", -1);
		TerminateState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_ContinualBurstFireSM, S_ContinualBurstFireSM_FactoryFn, "ContinualBurstFireSM");
	}

	/* static */
	void BurstFireSM::BuildBurstFireStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("BurstFireST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("BurstFireST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* BurstInitState = Builder.AddState("burstInit", -1);
		BurstInitState->AddEnterMessage(MESSAGE_BURST_INIT);
		BurstInitState->AddTransition("shotblocked", TRANSID_SHOTBLOCKED);
		BurstInitState->AddTransition("prepFire", TRANSID_GUN_READY);
		BurstInitState->AddTransition("delayFire", TRANSID_GUN_NOT_READY);

		EARS::Framework::SMBuilderState* DelayFireState = Builder.AddState("delayFire", -1);
		DelayFireState->AddEnterMessage(MESSAGE_PLAYIDLE);
		DelayFireState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		DelayFireState->AddTransition("prepFire", TRANSID_GUN_READY);

		EARS::Framework::SMBuilderState* PrepFireState = Builder.AddState("prepFire", -1);
		PrepFireState->AddEnterMessage(MESSAGE_CHATTER_SHOOT);
		PrepFireState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		PrepFireState->AddTransition("playFireAnim", TRANSID_IS_FACING_TARGET);

		// The shot is driven by a CCT event inside the firing animation; TRANSID_ANIMDONE is the
		// fallback for an animation that ends without ever raising it.
		EARS::Framework::SMBuilderState* PlayFireAnimState = Builder.AddState("playFireAnim", -1);
		PlayFireAnimState->AddEnterMessage(MESSAGE_FIRE_PLAYANIM);
		PlayFireAnimState->AddEnterMessage(MESSAGE_SET_WAIT_FOR_CCT_GUNFIRE_EVENT);
		PlayFireAnimState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		PlayFireAnimState->AddTransition("doneCCTFiring", TRANSID_CCT_GUNFIRE_REACHED);
		PlayFireAnimState->AddTransition("doneCCTFiring", TRANSID_ANIMDONE);

		EARS::Framework::SMBuilderState* DoneCCTFiringState = Builder.AddState("doneCCTFiring", -1);
		DoneCCTFiringState->AddEnterMessage(MESSAGE_INCREMENT_SHOT_COUNT);
		DoneCCTFiringState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		DoneCCTFiringState->AddTransition("postFireDelay", TRANSID_BURST_DONE);
		DoneCCTFiringState->AddTransition("fireGun", TRANSID_GUN_READY);

		// Shots after the first in a burst skip the animation and fire straight from this state.
		EARS::Framework::SMBuilderState* FireGunState = Builder.AddState("fireGun", -1);
		FireGunState->AddEnterMessage(MESSAGE_FIRE_GUN_NO_ANIM);
		FireGunState->AddEnterMessage(MESSAGE_INCREMENT_SHOT_COUNT);
		FireGunState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		FireGunState->AddTransition("postFireDelay", TRANSID_BURST_DONE);
		FireGunState->AddTransition("fireGun", TRANSID_GUN_READY);

		EARS::Framework::SMBuilderState* ShotblockedState = Builder.AddState("shotblocked", -1);
		ShotblockedState->AddEnterMessage(MESSAGE_MARK_SHOTBLOCKED);
		ShotblockedState->AddEnterMessage(MESSAGE_CHATTER_SHOTBLOCKED);
		ShotblockedState->AddEnterMessage(MESSAGE_PLAYIDLE);
		ShotblockedState->AddUpdateMessage(MESSAGE_BURSTFIRE_UPDATE);
		ShotblockedState->AddFloatTransition("burstDone", EARS::StateMachineSys::Transition::TRANSID_TIMEOUT, 0.75f, 1);

		EARS::Framework::SMBuilderState* PostFireDelayState = Builder.AddState("postFireDelay", -1);
		PostFireDelayState->AddFloatTransition("burstDone", EARS::StateMachineSys::Transition::TRANSID_TIMEOUT, 0.25f, 1);

		EARS::Framework::SMBuilderState* BurstDoneState = Builder.AddState("burstDone", -1);
		BurstDoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_BurstFireSM, S_BurstFireSM_FactoryFn, "BurstFireSM");
	}

	/* static */
	void BurstFireSM::BuildBurstFireNoAnimStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("BurstFireNoAnimST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("BurstFireNoAnimST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* BurstNoAnimInitState = Builder.AddState("burstNoAnimInit", -1);
		BurstNoAnimInitState->AddEnterMessage(MESSAGE_BURST_INIT);
		BurstNoAnimInitState->AddTransition("burstNoAnimDoneShotBlocked", TRANSID_SHOTBLOCKED);
		BurstNoAnimInitState->AddTransition("useGunNoAnim", TRANSID_GUN_READY);

		// Fires on entry and re-enters itself for each further shot, so there is no per-frame
		// update message and no post-fire delay state.
		EARS::Framework::SMBuilderState* UseGunNoAnimState = Builder.AddState("useGunNoAnim", -1);
		UseGunNoAnimState->AddEnterMessage(MESSAGE_FIRE_GUN_NO_ANIM);
		UseGunNoAnimState->AddEnterMessage(MESSAGE_INCREMENT_SHOT_COUNT);
		UseGunNoAnimState->AddEnterMessage(MESSAGE_CHATTER_SHOOT);
		UseGunNoAnimState->AddTransition("burstNoAnimDone", TRANSID_BURST_DONE);
		UseGunNoAnimState->AddTransition("useGunNoAnim", TRANSID_GUN_READY);
		UseGunNoAnimState->AddTransition("burstNoAnimDoneShotBlocked", TRANSID_SHOTBLOCKED);

		EARS::Framework::SMBuilderState* BurstNoAnimDoneShotBlockedState = Builder.AddState("burstNoAnimDoneShotBlocked", -1);
		BurstNoAnimDoneShotBlockedState->AddEnterMessage(MESSAGE_MARK_SHOTBLOCKED);
		BurstNoAnimDoneShotBlockedState->AddEnterMessage(MESSAGE_CHATTER_SHOTBLOCKED);
		BurstNoAnimDoneShotBlockedState->AddTransition("burstNoAnimDone", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		EARS::Framework::SMBuilderState* BurstNoAnimDoneState = Builder.AddState("burstNoAnimDone", -1);
		BurstNoAnimDoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_BurstFireSM, S_BurstFireSM_FactoryFn, "BurstFireSM");
	}
}
