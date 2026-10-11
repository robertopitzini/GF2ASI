#include "vehiclepathsm.h"

#include "modules/vehicles/behaviors/vehiclebasicmaneuversm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	VehicleFollowPathStateMachine::VehicleFollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleAiStateMachine(TableID, SMParams)
		, m_PathSMFlags(0)
		, m_PositionReachedThresholdSq(4.0f)
		, m_SpeedRegulationType(0)
		, m_SpecifiedSpeed(0.0f)
	{
		// The shipping build inlined this constructor into the factory, so there is no engine
		// constructor for the class itself to call -- only the one for VehicleAiStateMachine,
		// which fills in the brain, proxy, knowledge and vehicle handles.
		MemUtils::CallClassMethod<void, VehicleFollowPathStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086B060, this, TableID, SMParams);

		// That call leaves the base's vtable in place, so the engine's own is installed over it
		// exactly as the original does. Without this the engine would dispatch through ours, whose
		// slots past the ones declared here are only as correct as the modelled base chain.
		MemUtils::InstallVTable(this, 0x00D76090);
	}

	VehicleFollowPathStateMachine::~VehicleFollowPathStateMachine()
	{
		// Vtable slot 0 is the deleting destructor, so the flag has to be passed: 0 destroys the
		// members without releasing the allocation.
		MemUtils::CallClassMethod<void, VehicleFollowPathStateMachine*, int>(0x0871450, this, 0);
	}

	bool VehicleFollowPathStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleFollowPathStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0871180, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool VehicleFollowPathStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleFollowPathStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x08712B0, this, SimTime, FrameTime, TransID, TransData);
	}

	void VehicleFollowPathStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, VehicleFollowPathStateMachine*, EARS::StateMachineSys::StateMachine*>(0x08713A0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* VehicleFollowPathStateMachine::S_VehicleFollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleFollowPathStateMachine(InID, InSMParams);
	}

	/* static */
	void VehicleFollowPathStateMachine::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("vehicleFollowPathTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		// The original builds the child table from here, before its own.
		VehicleReverseStateMachine::BuildStateTable();

		EARS::Framework::SMBuilder Builder("vehicleFollowPathTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddTransition("seekToTarget", TRANSID_INIT_SEEKTOTARGET);

		// Getting stuck sends the vehicle to reverse, which on completion returns to init rather
		// than to seekToTarget, so the approach is re-evaluated from scratch after backing up.
		EARS::Framework::SMBuilderState* SeekToTargetState = Builder.AddState("seekToTarget", -1);
		SeekToTargetState->AddEnterMessage(MESSAGE_SEEKTOTARGET_INIT);
		SeekToTargetState->AddUpdateMessage(MESSAGE_SEEKTOTARGET_UPDATE);
		SeekToTargetState->AddTransition("done", TRANSID_SEEKTOTARGET_DONE);
		SeekToTargetState->AddTransition("reverse", TRANSID_ISSTUCK);

		EARS::Framework::SMBuilderState* ReverseState = Builder.AddState("reverse", -1);
		ReverseState->AddChild("vehicleReverseTable", true);
		ReverseState->AddTransition("init", EARS::StateMachineSys::Transition::TRANSID_NOCHILDREN);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(MESSAGE_PREPARE_FOR_SCRIPTED_SEQUENCE_TO_END);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_VehicleFollowPathStateMachine, S_VehicleFollowPathStateMachine_FactoryFn, "VehicleFollowPathStateMachine");
	}
}
