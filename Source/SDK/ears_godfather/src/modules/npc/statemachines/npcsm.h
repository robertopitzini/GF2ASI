#pragma once

#include "ears_statemachine/statemachine.h"
#include "modules/sentient/statemachines/sentientsm.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class NPC;

		/**
		 * Common base for every NPC-driven state machine.
		 *
		 * It sits between SentientSM and the individual NPC behaviours and owns the vocabulary
		 * they share: the pain, chatter, scripted-sequence and corpse messages, and the target
		 * and interrupt transitions. Behaviours number their own IDs from TRANSID_LAST and
		 * MESSAGE_LAST below, which is why an NPC state machine's first transition is 0x12 and
		 * its first message 0x23 rather than continuing SentientSM's 10 and 18.
		 */
		class NPCStateMachine : public EARS::Modules::SentientSM
		{
		public:

			NPCStateMachine() = delete;

			NPCStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);

			// DEVIATION: the original's destructor is not reconstructed. Every derived state
			// machine routes construction and destruction through an engine stub that handles
			// this layer's fields, so nothing here is left dangling.
			virtual ~NPCStateMachine() = default;

			//~ Begin EARS::Modules::SentientSM Interface
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
			virtual void InitialiseChild(StateMachine& ChildMachine) override;
			//~ End EARS::Modules::SentientSM Interface

			/** accessor to fetch active NPC */
			EARS::Modules::NPC* GetNPC() const { return m_NPC; }

			/** Reason a behaviour was interrupted; stored on the SM and read by CheckTransition. */
			enum Interrupt : uint32_t
			{
				INTERRUPT_NONE            = 0x0,
				INTERRUPT_FLAG_FORCESTATE = 0x1,
			};

			/** Bits of the SM's flag word. */
			enum NPCSMFlags : uint32_t
			{
				NPCSMFLAG_TRIGGERED_SS = 0x1,
				NPCSMFLAG_COMBAT_FLEE  = 0x2,
			};

			/** Indices into the per-SM config option block. */
			enum NPCSMConfigOption : uint32_t
			{
				CONFIG__FLEE__SAFETY_DISTANCE      = 0x0,
				CONFIG__SURRENDER__SAFETY_DISTANCE = 0x1,
				CONFIG_NUM                         = 0x2,
			};

		protected:

			// Named by every derived behaviour's BuildStateMachine, so these are protected rather
			// than private.

			// TransIDs extending SentientSMTransID (base TRANSID_LAST = 10).
			enum NPCSMTransID : uint32_t
			{
				TRANSID_INTERRUPT                           = 0xA,
				TRANSID_HAS_COMBATBEHAVIOR                  = 0xB,
				TRANSID_ISDEAD                              = 0xC,
				TRANSID_NO_TARGET                           = 0xD,
				TRANSID_TARGET_OUT_OF_RANGE                 = 0xE,
				TRANSID_TESTANDCLEAR_INTERRUPT_AND_NOT_DEAD = 0xF,
				TRANSID_ISDEAD_AND_PLAYED_DEATH_ANIM        = 0x10,
				TRANSID_PAIN_IN_GRAB                        = 0x11,
				TRANSID_LAST                                = 0x12,
			};

			// MessageIDs extending SentientSM's message enum (base MESSAGE_LAST = 18).
			// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
			enum NPCSMMessageID : uint32_t
			{
				MESSAGE_EXIT_INTERRUPT             = 0x12,
				MESSAGE_SPAWNCORPSE                = 0x13,
				MESSAGE_UPDATECORPSE               = 0x14,
				MESSAGE_BEGIN_SCRIPTED_SEQUENCE    = 0x15,
				MESSAGE_DROP_SCRIPTED_ITEM         = 0x16,
				MESSAGE_PAIN_STIMULUS              = 0x17,
				MESSAGE_START_PAIRED               = 0x18,
				MESSAGE_PICK_THREATENING_ANIMATION = 0x19,
				MESSAGE_END_STUMBLE                = 0x1A,
				MESSAGE_START_STARTLE              = 0x1B,
				MESSAGE_USE_RAYCASTCOLLISION       = 0x1C,
				MESSAGE_START_DODGE                = 0x1D,
				MESSAGE_BEGIN_COWER                = 0x1E,
				MESSAGE_TURN_TO_TARGET_ENTITY      = 0x1F,
				MESSAGE_PLAY_STATE_CHATTER         = 0x20,
				MESSAGE_PLAY_CHATTER               = 0x21,
				MESSAGE_ADVANCE_MELEE_ATTACK       = 0x22,
				MESSAGE_LAST                       = 0x23,
			};

		private:

			// TODO: the members are a Flags32 at +0x58, an NPC* at +0x5C, an Interrupt at +0x60, a
			// float state time limit at +0x64 and a SafePtr<Entity> target at +0x68, and the class
			// adds PlayStateChatter, GetConfigOptions and CombatInit to the vtable. None of that is
			// confirmed against the shipping build, so the padding stays; the 0x70 total is
			// confirmed, from the allocation size in each derived behaviour's factory.
			Flags32 m_Flags;
			EARS::Modules::NPC* m_NPC = nullptr;
			Interrupt m_Interrupt;
			float m_StateTimeLimit = 0.0f;
			SafePtr<EARS::Framework::Entity> m_Entity;

			static constexpr uint32_t kStateMachine_NPCStateMachine = 0x496F9653;
		};

		static_assert(sizeof(NPCStateMachine) == 0x70);
	}
}
