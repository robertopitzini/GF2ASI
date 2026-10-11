#include "rootvehiclesm.h"

#include "modules/vehicles/behaviors/whitebox_car/whitebox_car.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/base/base.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	RootVehicleSM::RootVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		// The shipping build inlined this constructor into the factory, so the engine vtable has
		// to be installed over the one AnimateStateMachine's constructor left behind.
		MemUtils::InstallVTable(this, 0x00D76DB0);

		// MAYBE_BUG: neither member is initialised before this point, so a vehicle whose owner
		// has no brain attached leaves m_Brain indeterminate rather than null. The original
		// asserts on both the owner and the brain, which only fires in a debug build.
		EARS::Vehicles::WhiteboxCar* Car =
			EARS::Framework::_GetInterface<EARS::Vehicles::WhiteboxCar>(GetAnimated(), 0xA5975EB2);

		EARS::Modules::IVehicleBrain* Brain = Car->GetAttachedBrain();
		if (Brain != nullptr)
		{
			m_Brain = static_cast<EARS::Modules::VehicleBrain*>(Brain);
		}
	}

	RootVehicleSM::~RootVehicleSM()
	{
		// Vtable slot 1 is the deleting destructor, so the flag has to be passed: 0 destroys the
		// members without releasing the allocation.
		MemUtils::CallClassMethod<void, RootVehicleSM*, int>(0x0881E70, this, 0);
	}

	bool RootVehicleSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, RootVehicleSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0881D10, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool RootVehicleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, RootVehicleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0881D50, this, SimTime, FrameTime, TransID, TransData);
	}

	int RootVehicleSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, RootVehicleSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* RootVehicleSM::S_RootVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new RootVehicleSM(InID, InSMParams);
	}

	/* static */
	void RootVehicleSM::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("RootVehicleST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("RootVehicleST", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* InitState = Builder.AddState("init", -1);
		InitState->AddTransition("pickBrain", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// Nothing happens here beyond the transitions: CheckTransition reads the brain's current
		// BrainType, and the trailing default sends an unrecognised one to idle.
		EARS::Framework::SMBuilderState* PickBrainState = Builder.AddState("pickBrain", -1);
		PickBrainState->AddTransition("idle", TRANSID_IDLE);
		PickBrainState->AddTransition("civilian", TRANSID_CIVILIAN);
		PickBrainState->AddTransition("chase", TRANSID_CHASE);
		PickBrainState->AddTransition("policePatrol", TRANSID_POLICE_PATROL);
		PickBrainState->AddTransition("mobVenue", TRANSID_MOB_VENUE);
		PickBrainState->AddTransition("idle", EARS::StateMachineSys::Transition::TRANSID_DEFAULT);

		// Every brain state is the same shape: announce the change, spawn that brain's table as a
		// child, and go back to pickBrain the moment the brain changes under it.
		EARS::Framework::SMBuilderState* IdleState = Builder.AddState("idle", -1);
		IdleState->AddEnterMessage(MESSAGE_NEW_STATE_ENTER);
		IdleState->AddChild("VehicleIdleST", true);
		IdleState->AddTransition("pickBrain", TRANSID_BRAIN_CHANGE);

		EARS::Framework::SMBuilderState* CivilianState = Builder.AddState("civilian", -1);
		CivilianState->AddEnterMessage(MESSAGE_NEW_STATE_ENTER);
		CivilianState->AddChild("civilianVehicleMainTable", true);
		CivilianState->AddTransition("pickBrain", TRANSID_BRAIN_CHANGE);

		EARS::Framework::SMBuilderState* ChaseState = Builder.AddState("chase", -1);
		ChaseState->AddEnterMessage(MESSAGE_NEW_STATE_ENTER);
		ChaseState->AddChild("vehicleChaseOffsetTable", true);
		ChaseState->AddTransition("pickBrain", TRANSID_BRAIN_CHANGE);

		EARS::Framework::SMBuilderState* PolicePatrolState = Builder.AddState("policePatrol", -1);
		PolicePatrolState->AddEnterMessage(MESSAGE_NEW_STATE_ENTER);
		PolicePatrolState->AddChild("PoliceCarRootST", true);
		PolicePatrolState->AddTransition("pickBrain", TRANSID_BRAIN_CHANGE);

		EARS::Framework::SMBuilderState* MobVenueState = Builder.AddState("mobVenue", -1);
		MobVenueState->AddEnterMessage(MESSAGE_NEW_STATE_ENTER);
		MobVenueState->AddChild("MobCarRootST", true);
		MobVenueState->AddTransition("pickBrain", TRANSID_BRAIN_CHANGE);

		Builder.CompileAndRegister(kStateMachine_RootVehicleSM, S_RootVehicleSM_FactoryFn, "RootVehicleSM");
	}
}
