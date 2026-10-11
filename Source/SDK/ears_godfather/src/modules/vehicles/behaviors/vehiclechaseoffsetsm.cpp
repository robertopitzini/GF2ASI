#include "vehiclechaseoffsetsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	VehicleChaseOffsetStateMachine::VehicleChaseOffsetStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleAiStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086ECC0, this, TableID, SMParams);
	}

	VehicleChaseOffsetStateMachine::~VehicleChaseOffsetStateMachine()
	{
	}

	EARS::StateMachineSys::StateMachine* VehicleChaseOffsetStateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, VehicleChaseOffsetStateMachine*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void VehicleChaseOffsetStateMachine::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void VehicleChaseOffsetStateMachine::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void VehicleChaseOffsetStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* VehicleChaseOffsetStateMachine::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, VehicleChaseOffsetStateMachine*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* VehicleChaseOffsetStateMachine::S_VehicleChaseOffsetStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleChaseOffsetStateMachine(InID, InSMParams);
	}

	/* static */
	void VehicleChaseOffsetStateMachine::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("vehicleChaseOffsetTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("vehicleChaseOffsetTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddEnterMessage(MESSAGE_INIT_INIT);
		InitState->AddTransition("done", TRANSID_INIT_DONE);
		InitState->AddTransition("block", TRANSID_BLOCK);
		InitState->AddTransition("seek", TRANSID_INIT_SEEK);
		InitState->AddTransition("followEntity", TRANSID_INIT_FOLLOWENTITY);

		// followEntity is the close-quarters pursuit and seek is the long-range one; they hand
		// back and forth as the gap opens and closes.
		EARS::Framework::SMBuilderState* FollowEntityState = Builder.AddState("followEntity", -1);
		FollowEntityState->AddEnterMessage(MESSAGE_FOLLOWENTITY_INIT);
		FollowEntityState->AddUpdateMessage(MESSAGE_UPDATE_CHASE);
		FollowEntityState->AddUpdateMessage(MESSAGE_FOLLOWENTITY_UPDATE);
		FollowEntityState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		FollowEntityState->AddUpdateMessage(MESSAGE_STUCK_TIMER);
		FollowEntityState->AddTransition("teleport", TRANSID_TELEPORT);
		FollowEntityState->AddTransition("seek", TRANSID_CLOSEBY);
		FollowEntityState->AddTransition("reverse", TRANSID_ISSTUCK);
		FollowEntityState->AddTransition("stop", TRANSID_CLOSE_TO_TARGET_STOPPED);
		FollowEntityState->AddTransition("slow_then_reverse", TRANSID_FOLLOW_TO_REVERSE_OR_FLIP);

		EARS::Framework::SMBuilderState* SeekState = Builder.AddState("seek", -1);
		SeekState->AddEnterMessage(MESSAGE_SEEK_INIT);
		SeekState->AddUpdateMessage(MESSAGE_UPDATE_CHASE);
		SeekState->AddUpdateMessage(MESSAGE_SEEK_UPDATE);
		SeekState->AddUpdateMessage(MESSAGE_STUCK_TIMER);
		SeekState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		SeekState->AddTransition("followEntity", TRANSID_SEEK_FOLLOWENTITY);
		SeekState->AddTransition("done", TRANSID_SEEK_DONE);
		SeekState->AddTransition("teleport", TRANSID_TELEPORT);
		SeekState->AddTransition("reverse", TRANSID_ISSTUCK);
		SeekState->AddTransition("stop", TRANSID_CLOSE_TO_TARGET_STOPPED);
		SeekState->AddTransition("maneuver", TRANSID_SEEK_MANEUVER);

		EARS::Framework::SMBuilderState* ManeuverState = Builder.AddState("maneuver", -1);
		ManeuverState->AddEnterMessage(MESSAGE_MANEUVER_INIT);
		ManeuverState->AddUpdateMessage(MESSAGE_MANEUVER_UPDATE);
		ManeuverState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		ManeuverState->AddTransition("seek", TRANSID_MANEUVER_SEEK);
		ManeuverState->AddTransition("followEntity", TRANSID_MANEUVER_FOLLOWENTITY);
		ManeuverState->AddTransition("stop", TRANSID_CLOSE_TO_TARGET_STOPPED);
		ManeuverState->AddTransition("done", TRANSID_MANEUVER_DONE);

		// Teleporting is how a chase keeps up once it is off camera. The default transition back
		// to init runs unconditionally, so this state is one frame long; only a build with
		// teleporting disabled diverts to kill instead.
		EARS::Framework::SMBuilderState* TeleportState = Builder.AddState("teleport", -1);
		TeleportState->AddEnterMessage(MESSAGE_TELEPORT_ENTER);
		TeleportState->AddExitMessage(MESSAGE_TELEPORT_EXIT);
		TeleportState->AddTransition("kill", TRANSID_TELEPORT_DISABLED);
		TeleportState->AddTransition("init", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// Reversing straight from speed would look wrong, so a vehicle that needs to flip around
		// slows to a stop here first and only then spawns the reverse child.
		EARS::Framework::SMBuilderState* SlowThenReverseState = Builder.AddState("slow_then_reverse", -1);
		SlowThenReverseState->AddEnterMessage(MESSAGE_SLOWDOWN_ENTER);
		SlowThenReverseState->AddEnterMessage(MESSAGE_REVERSE_ENTER);
		SlowThenReverseState->AddUpdateMessage(MESSAGE_STUCK_TIMER);
		SlowThenReverseState->AddUpdateMessage(MESSAGE_UPDATE_CHASE);
		SlowThenReverseState->AddUpdateMessage(MESSAGE_SLOWDOWN_UPDATE);
		SlowThenReverseState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		SlowThenReverseState->AddTransition("teleport", TRANSID_TELEPORT);
		SlowThenReverseState->AddTransition("reverse", TRANSID_SLOWENOUGHTOREVERSE);

		EARS::Framework::SMBuilderState* ReverseState = Builder.AddState("reverse", -1);
		ReverseState->AddChild("vehicleReverseTable", true);
		ReverseState->AddEnterMessage(MESSAGE_REVERSE_ENTER);
		ReverseState->AddUpdateMessage(MESSAGE_STUCK_TIMER);
		ReverseState->AddUpdateMessage(MESSAGE_UPDATE_CHASE);
		ReverseState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		ReverseState->AddTransition("teleport", TRANSID_TELEPORT);
		ReverseState->AddTransition("stop", TRANSID_CLOSE_TO_TARGET_STOPPED);
		ReverseState->AddTransition("init", EARS::StateMachineSys::Transition::TRANSID_NOCHILDREN);

		// MAYBE_BUG: block has no way out other than teleport -- no stuck handling and no return
		// to the chase -- so a blocking vehicle stays blocking until the chase goes off camera.
		EARS::Framework::SMBuilderState* BlockState = Builder.AddState("block", -1);
		BlockState->AddUpdateMessage(MESSAGE_UPDATE_BLOCK);
		BlockState->AddUpdateMessage(MESSAGE_STUCK_TIMER);
		BlockState->AddUpdateMessage(MESSAGE_UPDATE_CHASE);
		BlockState->AddUpdateMessage(MESSAGE_FORCE_TARGET_ON_OCCUPANTS);
		BlockState->AddTransition("teleport", TRANSID_TELEPORT);

		EARS::Framework::SMBuilderState* StopState = Builder.AddState("stop", -1);
		StopState->AddEnterMessage(MESSAGE_SLOWDOWN_UPDATE);
		StopState->AddUpdateMessage(MESSAGE_SLOWDOWN_UPDATE);
		StopState->AddTransition("exitOccupants", TRANSID_HAS_STOPPED);
		StopState->AddTransition("init", TRANSID_TARGET_FLED);

		EARS::Framework::SMBuilderState* ExitOccupantsState = Builder.AddState("exitOccupants", -1);
		ExitOccupantsState->AddEnterMessage(MESSAGE_EXIT_OCCUPANTS);
		ExitOccupantsState->AddUpdateMessage(MESSAGE_UPDATE_STOPPED);
		ExitOccupantsState->AddTransition("teleport", TRANSID_TELEPORT);

		// The kill message is on exit rather than entry, so it fires as the machine leaves for
		// done rather than while it is still here.
		EARS::Framework::SMBuilderState* KillState = Builder.AddState("kill", -1);
		KillState->AddExitMessage(MESSAGE_KILL);
		KillState->AddTransition("done", TRANSID_ITSKILLINGTIME);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(MESSAGE_DONE_INIT);

		Builder.CompileAndRegister(kStateMachine_VehicleChaseOffsetStateMachine, S_VehicleChaseOffsetStateMachine_FactoryFn, "VehicleChaseOffsetStateMachine");
	}
}
