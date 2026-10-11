#pragma once

#include "modules/npc/statemachines/npcsm.h"

#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class AmbushSM : public EARS::Modules::NPCStateMachine
	{
	public:

		AmbushSM() = delete;
		AmbushSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~AmbushSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x9C39BACE; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_AmbushSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_AmbushSM_FactoryFn.
		char m_Padding[0x4];
	};
	static_assert(sizeof(AmbushSM) == 0x74);

	/**
	 * Common base for the NPC search behaviours.
	 *
	 * It holds the position being searched and whether the NPC can currently see it, and
	 * contributes the transitions that decide whether a search position is still worth walking to
	 * and the message that picks which pathing state machine to get there with.
	 */
	class BaseSearchSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_BaseSearchSM = 0x6F577FB3;

		BaseSearchSM() = delete;

		BaseSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
			: EARS::Modules::NPCStateMachine(TableID, SMParams)
		{
		}

		// DEVIATION: the original's destructor is not reconstructed. Each derived behaviour routes
		// construction and destruction through an engine stub that covers this layer.
		virtual ~BaseSearchSM() = default;

		/** Bits of the SM's flag word. */
		enum BaseSearchSMFlags : uint32_t
		{
			FLAG_CAN_SEE_SEARCH_POS = 0x1,
			LAST_BASE_SEARCH_FLAG   = 0x1,
		};

	protected:

		// Named by both derived behaviours' BuildStateTable, so these are protected rather than
		// private.

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum BaseSearchSMTransID : uint32_t
		{
			TRANSID_CAN_SEE_SEARCH_POS          = 0x12,
			TRANSID_CANNOT_SEE_SEARCH_POS       = 0x13,
			TRANSID_NOTHING_FOUND_AT_SEARCH_POS = 0x14,
			TRANSID_LAST                        = 0x15,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum BaseSearchSMMessageID : uint32_t
		{
			MESSAGE_UPDATE_SEARCH_TARGET = 0x23,
			MESSAGE_SELECT_PATHING_SM    = 0x24,
			MESSAGE_PLAY_SEARCH_FAILED   = 0x25,
			MESSAGE_LAST                 = 0x26,
		};

	private:

		// TODO: the members are a Flags32 at +0x70, the search position at +0x74 and the search
		// animation speed at +0x80, with IsBadLocation, CouldSeePos and ShouldStrafeWalk over
		// them. Not confirmed against the shipping build.
		//
		// The 0x84 total is confirmed: NPCSearchSM and NPCRandomSearchSM both start their own
		// members at +0x84, and the shipping sizes in their factories match the debug build.
		char m_Padding[0x14];
	};

	static_assert(sizeof(BaseSearchSM) == 0x84);

	class NPCRandomSearchSM : public EARS::Modules::BaseSearchSM
	{
	public:

		NPCRandomSearchSM() = delete;
		NPCRandomSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCRandomSearchSM();

		//~ Begin BaseSearchSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0CF840186; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End BaseSearchSM Interface

		static EARS::StateMachineSys::StateMachine* S_NPCRandomSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCRandomSearchSM_FactoryFn.
		char m_Padding[0x4];
	};
	static_assert(sizeof(NPCRandomSearchSM) == 0x88);

	class NPCSearchSM : public EARS::Modules::BaseSearchSM
	{
	public:

		NPCSearchSM() = delete;
		NPCSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCSearchSM();

		//~ Begin BaseSearchSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E653FF63; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End BaseSearchSM Interface

		static EARS::StateMachineSys::StateMachine* S_NPCSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCSearchSM_FactoryFn.
		char m_Padding[0x44];
	};
	static_assert(sizeof(NPCSearchSM) == 0xC8);
}
