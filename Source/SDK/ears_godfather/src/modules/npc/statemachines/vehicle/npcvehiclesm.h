#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCVehicleSM : public EARS::Modules::NPCStateMachine
	{
	public:

		NPCVehicleSM() = delete;
		NPCVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCVehicleSM();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0EE359985; }
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TODO: sizeof is the shipping build's, from the allocation in S_NPCVehicleSM_FactoryFn.
		// The debug build is 0xB4 here, so its member layout cannot be reused.
		// The padding also covers the secondary bases, RWS::CEventHandler and
		// EARS::WeaponUtils::IDamageListener, which are not modelled.
		char m_Padding[0x40];
	};
	static_assert(sizeof(NPCVehicleSM) == 0xB0);
}
