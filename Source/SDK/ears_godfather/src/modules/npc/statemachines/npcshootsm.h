#pragma once

#include "Platform/MemUtils.h"
#include "modules/npc/statemachines/npcsm.h"

namespace EARS::Modules
{
	// These derive from NPCStateMachine, whose TRANSID_LAST is 18 and MESSAGE_LAST 35; that is
	// where each class below starts numbering its own IDs.

	class ShootSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_ShootSM = 0x44BE8AE2;

		ShootSM() = delete;
		ShootSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ShootSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_ShootSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_ShootSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "ShootST" state table. */
		static void BuildShootStateTable();

		/** Bits of the SM's flag word. */
		enum ShootSMFlags : uint32_t
		{
			SHOOT_FLAG_DELAYED_START = 0x1,
			SHOOT_FLAG_SET_CROUCHING = 0x2,
		};

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum ShootSMTransID : uint32_t
		{
			TRANSID_HAS_MORE_BURSTS    = 0x12,
			TRANSID_CAN_BURST          = 0x13,
			TRANSID_EXIT_BASED_ON_GUN  = 0x14,
			TRANSID_TEST_MINIMUM_DELAY = 0x15,
			TRANSID_LAST               = 0x16,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum ShootSMMessageID : uint32_t
		{
			MESSAGE_PLAY_IDLE        = 0x23,
			MESSAGE_INCREMENT_BURSTS = 0x24,
			MESSAGE_LAST             = 0x25,
		};

		char m_Padding[0x20];
	};
	static_assert(sizeof(ShootSM) == 0x90);

	class ContinualBurstFireSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_ContinualBurstFireSM = 0x5EA8C0E7;

		ContinualBurstFireSM() = delete;
		ContinualBurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ContinualBurstFireSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_ContinualBurstFireSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_ContinualBurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "ContinualBurstFireST" state table. */
		static void BuildStateMachine();

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum ContinualBurstFireSMTransID : uint32_t
		{
			TRANSID_NO_GUN = 0x12,
			TRANSID_LAST   = 0x13,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum ContinualBurstFireSMMessageID : uint32_t
		{
			MESSAGE_FIRE_UPDATE = 0x23,
			MESSAGE_LAST        = 0x24,
		};

		char m_Padding[0x8];
	};
	static_assert(sizeof(ContinualBurstFireSM) == 0x78);

	class BurstFireSM : public EARS::Modules::NPCStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_BurstFireSM = 0x211BC9C1;

		BurstFireSM() = delete;
		BurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~BurstFireSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_BurstFireSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_BurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// One class, two tables: BurstFireST plays a firing animation per shot, BurstFireNoAnimST
		// fires without one. ShootSM spawns BurstFireST as a child.
		static void BuildBurstFireStateTable();
		static void BuildBurstFireNoAnimStateTable();

		/** Bits of the SM's flag word. */
		enum BurstFireSMFlags : uint32_t
		{
			FLAG_SHOT_FAILED = 0x1,
		};

	private:

		// TransIDs extending NPCSMTransID (base TRANSID_LAST = 18).
		enum BurstFireSMTransID : uint32_t
		{
			TRANSID_IS_FACING_TARGET    = 0x12,
			TRANSID_CCT_GUNFIRE_REACHED = 0x13,
			TRANSID_SHOTBLOCKED         = 0x14,
			TRANSID_BURST_DONE          = 0x15,
			TRANSID_GUN_READY           = 0x16,
			TRANSID_GUN_NOT_READY       = 0x17,
			TRANSID_LAST                = 0x18,
		};

		// MessageIDs extending NPCStateMachine's message enum (base MESSAGE_LAST = 35).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum BurstFireSMMessageID : uint32_t
		{
			MESSAGE_BURST_INIT                     = 0x23,
			MESSAGE_FIRE_PLAYANIM                  = 0x24,
			MESSAGE_BURSTFIRE_UPDATE               = 0x25,
			MESSAGE_FIRE_GUN_NO_ANIM               = 0x26,
			MESSAGE_INCREMENT_SHOT_COUNT           = 0x27,
			MESSAGE_PLAYIDLE                       = 0x28,
			MESSAGE_MARK_SHOTBLOCKED               = 0x29,
			MESSAGE_CHATTER_SHOTBLOCKED            = 0x2A,
			MESSAGE_CHATTER_SHOOT                  = 0x2B,
			MESSAGE_SET_WAIT_FOR_CCT_GUNFIRE_EVENT = 0x2C,
			MESSAGE_LAST                           = 0x2D,
		};

		char m_Padding[0x18];
	};
	static_assert(sizeof(BurstFireSM) == 0x88);
}
