#pragma once

#include "ears_common/rwtypes.h"
#include "framework/toolkits/statemachine/animatesm.h"

namespace EARS
{
	namespace Vehicles { class AICarBehavior; }

	namespace Modules
	{
		/**
		 * Common base for the AI behaviours that drive a vehicle to a location and stop it there.
		 *
		 * It owns the approach location and the steering toward it, and contributes the whole
		 * vocabulary for arriving: parked, close enough, stuck, forced stop, slow enough, hit a
		 * dead end, occupants out. A derived behaviour supplies only what is specific to why the
		 * vehicle is going there.
		 *
		 * It adds both transitions and messages, so derived behaviours number their own from
		 * TRANSID_LAST and MESSAGE_LAST below.
		 */
		class VehicleApproachPositionSM : public EARS::Framework::AnimateStateMachine
		{
		public:

			static constexpr uint32_t kStateMachine_VehicleApproachPositionSM = 0x203865D7;

			VehicleApproachPositionSM() = delete;

			VehicleApproachPositionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
				: EARS::Framework::AnimateStateMachine(TableID, SMParams)
			{
			}

			// DEVIATION: the original's destructor is not reconstructed. Each derived behaviour
			// routes construction and destruction through an engine stub that covers this layer.
			virtual ~VehicleApproachPositionSM() = default;

			/** Bits of the SM's flag word. */
			enum VehicleApproachPositionSMFlags : uint32_t
			{
				APPROACH_FLAG_FORCED_EXIT    = 0x1,
				APPROACH_FLAG_LOCATION_SET   = 0x2,
				APPROACH_FLAG_FULL           = 0x4,
				APPROACH_FLAG_EVENT_LISTENER = 0x8,
				// The lowercase l is the original's spelling, kept so a search for it matches.
				APPROACH_FLAG_INITIAlIZED    = 0x10,
				APPROACH_FLAG_ARRIVED        = 0x20,
				APPROACH_LAST                = 0x20,
			};

		protected:

			// Named by every derived behaviour's BuildStateTable, so these are protected rather
			// than private.

			// TransIDs extending AnimateSMTransID (base TRANSID_LAST = 10).
			enum VehicleApproachPositionSMTransID : uint32_t
			{
				TRANSID_PARKED                      = 0xA,
				TRANSID_CLOSE_TO_LOCATION           = 0xB,
				TRANSID_NO_APPROACH_LOCATION        = 0xC,
				TRANSID_HAS_APPROACH_LOCATION       = 0xD,
				TRANSID_VEHICLE_STUCK               = 0xE,
				TRANSID_FORCED_STOP                 = 0xF,
				TRANSID_VEHICLE_SLOW_ENOUGH         = 0x10,
				TRANSID_REVERSING_BUT_CLOSE_ENOUGH  = 0x11,
				TRANSID_HIT_DEADEND                 = 0x12,
				TRANSID_OCCUPANTS_EXITED            = 0x13,
				TRANSID_RULEBREAKER_AND_SLOW_ENOUGH = 0x14,
				TRANSID_LAST                        = 0x15,
			};

			// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10).
			// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
			enum VehicleApproachPositionSMMessageID : uint32_t
			{
				MESSAGE_FOLLOWGRAPH_UPDATE    = 0xA,
				MESSAGE_SET_TO_IDLE           = 0xB,
				MESSAGE_SET_APPROACH_LOCATION = 0xC,
				MESSAGE_URGENT_APPROACH       = 0xD,
				MESSAGE_DEBUG                 = 0xE,
				MESSAGE_EXIT_OCCUPANTS        = 0xF,
				MESSAGE_PULLING_OVER_INIT     = 0x10,
				MESSAGE_PULLING_OVER_UPDATE   = 0x11,
				MESSAGE_LAST                  = 0x12,
			};

			// The padding covers the secondary bases, EARS::WeaponUtils::IDamageListener and
			// EARS::Modules::IStateMachineEventListener, which are not modelled. The engine's
			// constructor installs their vtables and links the damage listener into its list
			// inside this range.
			char m_Padding[0x18];

			EARS::Vehicles::AICarBehavior* m_Vehicle;
			RwV3d                          m_ApproachLoc;
			uint32_t                       m_Flags;
			float                          m_Steering;

			// Squared. PoliceCarPatrolSM is the only behaviour that overrides the default.
			float                          m_CloseEnoughToActAsRuleBreakerSq;
		};

		static_assert(sizeof(VehicleApproachPositionSM) == 0x84);
	}
}
