#pragma once

// SDK
#include "ears_common/string.h"
#include "ears_physics/behaviors/vehicles/base_vehicle.h"

namespace EARS
{
	namespace Modules { class VehicleDamageComponent; class PresetInstance; class SimNPC; }

	namespace Vehicles
	{
		// forward declares
		class HavokWheeledVehicle;

		/**
		 * A Car in the world of Godfather II.
		 */
		class WhiteboxCar : public EARS::Vehicles::BaseVehicle
		{
		public:

			// One slot per seat locator; see BaseVehicle::GetNumSeatLocators.
			static const uint32_t MAX_OCCUPANTS = 4;

			// Getters
			EARS::Modules::VehicleDamageComponent* GetDamageComponent() const { return m_DamageComponent; }

			/**
			 * Occupants are pooled SimNPC records rather than live NPCs, one per
			 * seat, and each seat picks its own by the GenomeFilter whose guid
			 * sits in the matching slot: SetupOccupants resolves that guid
			 * through SimManager and asks SimNpcManager for a pooled SimNPC the
			 * filter accepts. A null slot therefore means either the filter
			 * resource is not loaded or the pool had nobody matching it.
			 */
			EARS::Modules::SimNPC* GetSimNPCAtSeat(const uint32_t InSeat) const { return m_SimNPC[InSeat]; }
			const EARS::Common::guid128_t& GetGenomeFilterGuidAtSeat(const uint32_t InSeat) const { return m_GuidGenomeFilter[InSeat]; }

		private:

			// WheeledVehicle
			char m_WheeledVehicle_Padding0[0x84];								// 0x2D9
			HavokWheeledVehicle* m_HavokWheeledVehicle = nullptr;				// 0x360
			char m_WheeledVehicle_Padding1[0x2C];
			int m_UsePhysics = 0;												// 0x390
			char m_WhiteboxCar_Padding0[0x42C];

			// WhiteboxCar
			EARS::Modules::VehicleDamageComponent* m_DamageComponent = nullptr; // 0x7C0
			char m_WhiteboxCar_Padding1A[0x288];
			EARS::Modules::SimNPC* m_SimNPC[MAX_OCCUPANTS];						// 0xA4C
			EARS::Common::guid128_t m_GuidGenomeFilter[MAX_OCCUPANTS];			// 0xA5C
			char m_WhiteboxCar_Padding1B[0x3C0];
			String m_PartsPresetName;											// 0xE5C
			String m_AssemblyName;												// 0xE6C
			EARS::Modules::PresetInstance* m_PresetInstance = nullptr;			// 0xE7C
			char m_WhiteboxCar_Padding2[0x88];
			EARS::Common::guid128_t m_ModelStrGuid;
			char m_WhiteboxCar_Padding3[0x8];

			// NB:
			//0x2D8 -> m_bBaseVehicleInitialised;
			//0x360 -> m_HavokWheeledVehicle
			//0x390 -> m_bUsePhysics
		};

		static_assert(sizeof(WhiteboxCar) == 0xF20, "EARS::Vehicles::WhiteboxCar must equal 0xF20");
	} // Vehicles
} // EARS
