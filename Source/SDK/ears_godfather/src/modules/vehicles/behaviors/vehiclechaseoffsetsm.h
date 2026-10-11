#pragma once

#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * Chases a target vehicle, holding an offset beside or behind it rather than tailgating.
	 *
	 * The largest of the vehicle behaviours: it rams, cuts off, blocks, reverses out of being
	 * stuck, stops when the target does, makes its occupants get out, and teleports itself when
	 * the chase runs off-camera. The stuck timer runs in every driving state, so any of them can
	 * hand over to the shared reverse child.
	 */
	class VehicleChaseOffsetStateMachine : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_VehicleChaseOffsetStateMachine = 0xDFD3C4CB;

		VehicleChaseOffsetStateMachine() = delete;
		VehicleChaseOffsetStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleChaseOffsetStateMachine();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_VehicleChaseOffsetStateMachine; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child) override;
		virtual void RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleChaseOffsetStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/**
		 * Describe and register the "vehicleChaseOffsetTable" state table.
		 *
		 * The reverse state takes "vehicleReverseTable" as a child but does not build it: the
		 * original relies on VehicleFollowPathStateMachine having registered it already, which it
		 * does from a different branch of the tree. Registering this table alone leaves the
		 * reverse child unresolved.
		 */
		static void BuildStateTable();

		/** Bits of the SM's flag word. */
		enum VehicleChaseOffsetStateMachineFlags : uint32_t
		{
			FLAG_IS_FIRST_CALC_TARGET        = 0x1,
			FLAG_TELEPORT_NECESSARY          = 0x2,
			FLAG_LAST_CAN_REACH_CHECK_RESULT = 0x8,
			FLAG_LAST_TARGET_POS_VALID       = 0x10,
			FLAGS_REQUEST_NEW_OFFSET         = 0x20,
			FLAGS_SHOULD_RAM_PLAYER          = 0x40,
			FLAGS_SHOULD_CUT_PLAYER_OFF      = 0x80,
			FLAGS_OVERTAKE_TO_CUT_OFF        = 0x100,
			FLAGS_HEAD_ON_CUT_OFF            = 0x200,
			FLAGS_RAM_STEER_AWAY             = 0x400,
		};

	protected:

		//~ Begin VehicleAiStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual const EARS::StateMachineSys::StateMachineSnapshot* ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End VehicleAiStateMachine Interface

	private:

		// TransIDs extending VehicleAiStateMachineTransID (base TRANSID_LAST = 11).
		enum VehicleChaseOffsetStateMachineTransID : uint32_t
		{
			TRANSID_INIT_FOLLOWENTITY         = 0xB,
			TRANSID_INIT_DONE                 = 0xC,
			TRANSID_INIT_SEEK                 = 0xD,
			TRANSID_SEEK_FOLLOWENTITY         = 0xE,
			TRANSID_SEEK_MANEUVER             = 0xF,
			TRANSID_SEEK_DONE                 = 0x10,
			TRANSID_FOLLOWENTITY_DONE         = 0x11,
			TRANSID_TELEPORT                  = 0x12,
			TRANSID_CLOSEBY                   = 0x13,
			TRANSID_BLOCK                     = 0x14,
			TRANSID_TELEPORT_DISABLED         = 0x15,
			TRANSID_MANEUVER_SEEK             = 0x16,
			TRANSID_MANEUVER_FOLLOWENTITY     = 0x17,
			TRANSID_MANEUVER_DONE             = 0x18,
			TRANSID_FOLLOW_TO_REVERSE_OR_FLIP = 0x19,
			TRANSID_SLOWENOUGHTOREVERSE       = 0x1A,
			TRANSID_ITSKILLINGTIME            = 0x1B,
			TRANSID_CLOSE_TO_TARGET_STOPPED   = 0x1C,
			TRANSID_HAS_STOPPED               = 0x1D,
			TRANSID_TARGET_FLED               = 0x1E,
			TRANSID_LAST                      = 0x1F,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleChaseOffsetStateMachineMessageID : uint32_t
		{
			MESSAGE_INIT_INIT                 = 0xA,
			MESSAGE_FOLLOWENTITY_INIT         = 0xB,
			MESSAGE_FOLLOWENTITY_UPDATE       = 0xC,
			MESSAGE_UPDATE_CHASE              = 0xD,
			MESSAGE_SEEK_INIT                 = 0xE,
			MESSAGE_SEEK_UPDATE               = 0xF,
			MESSAGE_TELEPORT_ENTER            = 0x10,
			MESSAGE_TELEPORT_EXIT             = 0x11,
			MESSAGE_DONE_INIT                 = 0x12,
			MESSAGE_KILL                      = 0x13,
			MESSAGE_STUCK_TIMER               = 0x14,
			MESSAGE_MANEUVER_INIT             = 0x15,
			MESSAGE_MANEUVER_UPDATE           = 0x16,
			MESSAGE_UPDATE_BLOCK              = 0x17,
			MESSAGE_REVERSE_ENTER             = 0x18,
			MESSAGE_SLOWDOWN_ENTER            = 0x19,
			MESSAGE_SLOWDOWN_UPDATE           = 0x1A,
			MESSAGE_EXIT_OCCUPANTS            = 0x1B,
			MESSAGE_UPDATE_STOPPED            = 0x1C,
			MESSAGE_FORCE_TARGET_ON_OCCUPANTS = 0x1D,
			MESSAGE_LAST                      = 0x1E,
		};

		// TODO: the members run from +0x6C to +0xBB -- the flag word, the last and last-calculated
		// target positions, the left/right and front/back offsets, and the timers for spawning
		// pedestrians, being stuck, being parked or behind off camera, manoeuvring, slowing down
		// and forcing a target on the occupants.
		//
		// The padding also covers the secondary base EARS::WeaponUtils::IDamageListener, which is
		// not modelled.
		char m_Padding[0x5C];
	};
	static_assert(sizeof(VehicleChaseOffsetStateMachine) == 0xBC);
}
