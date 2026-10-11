#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class CoverPeekFireSM : public EARS::Modules::NPCStateMachine
	{
	public:

		CoverPeekFireSM() = delete;
		CoverPeekFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CoverPeekFireSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x5A546DE2; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CoverPeekFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_CoverPeekFireSM_FactoryFn.
		char m_Padding[0x1C];
	};
	static_assert(sizeof(CoverPeekFireSM) == 0x8C);
}
