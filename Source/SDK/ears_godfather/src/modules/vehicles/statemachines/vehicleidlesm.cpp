#include "vehicleidlesm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	VehicleIdleSM::VehicleIdleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::VehicleAiStateMachine(TableID, SMParams)
	{
		// The shipping build inlined this constructor into the factory, so there is no engine
		// constructor for the class itself to call -- only the one for VehicleAiStateMachine,
		// which fills in the brain, proxy, knowledge and vehicle handles.
		MemUtils::CallClassMethod<void, VehicleIdleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086B060, this, TableID, SMParams);

		// That call leaves the base's vtable in place, so the engine's own goes over it.
		MemUtils::InstallVTable(this, 0x00D76F00);
	}

	VehicleIdleSM::~VehicleIdleSM()
	{
		MemUtils::CallClassMethod<void, VehicleIdleSM*>(0x04AC1E0, this);
	}

	bool VehicleIdleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleIdleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x086B010, this, SimTime, FrameTime, TransID, TransData);
	}

	void VehicleIdleSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, VehicleIdleSM*, uint32_t, float, uint32_t>(0x086B0E0, this, SimTime, FrameTime, TableID);
	}

	int VehicleIdleSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, VehicleIdleSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* VehicleIdleSM::S_VehicleIdleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleIdleSM(InID, InSMParams);
	}

	/* static */
	void VehicleIdleSM::BuildStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("VehicleIdleST");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("VehicleIdleST", get_thread_new_allocator());

		// The idle update runs on entry as well as every frame, so a vehicle that has just been
		// parked settles in the same frame rather than on the next one.
		EARS::Framework::SMBuilderState* IdleState = Builder.AddState("idle", -1);
		IdleState->AddEnterMessage(MESSAGE_UPDATE_IDLE);
		IdleState->AddUpdateMessage(MESSAGE_UPDATE_IDLE);

		Builder.CompileAndRegister(kStateMachine_VehicleIdleSM, S_VehicleIdleSM_FactoryFn, "VehicleIdleSM");
	}
}
