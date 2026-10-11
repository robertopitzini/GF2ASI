#pragma once

#include "ears_common/rwtypes.h"
#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * Backs a vehicle away from whatever it is stuck against.
	 *
	 * VehicleFollowPathStateMachine spawns its table as a child on TRANSID_ISSTUCK. It gives up
	 * after 2.5 seconds whether or not it got anywhere, so a vehicle wedged against geometry
	 * retries the approach rather than reversing forever.
	 */
	class VehicleReverseStateMachine : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_VehicleReverseStateMachine = 0x72703570;

		VehicleReverseStateMachine() = delete;
		VehicleReverseStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleReverseStateMachine();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_VehicleReverseStateMachine; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleReverseStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "vehicleReverseTable" state table. */
		static void BuildStateTable();

	private:

		// TransIDs extending VehicleAiStateMachineTransID (base TRANSID_LAST = 11).
		enum VehicleReverseStateMachineTransID : uint32_t
		{
			TRANSID_DONE = 0xB,
			TRANSID_LAST = 0xC,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleReverseStateMachineMessageID : uint32_t
		{
			MESSAGE_REVERSE_UPDATE = 0xA,
			MESSAGE_LAST           = 0xB,
		};

		// TODO: SetDesiredTarget and the protected ReverseUpdate are not reconstructed.
		// GetTransIDString is overridden in the original but has no slot in the shipping vtable.

		RwV3d m_DesiredTarget;
		bool  m_bDone;
	};

	static_assert(sizeof(VehicleReverseStateMachine) == 0x70);
}
