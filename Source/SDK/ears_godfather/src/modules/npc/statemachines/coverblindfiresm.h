#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class CoverBlindFireSM : public EARS::Modules::NPCStateMachine
	{
	public:

		CoverBlindFireSM() = delete;
		CoverBlindFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CoverBlindFireSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1F18954E; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CoverBlindFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_CoverBlindFireSM_FactoryFn.
		char m_Padding[0x10];
	};
	static_assert(sizeof(CoverBlindFireSM) == 0x80);
}
