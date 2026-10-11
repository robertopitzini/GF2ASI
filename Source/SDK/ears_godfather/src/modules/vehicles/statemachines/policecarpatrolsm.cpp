#include "policecarpatrolsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	PoliceCarPatrolSM::PoliceCarPatrolSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleApproachPositionSM(TableID, SMParams)
	{
		// The shipping build inlined this constructor into the factory, so there is no engine
		// constructor for the class itself to call -- only the one for VehicleApproachPositionSM,
		// which sets up the approach location, the damage listener and the event listener.
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0882A00, this, TableID, SMParams);

		// That call leaves the base vtables in place, so the engine ones go over them: the
		// primary, then IDamageListener and IStateMachineEventListener.
		MemUtils::InstallVTable(this, 0x00D76CF0);
		MemUtils::InstallVTable(this, 0x00D76CDC, 0x50);
		MemUtils::InstallVTable(this, 0x00D76CD8, 0x5C);

		// 200 units, over the default this layer sets for itself. A police car will break traffic
		// rules to reach a scene from much further out than any other approaching vehicle.
		m_CloseEnoughToActAsRuleBreakerSq = 40000.0f;
	}

	PoliceCarPatrolSM::~PoliceCarPatrolSM()
	{
	}

	EARS::StateMachineSys::StateMachine* PoliceCarPatrolSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PoliceCarPatrolSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void PoliceCarPatrolSM::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void PoliceCarPatrolSM::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void PoliceCarPatrolSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* PoliceCarPatrolSM::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, PoliceCarPatrolSM*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* PoliceCarPatrolSM::S_PoliceCarPatrolSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PoliceCarPatrolSM(InID, InSMParams);
	}

	/* static */
	void PoliceCarPatrolSM::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("PoliceCarRootST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("PoliceCarRootST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddEnterMessage(MESSAGE_SET_APPROACH_LOCATION);
		InitState->AddTransition("driveAround", TRANSID_NO_APPROACH_LOCATION);
		InitState->AddTransition("approachScene", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// Patrolling. The approach location is asked for again every frame, so a car already on
		// the road graph picks up a new scene without going back through init.
		EARS::Framework::SMBuilderState* DriveAroundState = Builder.AddState("driveAround", -1);
		DriveAroundState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		DriveAroundState->AddUpdateMessage(MESSAGE_SET_APPROACH_LOCATION);
		DriveAroundState->AddTransition("driveAroundEBrake", TRANSID_OCCUPANTS_SEE_CRIMINAL);
		DriveAroundState->AddTransition("driveAroundEBrake", TRANSID_WITHIN_CRIME_SCENE);
		DriveAroundState->AddTransition("letCopsOut", TRANSID_PARKED);
		DriveAroundState->AddTransition("letCopsOut", TRANSID_FORCED_STOP);
		DriveAroundState->AddTransition("approachScene", TRANSID_HAS_APPROACH_LOCATION);

		// The timeout is what guarantees the cops get out: every other way out of this state
		// needs the car to actually stop somewhere sensible first.
		EARS::Framework::SMBuilderState* DriveAroundEBrakeState = Builder.AddState("driveAroundEBrake", -1);
		DriveAroundEBrakeState->AddEnterMessage(MESSAGE_ACT_AS_RULEBREAKER);
		DriveAroundEBrakeState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		DriveAroundEBrakeState->AddUpdateMessage(MESSAGE_DEBUG);
		DriveAroundEBrakeState->AddTransition("letCopsOut", TRANSID_PARKED);
		DriveAroundEBrakeState->AddTransition("letCopsOut", TRANSID_HIT_DEADEND);
		DriveAroundEBrakeState->AddTransition("letCopsOut", TRANSID_REVERSING_BUT_CLOSE_ENOUGH);
		DriveAroundEBrakeState->AddTransition("letCopsOut", TRANSID_VEHICLE_HIGH_VELOCITY);
		DriveAroundEBrakeState->AddFloatTransition("letCopsOut", EARS::StateMachineSys::Transition::TRANSID_TIMEOUT, 2.0f, 1);

		EARS::Framework::SMBuilderState* ApproachSceneState = Builder.AddState("approachScene", -1);
		ApproachSceneState->AddUpdateMessage(MESSAGE_URGENT_APPROACH);
		ApproachSceneState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		ApproachSceneState->AddUpdateMessage(MESSAGE_DEBUG);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_PARKED);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_FORCED_STOP);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_CLOSE_TO_LOCATION);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_RULEBREAKER_AND_SLOW_ENOUGH);
		ApproachSceneState->AddTransition("prepareForEBrake", TRANSID_OCCUPANTS_SEE_CRIMINAL);
		ApproachSceneState->AddTransition("prepareForEBrake", TRANSID_WITHIN_CRIME_SCENE);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_REVERSING_BUT_CLOSE_ENOUGH);
		ApproachSceneState->AddTransition("letCopsOut", TRANSID_HIT_DEADEND);

		// Same shape as driveAroundEBrake, reached from the approach instead of the patrol.
		EARS::Framework::SMBuilderState* PrepareForEBrakeState = Builder.AddState("prepareForEBrake", -1);
		PrepareForEBrakeState->AddEnterMessage(MESSAGE_ACT_AS_RULEBREAKER);
		PrepareForEBrakeState->AddUpdateMessage(MESSAGE_FOLLOWGRAPH_UPDATE);
		PrepareForEBrakeState->AddUpdateMessage(MESSAGE_DEBUG);
		PrepareForEBrakeState->AddTransition("letCopsOut", TRANSID_PARKED);
		PrepareForEBrakeState->AddTransition("letCopsOut", TRANSID_CLOSE_TO_LOCATION);
		PrepareForEBrakeState->AddTransition("letCopsOut", TRANSID_REVERSING_BUT_CLOSE_ENOUGH);
		PrepareForEBrakeState->AddTransition("letCopsOut", TRANSID_HIT_DEADEND);
		PrepareForEBrakeState->AddTransition("letCopsOut", TRANSID_VEHICLE_HIGH_VELOCITY);
		PrepareForEBrakeState->AddFloatTransition("letCopsOut", EARS::StateMachineSys::Transition::TRANSID_TIMEOUT, 2.0f, 1);

		// The pulling-over update is an enter message as well, so the first frame of stopping
		// happens immediately rather than a frame late.
		EARS::Framework::SMBuilderState* LetCopsOutState = Builder.AddState("letCopsOut", -1);
		LetCopsOutState->AddEnterMessage(MESSAGE_PULLING_OVER_INIT);
		LetCopsOutState->AddEnterMessage(MESSAGE_PULLING_OVER_UPDATE);
		LetCopsOutState->AddUpdateMessage(MESSAGE_PULLING_OVER_UPDATE);
		LetCopsOutState->AddTransition("occupantsExit", TRANSID_PARKED);
		LetCopsOutState->AddTransition("occupantsExit", TRANSID_VEHICLE_SLOW_ENOUGH);

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

		Builder.CompileAndRegister(kStateMachine_PoliceCarPatrolSM, S_PoliceCarPatrolSM_FactoryFn, "PoliceCarPatrolSM");
	}
}
