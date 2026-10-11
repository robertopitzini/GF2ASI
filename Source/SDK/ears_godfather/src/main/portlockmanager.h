#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace MainLoop
	{
		/**
		 * Owns the "which controller port is the player on" rules, and the dialogs
		 * the game puts up when that changes - a pad being unplugged, or a second
		 * pad pressing Start while one is already bound.
		 */
		class PortLockManager
		{
		public:

			/** Allow or suppress the port dialogs. The front end suppresses them for
			 *  the duration of the boot screens, where there is nothing to bind to
			 *  yet, and allows them again from the title screen onwards. */
			void SetIsDialogAllowed(const bool bInDialogAllowed);

			/** get game instance of this object */
			static PortLockManager* GetInstance();

		private:
		};

	} // MainLoop
} // EARS
