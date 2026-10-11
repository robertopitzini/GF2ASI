#include "vehiclebasicmaneuversm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	VehicleReverseStateMachine::VehicleReverseStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleAiStateMachine(TableID, SMParams)
		, m_bDone(false)
	{
		// The shipping build inlined this constructor into the factory, so there is no engine
		// constructor for the class itself to call -- only the one for VehicleAiStateMachine,
		// which fills in the brain, proxy, knowledge and vehicle handles.
		MemUtils::CallClassMethod<void, VehicleReverseStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086B060, this, TableID, SMParams);

		// That call leaves the base's vtable in place, so the engine's own is installed over it
		// exactly as the original does.
		MemUtils::InstallVTable(this, 0x00D75D54);
	}

	VehicleReverseStateMachine::~VehicleReverseStateMachine()
	{
		// Vtable slot 0 is the deleting destructor, so the flag has to be passed: 0 destroys the
		// members without releasing the allocation.
		MemUtils::CallClassMethod<void, VehicleReverseStateMachine*, int>(0x086B1E0, this, 0);
	}

	bool VehicleReverseStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleReverseStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x086B340, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool VehicleReverseStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleReverseStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x086B380, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* VehicleReverseStateMachine::S_VehicleReverseStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleReverseStateMachine(InID, InSMParams);
	}

	/* static */
	void VehicleReverseStateMachine::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("vehicleReverseTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("vehicleReverseTable", get_thread_new_allocator());

		// The timeout is the only way out for a vehicle that cannot back up at all, so the SM
		// always terminates and the parent always gets its TRANSID_NOCHILDREN.
		EARS::Framework::SMBuilderState* ReverseState = Builder.AddState("reverse", -1);
		ReverseState->AddUpdateMessage(MESSAGE_REVERSE_UPDATE);
		ReverseState->AddTransition("done", TRANSID_DONE);
		ReverseState->AddFloatTransition("done", EARS::StateMachineSys::Transition::TRANSID_TIMEOUT, 2.5f, 1);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_VehicleReverseStateMachine, S_VehicleReverseStateMachine_FactoryFn, "VehicleReverseStateMachine");
	}
}
