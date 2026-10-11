#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * Common base for the state machines that move an NPC along a path.
		 *
		 * It owns the pathing setup the behaviours share: the target position and proximity, the
		 * strafe component and its direction, the idle and move animation functors, and the
		 * dynamic blocker that keeps other NPCs from routing through this one.
		 *
		 * It adds messages but no transitions, so a derived behaviour numbers its own messages
		 * from MESSAGE_LAST below and its own transitions from AnimateSMTransID::TRANSID_LAST.
		 */
		class PathingBaseSM : public EARS::Framework::AnimateStateMachine
		{
		public:

			static constexpr uint32_t kStateMachine_PathingBaseSM = 0x2036D128;

			PathingBaseSM() = delete;

			PathingBaseSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
				: EARS::Framework::AnimateStateMachine(TableID, SMParams)
			{
			}

			// DEVIATION: the original's destructor is not reconstructed. Each derived behaviour
			// routes construction and destruction through an engine stub that covers this layer.
			virtual ~PathingBaseSM() = default;

			/** Bits of the SM's flag word. */
			enum PathingBaseSMFlags : uint32_t
			{
				BASEPATHING_FLAG_STRAFE                       = 0x1,
				BASEPATHING_FLAG_STRAFE_WALK                  = 0x2,
				BASEPATHING_FLAG_GO_LEFT                      = 0x4,
				BASEPATHING_FLAG_USETURNANIMS                 = 0x8,
				BASEPATHING_FLAG_PATHING_SET                  = 0x10,
				BASEPATHING_FLAG_CAN_COMPUTE_AVOIDANCE_DIR    = 0x20,
				BASEPATHING_FLAG_DONT_DISABLE_HT              = 0x40,
				PATHSMFLAGS_FLAG_DISABLED_HT                  = 0x80,
				BASEPATHING_FLAG_FULL_PROXY_ENABLED           = 0x100,
				BASEPATHING_FLAG_STOP_EXACTLY                 = 0x200,
				BASEPATHING_FLAG_GRAPHCLIENT_BLOCKER_DISABLED = 0x400,
				BASEPATHING_LAST                              = 0x400,
			};

		protected:

			// Named by every derived behaviour's BuildStateMachine, so this is protected rather
			// than private.

			// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10).
			// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
			enum PathingBaseSMMessageID : uint32_t
			{
				MESSAGE_INIT_PATHING                     = 0xA,
				MESSAGE_DISABLE_HEADTRACKING_IN_STRAFE   = 0xB,
				MESSAGE_ENABLE_HEADTRACKING_AFTER_STRAFE = 0xC,
				MESSAGE_SEND_BLOCKER                     = 0xD,
				MESSAGE_LAST                             = 0xE,
			};

		private:

			// TODO: the members run from +0x58 to +0xF3 — the Sentient and StrafeComponent
			// pointers, the final target position and proximity, the strafe direction and angle,
			// the idle and move functors, the flag word, the dynamic blocker, the character proxy
			// and the turn animation type. The class also has three protected float constants
			// (CLOSE_ENOUGH_TO_NODE, MAX_TIME_BLOCKER_STUCK, CLOSE_ENOUGH_TURN_ANGLE_COS) and a
			// large setter surface. None of that is confirmed against the shipping build.
			//
			// The padding also covers the secondary base SafeObj, which is not modelled.
			//
			// The 0xF4 total is confirmed: NPCBackupSM and SeekToPositionSM both start their own
			// members at +0xF4, and the shipping sizes in their factories match the debug build.
			char m_Padding[0xA4];
		};

		static_assert(sizeof(PathingBaseSM) == 0xF4);
	}
}
