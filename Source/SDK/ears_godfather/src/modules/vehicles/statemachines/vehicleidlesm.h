#pragma once

#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * What a vehicle runs when its brain has nothing for it to do.
	 *
	 * A single state with no transitions out: the idle update is all there is, and only a brain
	 * change gets the vehicle out, via RootVehicleSM's TRANSID_BRAIN_CHANGE.
	 */
	class VehicleIdleSM : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_VehicleIdleSM = 0x3E3B465A;

		VehicleIdleSM() = delete;
		VehicleIdleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleIdleSM();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_VehicleIdleSM; }
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleIdleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "VehicleIdleST" state table. */
		static void BuildStateTable();

	protected:

		//~ Begin VehicleAiStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		//~ End VehicleAiStateMachine Interface

	private:

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleIdleSMMessageID : uint32_t
		{
			MESSAGE_UPDATE_IDLE = 0xA,
			MESSAGE_LAST        = 0xB,
		};

		// The class adds no members and no transitions of its own.
	};
	static_assert(sizeof(VehicleIdleSM) == 0x60);
}
