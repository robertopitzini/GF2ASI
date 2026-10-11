#pragma once

#include "ears_common/rwtypes.h"
#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * Drives a vehicle to a target position, backing up and retrying whenever it gets stuck.
	 *
	 * Other vehicle behaviours use it as a child rather than pathing themselves: its table is what
	 * VehicleScriptedSequenceSM's pathToPos state spawns.
	 */
	class VehicleFollowPathStateMachine : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_VehicleFollowPathStateMachine = 0xB1C4C5B5;

		VehicleFollowPathStateMachine() = delete;
		VehicleFollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleFollowPathStateMachine();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_VehicleFollowPathStateMachine; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleFollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/**
		 * Describe and register the "vehicleFollowPathTable" state table.
		 *
		 * The original also builds "vehicleReverseTable" here, which the reverse state takes as a
		 * child; VehicleReverseStateMachine::BuildStateTable covers that, and both must be
		 * registered together or not at all.
		 */
		static void BuildStateTable();

		/** Bits of the SM's flag word. */
		enum VehicleFollowPathStateMachineFlags : uint32_t
		{
			PATHSMFLAGS_NOSEEKTOTARGET = 0x1,
			PATHSMFLAGS_NODESPASSEDIN  = 0x2,
			PATHSMFLAGS_ISSEEKING      = 0x4,
			PATHSMFLAGS_STOPATTARGET   = 0x10,
		};

	private:

		// TransIDs extending VehicleAiStateMachineTransID (base TRANSID_LAST = 11).
		enum VehicleFollowPathStateMachineTransID : uint32_t
		{
			TRANSID_INIT_SEEKTOTARGET = 0xB,
			TRANSID_SEEKTOTARGET_DONE = 0xC,
			TRANSID_LAST              = 0xD,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleFollowPathStateMachineMessageID : uint32_t
		{
			MESSAGE_SEEKTOTARGET_INIT                    = 0xA,
			MESSAGE_SEEKTOTARGET_UPDATE                  = 0xB,
			MESSAGE_PREPARE_FOR_SCRIPTED_SEQUENCE_TO_END = 0xC,
			MESSAGE_LAST                                 = 0xD,
		};

		// TODO: the setters (SetFinalTargetPos, SetStopAtTarget, SetPositionThresholdSq,
		// SetSpeedRegulationType, GetSpeedRegulationType) are not reconstructed. GetTransIDString
		// is overridden in the original but has no slot in the shipping vtable.

		RwV3d    m_FinalTargetPos;
		uint32_t m_PathSMFlags;
		float    m_PositionReachedThresholdSq;
		// EARS::Modules::VehicleSpeedRegulationType in the original; the enum is not reconstructed.
		uint32_t m_SpeedRegulationType;
		float    m_SpecifiedSpeed;
	};

	static_assert(sizeof(VehicleFollowPathStateMachine) == 0x7C);
}
