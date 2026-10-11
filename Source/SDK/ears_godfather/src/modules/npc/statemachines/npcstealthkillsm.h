#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCStealthKillSM : public EARS::Modules::NPCStateMachine
	{
	public:

		NPCStealthKillSM() = delete;
		NPCStealthKillSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCStealthKillSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E4B1F57A; }
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCStealthKillSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCStealthKillSM_FactoryFn.
		char m_Padding[0x8];
	};
	static_assert(sizeof(NPCStealthKillSM) == 0x78);
}
