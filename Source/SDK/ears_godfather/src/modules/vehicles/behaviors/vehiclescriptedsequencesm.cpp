#include "vehiclescriptedsequencesm.h"

#include "modules/vehicles/behaviors/vehiclepathsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	VehicleScriptedSequenceSM::VehicleScriptedSequenceSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleAiStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleScriptedSequenceSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x08727C0, this, TableID, SMParams);
	}

	VehicleScriptedSequenceSM::~VehicleScriptedSequenceSM()
	{
		// Vtable slot 0 is the deleting destructor, so the flag has to be passed: 0 destroys the
		// members without releasing the allocation, which is what a destructor call means here.
		MemUtils::CallClassMethod<void, VehicleScriptedSequenceSM*, int>(0x0872900, this, 0);
	}

	bool VehicleScriptedSequenceSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleScriptedSequenceSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0872FF0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool VehicleScriptedSequenceSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleScriptedSequenceSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0873270, this, SimTime, FrameTime, TransID, TransData);
	}

	void VehicleScriptedSequenceSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, VehicleScriptedSequenceSM*, EARS::StateMachineSys::StateMachine*>(0x0872960, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* VehicleScriptedSequenceSM::S_VehicleScriptedSequenceSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleScriptedSequenceSM(InID, InSMParams);
	}

	/* static */
	void VehicleScriptedSequenceSM::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("vehicleScriptedSequenceTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		// The original builds the child table from here, before its own; that one in turn builds
		// vehicleReverseTable, so one call covers the whole chain.
		VehicleFollowPathStateMachine::BuildStateTable();

		EARS::Framework::SMBuilder Builder("vehicleScriptedSequenceTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* IdleState = Builder.AddState("idle", -1);
		IdleState->AddEnterMessage(MESSAGE_SSINIT);
		IdleState->AddUpdateMessage(MESSAGE_APPLYGAMEMOVEMENT);
		IdleState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		IdleState->AddTransition("gotoSS", TRANSID_STARTSS);

		// How the vehicle gets to the sequence depends on what the sequence asks for, so this
		// state fans out to pathing, to a turn in place, or straight to waiting.
		EARS::Framework::SMBuilderState* GotoSSState = Builder.AddState("gotoSS", -1);
		GotoSSState->AddEnterMessage(MESSAGE_PROCESSMOVEPOSTYPE);
		GotoSSState->AddUpdateMessage(MESSAGE_APPLYGAMEMOVEMENT);
		GotoSSState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		GotoSSState->AddTransition("pathToPos", TRANSID_GOTOSS_PATHTOPOS);
		GotoSSState->AddTransition("waitAtSS", TRANSID_WAITATSS);
		GotoSSState->AddTransition("faceOrient", TRANSID_FACEORIENT);
		GotoSSState->AddTransition("done", TRANSID_TERMINATESS);

		EARS::Framework::SMBuilderState* PathToPosState = Builder.AddState("pathToPos", -1);
		PathToPosState->AddChild("vehicleFollowPathTable", false);
		PathToPosState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		PathToPosState->AddTransition("waitAtSS", TRANSID_PATHTOPOS_WAITATSS);
		PathToPosState->AddTransition("faceOrient", EARS::StateMachineSys::Transition::TRANSID_NOCHILDREN);

		EARS::Framework::SMBuilderState* FaceOrientState = Builder.AddState("faceOrient", -1);
		FaceOrientState->AddEnterMessage(MESSAGE_FACEORIENT_INIT);
		FaceOrientState->AddUpdateMessage(MESSAGE_FACEORIENT_UPDATE);
		FaceOrientState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		FaceOrientState->AddTransition("waitAtSS", TRANSID_FACEORIENT_WAITATSS);

		EARS::Framework::SMBuilderState* WaitAtSSState = Builder.AddState("waitAtSS", -1);
		WaitAtSSState->AddEnterMessage(MESSAGE_PLAYWAITANIM);
		WaitAtSSState->AddEnterMessage(MESSAGE_PROCESSINPOSITION);
		WaitAtSSState->AddUpdateMessage(MESSAGE_PROCESSINPOSITION);
		WaitAtSSState->AddUpdateMessage(MESSAGE_APPLYGAMEMOVEMENT);
		WaitAtSSState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		WaitAtSSState->AddTransition("action", TRANSID_STARTACTION);

		EARS::Framework::SMBuilderState* ActionState = Builder.AddState("action", -1);
		ActionState->AddEnterMessage(MESSAGE_ACTIONINIT);
		ActionState->AddUpdateMessage(MESSAGE_APPLYGAMEMOVEMENT);
		ActionState->AddTransition("done", TRANSID_NO_SCRIPTED_SEQUENCE);
		ActionState->AddTransition("done", TRANSID_ACTIONDONE);
		ActionState->AddTransition("action", TRANSID_REPEATACTION);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(MESSAGE_SSDONE);

		Builder.CompileAndRegister(kStateMachine_VehicleScriptedSequenceSM, S_VehicleScriptedSequenceSM_FactoryFn, "VehicleScriptedSequenceSM");
	}
}
