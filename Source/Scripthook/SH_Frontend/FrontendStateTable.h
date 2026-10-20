#pragma once

namespace Mod
{
	/**
	 * Makes UIFrontend's screen table redirectable.
	 *
	 * The table the game dispatches through is const data in the image, so a
	 * state cannot be pointed somewhere else in place. UIFrontend::CopyStates
	 * takes a writable copy and UIFrontend::PatchState edits it, but only the
	 * SDK's reimplementation of ExecuteNextState reads that copy - until the
	 * game's own ExecuteNextState is detoured onto it, PatchState changes
	 * nothing.
	 *
	 * Both halves are installed together here, so a feature that redirects a
	 * state only has to ask for this first.
	 */
	class FrontendStateTable
	{
	public:

		/**
		 * Take the writable copy and detour the dispatch onto it. Idempotent, so
		 * every caller can ask without knowing whether another already has.
		 *
		 * Must run before the front end does, which Init_AttachHooks satisfies.
		 */
		static void Install();
	};
}
