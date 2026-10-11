#pragma once

// SDK
#include "ears_common/array.h"
#include "ears_common/guid.h"
#include "ears_common/singleton.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class DemoPackage;

		/**
		 * The demo build's list of launchable packages, kept sorted by menu order.
		 *
		 * The pending package is the one the next LaunchGame should spawn into; it
		 * is held as a guid rather than a pointer because the package entity is
		 * destroyed along with the front end before the map it names is loaded.
		 */
		class DemoPackageManager : public RWS::CEventHandler, public Singleton<DemoPackageManager>
		{
		public:

			/** Nominate the package the next LaunchGame spawns into. */
			void SetPendingPackageByIndex(const uint32_t InIndex);

			void ClearPendingPackage();

			/** Nominate the package the next LaunchGame spawns into, or nullptr to
			 *  clear it. Always clears the active package, so the pending one is
			 *  what the next launch reads. */
			void SetPendingPackage(const DemoPackage* InNewPackage);

			/** resolve a package using a given index */
			const DemoPackage* GetPackage(const uint32_t Index) const { return m_Packages[Index]; }

			/** get the number of packages currently registered */
			uint32_t GetNumPackages() const { return m_Packages.Size(); }

			static DemoPackageManager* GetInstance();

		private:

			EARS::Common::guid128_t m_PendingPackageGuid;
			EARS::Common::guid128_t m_ActivePackageGuid;
			Array<EARS::Modules::DemoPackage*> m_Packages;
		};

		// The two bases carry a vtable pointer each, so the guids start at 0x10 -
		// which is where GetPendingPackage reads the pending one and RegisterPackage
		// finds the array at 0x30.
		static_assert(sizeof(DemoPackageManager) == 0x3C, "EARS::Modules::DemoPackageManager must equal 0x3C");

	} // Modules
} // EARS
