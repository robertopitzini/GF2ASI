#pragma once

#include "framework/core/base/base.h"

/**
 * Which behaviour a vehicle's brain is currently running.
 *
 * At global scope in the original rather than inside EARS::Modules, and the _REF / _MAX_VALUE
 * bookends are the game's reflected-enum convention: _REF marks "no value", _MAX_VALUE bounds
 * the range. RootVehicleSM's pickBrain state turns one of these into the matching child table.
 */
enum BrainType
{
	BrainType_REF       = -1,
	IDLE_BRAIN          = 0,
	CIVILIAN_BRAIN      = 1,
	CHASEOFFSET_BRAIN   = 2,
	POLICE_PATROL_BRAIN = 3,
	MOB_VENUE_BRAIN     = 4,
	BrainType_MAX_VALUE = 5,
};

namespace EARS::Modules
{
	class IVehicleBrain : public EARS::Framework::Base
	{

	};

	class VehicleBrain : public IVehicleBrain
	{

	};
} // EARS::Modules
