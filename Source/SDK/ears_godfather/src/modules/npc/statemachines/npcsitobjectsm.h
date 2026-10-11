#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCSitObjectSM : public EARS::Modules::NPCStateMachine
	{
	public:

		NPCSitObjectSM() = delete;
		NPCSitObjectSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCSitObjectSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1E9BA656; }
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCSitObjectSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_NPCSitObjectSM_FactoryFn.
		// The padding also covers the secondary base LevelingScheduler::Task, which is not
		// modelled.
		char m_Padding[0x58];
	};
	static_assert(sizeof(NPCSitObjectSM) == 0xC8);
}
