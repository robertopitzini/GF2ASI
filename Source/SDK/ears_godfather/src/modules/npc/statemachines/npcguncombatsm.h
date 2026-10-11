#pragma once

#include "modules/npc/statemachines/npcsm.h"
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	// These derive from NPCStateMachine, whose TRANSID_LAST is 18 and MESSAGE_LAST 35; that is
	// where each class below starts numbering its own IDs.

	class ReloadGunSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_ReloadGunSM = 0x77A4600C;

		ReloadGunSM() = delete;
		ReloadGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ReloadGunSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_ReloadGunSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_ReloadGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "ReloadGunST" state table. */
		static void BuildReloadGunStateTable();

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum ReloadGunSMTransID : uint32_t
		{
			TRANSID_RELOAD_DONE = 0x12,
			TRANSID_LAST        = 0x13,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum ReloadGunSMMessageID : uint32_t
		{
			MESSAGE_RELOAD_INIT   = 0x23,
			MESSAGE_RELOAD_UPDATE = 0x24,
			MESSAGE_LAST          = 0x25,
		};

		char m_Padding[0x4];
	};
	static_assert(sizeof(ReloadGunSM) == 0x74);

	class DrawGunSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_DrawGunSM = 0x3B6DBFA9;

		DrawGunSM() = delete;
		DrawGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~DrawGunSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_DrawGunSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_DrawGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "DrawGunST" state table. */
		static void BuildDrawGunStateTable();

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum DrawGunSMTransID : uint32_t
		{
			TRANSID_CAN_DRAW = 0x12,
			TRANSID_LAST     = 0x13,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum DrawGunSMMessageID : uint32_t
		{
			MESSAGE_DRAWGUN_INIT   = 0x23,
			MESSAGE_DRAWGUN_UPDATE = 0x24,
			MESSAGE_LAST           = 0x25,
		};

		char m_Padding[0x8];
	};
	static_assert(sizeof(DrawGunSM) == 0x78);

	class HolsterGunSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_HolsterGunSM = 0x2D48E99B;

		HolsterGunSM() = delete;
		HolsterGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~HolsterGunSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_HolsterGunSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_HolsterGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "HolsterGunST" state table. */
		static void BuildHolsterStateTable();

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum HolsterGunSMTransID : uint32_t
		{
			TRANSID_HOLSTER_DONE = 0x12,
			TRANSID_LAST         = 0x13,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum HolsterGunSMMessageID : uint32_t
		{
			MESSAGE_HOLSTER_INIT   = 0x23,
			MESSAGE_HOLSTER_UPDATE = 0x24,
			MESSAGE_LAST           = 0x25,
		};

		char m_Padding[0x4];
	};
	static_assert(sizeof(HolsterGunSM) == 0x74);
}
