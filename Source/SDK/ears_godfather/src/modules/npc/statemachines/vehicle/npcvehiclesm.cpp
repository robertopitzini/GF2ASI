#include "npcvehiclesm.h"

namespace EARS::Modules
{
	NPCVehicleSM::NPCVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::NPCStateMachine(TableID, SMParams)
	{
	}

	NPCVehicleSM::~NPCVehicleSM()
	{
	}

	EARS::StateMachineSys::StateMachine* NPCVehicleSM::S_NPCVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCVehicleSM(InID, InSMParams);
	}
}
