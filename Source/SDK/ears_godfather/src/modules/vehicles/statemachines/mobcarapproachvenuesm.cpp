#include "mobcarapproachvenuesm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	MobCarApproachVenueSM::MobCarApproachVenueSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleApproachPositionSM(TableID, SMParams)
		, m_VenueID(0)
	{
		// The shipping build inlined this constructor into the factory, so there is no engine
		// constructor for the class itself to call -- only the one for VehicleApproachPositionSM,
		// which sets up the approach location, the damage listener and the event listener.
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0882A00, this, TableID, SMParams);

		// That call leaves the base vtables in place, so the engine ones go over them: the
		// primary, then IDamageListener and IStateMachineEventListener.
		MemUtils::InstallVTable(this, 0x00D76C04);
		MemUtils::InstallVTable(this, 0x00D76BF0, 0x50);
		MemUtils::InstallVTable(this, 0x00D76BEC, 0x5C);
	}

	MobCarApproachVenueSM::~MobCarApproachVenueSM()
	{
	}

	EARS::StateMachineSys::StateMachine* MobCarApproachVenueSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, MobCarApproachVenueSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void MobCarApproachVenueSM::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void MobCarApproachVenueSM::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void MobCarApproachVenueSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* MobCarApproachVenueSM::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, MobCarApproachVenueSM*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* MobCarApproachVenueSM::S_MobCarApproachVenueSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new MobCarApproachVenueSM(InID, InSMParams);
	}

	/* static */
	void MobCarApproachVenueSM::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("MobCarRootST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("MobCarRootST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddEnterMessage(MESSAGE_MOB_INIT);
		InitState->AddEnterMessage(MESSAGE_SET_APPROACH_LOCATION);
		InitState->AddTransition("normalVehicleFollowGraph", TRANSID_NO_APPROACH_LOCATION);
		InitState->AddTransition("followGraph", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// Driving as ordinary traffic while waiting for a venue. The approach location is asked
		// for again every frame, so the car moves up to followGraph as soon as one is assigned.
		EARS::Framework::SMBuilderState* NormalVehicleFollowGraphState = Builder.AddState("normalVehicleFollowGraph", -1);
		NormalVehicleFollowGraphState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		NormalVehicleFollowGraphState->AddUpdateMessage(MESSAGE_SET_APPROACH_LOCATION);
		NormalVehicleFollowGraphState->AddTransition("letOccupantsOut", TRANSID_PARKED);
		NormalVehicleFollowGraphState->AddTransition("letOccupantsOut", TRANSID_FORCED_STOP);
		NormalVehicleFollowGraphState->AddTransition("letOccupantsOut", TRANSID_VEHICLE_STUCK);
		NormalVehicleFollowGraphState->AddTransition("letOccupantsOut", TRANSID_HIT_DEADEND);
		NormalVehicleFollowGraphState->AddTransition("followGraph", TRANSID_HAS_APPROACH_LOCATION);

		// Every way this state ends drops the occupants off, including getting stuck or hitting a
		// dead end, so a mob car that cannot reach the venue unloads wherever it got to.
		EARS::Framework::SMBuilderState* FollowGraphState = Builder.AddState("followGraph", -1);
		FollowGraphState->AddUpdateMessage(MESSAGE_URGENT_APPROACH);
		FollowGraphState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		FollowGraphState->AddUpdateMessage(MESSAGE_DEBUG);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_PARKED);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_RULEBREAKER_AND_SLOW_ENOUGH);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_FORCED_STOP);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_VEHICLE_STUCK);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_CLOSE_TO_LOCATION);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_REVERSING_BUT_CLOSE_ENOUGH);
		FollowGraphState->AddTransition("letOccupantsOut", TRANSID_HIT_DEADEND);

		EARS::Framework::SMBuilderState* LetOccupantsOutState = Builder.AddState("letOccupantsOut", -1);
		LetOccupantsOutState->AddEnterMessage(MESSAGE_PULLING_OVER_INIT);
		LetOccupantsOutState->AddEnterMessage(MESSAGE_PULLING_OVER_UPDATE);
		LetOccupantsOutState->AddUpdateMessage(MESSAGE_PULLING_OVER_UPDATE);
		LetOccupantsOutState->AddUpdateMessage(MESSAGE_DEBUG);
		LetOccupantsOutState->AddTransition("occupantsExit", TRANSID_PARKED);
		LetOccupantsOutState->AddTransition("occupantsExit", TRANSID_VEHICLE_SLOW_ENOUGH);

		EARS::Framework::SMBuilderState* OccupantsExitState = Builder.AddState("occupantsExit", -1);
		OccupantsExitState->AddEnterMessage(MESSAGE_EXIT_OCCUPANTS);
		OccupantsExitState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		OccupantsExitState->AddTransition("waitForOccupantsToExit", TRANSID_PARKED);

		EARS::Framework::SMBuilderState* WaitForOccupantsToExitState = Builder.AddState("waitForOccupantsToExit", -1);
		WaitForOccupantsToExitState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		WaitForOccupantsToExitState->AddTransition("done", TRANSID_OCCUPANTS_EXITED);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(MESSAGE_SET_TO_IDLE);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_MobCarApproachVenueSM, S_MobCarApproachVenueSM_FactoryFn, "MobCarApproachVenueSM");
	}
}
