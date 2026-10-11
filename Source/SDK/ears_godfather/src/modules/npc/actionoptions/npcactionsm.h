#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCActionSM : public EARS::Modules::NPCStateMachine
	{
	public:

		NPCActionSM() = delete;
		NPCActionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCActionSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x3601A991; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCActionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCActionSM_FactoryFn.
		char m_Padding[0x1C];
	};
	static_assert(sizeof(NPCActionSM) == 0x8C);
}
