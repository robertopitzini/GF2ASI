#pragma once

// SDK.h
#include "ears_common/guid.h"
#include "ears_common/string.h"
#include "framework/core/base/base.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * One entry in the demo build's level picker: a named map plus the spawn to
		 * drop the player at. These are entities in the game database, so a build
		 * only has packages if its data ships them - the retail data does not, and
		 * DemoPackageManager stays empty there.
		 *
		 * Registered with the manager on iMsgPostSpawnInitialize rather than in the
		 * constructor, so the attributes have been applied by the time the manager
		 * sorts it into the menu.
		 */
		class DemoPackage : public EARS::Framework::Base
		{
		public:

			/** resolve the name of this package */
			const String* GetName() const { return &m_Name; }

			const EARS::Common::guid128_t* GetInstanceID() const { return &m_InstanceID; }

			/** Where the player is placed when the package is launched. Compared
			 *  against the default spawn by UIFrontend::LaunchGame, which only
			 *  overrides the spawn when the two differ. */
			const EARS::Common::guid128_t* GetPlayerSpawnGuid() const { return &m_PlayerSpawnGuid; }

			/** Sort key for the popup - DemoPackageManager::RegisterPackage inserts
			 *  ahead of the first package with a higher order. */
			uint32_t GetMenuOrder() const { return m_MenuOrder; }

		private:

			EARS::Common::guid128_t m_InstanceID;

			String m_Name;

			RWS::CEventId m_MsgStart;

			EARS::Common::guid128_t m_PlayerSpawnGuid;

			uint32_t m_MenuOrder = 0;
		};

		// RegisterEntityClasses registers the class with this size.
		static_assert(sizeof(DemoPackage) == 0x8C, "EARS::Modules::DemoPackage must equal 0x8C");

	} // Modules
} // EARS
