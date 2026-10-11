#include "base_vehicle.h"

#include "modules/vehicles/behaviors/vehiclebrain.h"

namespace EARS::Vehicles
{
    void BaseVehicle::SetAttachedBrain(EARS::Modules::IVehicleBrain* InBrain)
    {
        m_Brain = InBrain;
    }

    EARS::Modules::IVehicleBrain* BaseVehicle::GetAttachedBrain() const
    {
        return m_Brain.GetPtr();
    }
}
