#pragma once

// SDK
#include "framework/core/animated/animated.h"

// forward declares
namespace EARS::Modules
{
	class IVehicleBrain;
}

namespace EARS::Vehicles
{
	class BaseVehicle : public EARS::Framework::Animated
	{
	public:

		struct VehicleLocator
		{
			uint32_t m_LocatorID = 0;
			EARS::Framework::Entity::BoneInfo m_BoneInfo;
		};

		// Brain API for vehicles
		void SetAttachedBrain(EARS::Modules::IVehicleBrain* InBrain);
		EARS::Modules::IVehicleBrain* GetAttachedBrain() const;
		bool IsBrainAttached() const { return m_Brain.IsValid(); }

		/**
		 * How many seat locators this vehicle's model declares, which is also
		 * the number of occupants it can carry. Everything that walks seats
		 * uses this as its bound, so a vehicle whose model has none can never
		 * be occupied.
		 */
		uint32_t GetNumSeatLocators() const { return m_NumSeatLocators; }

	private:

		SafePtr<EARS::Framework::Entity> m_VehicleDriver;						// 0x2B0
		SafePtr<EARS::Framework::Entity>* m_EntitySeatAssignments = nullptr;	// 0x2B8
		uint32_t m_NumSeatLocators = 0;											// 0x2BC
		EARS::Common::guid128_t m_BrainGuid;									// 0x2C0
		SafePtr<EARS::Modules::IVehicleBrain> m_Brain;
		bool m_bBaseVehicleInitialised = false;								// 0x2D8
	};

	static_assert(sizeof(BaseVehicle) == 0x2DC);
} // EARS::Vehicles

