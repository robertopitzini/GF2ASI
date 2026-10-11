#pragma once

#include "modules/npc/statemachines/npcsm.h"

#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class CoverDashSM : public EARS::Modules::NPCStateMachine
	{
	public:

		CoverDashSM() = delete;
		CoverDashSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CoverDashSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x14CBC763; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CoverDashSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_CoverDashSM_FactoryFn.
		char m_Padding[0x18];
	};
	static_assert(sizeof(CoverDashSM) == 0x88);

	class CoverPeekSM : public EARS::Modules::NPCStateMachine
	{
	public:

		CoverPeekSM() = delete;
		CoverPeekSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CoverPeekSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0A4AC966C; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CoverPeekSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_CoverPeekSM_FactoryFn.
		char m_Padding[0x4];
	};
	static_assert(sizeof(CoverPeekSM) == 0x74);
}
