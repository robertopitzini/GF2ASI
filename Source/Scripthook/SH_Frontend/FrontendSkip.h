#pragma once

namespace Mod
{
	/**
	 * Drops front end screens between launch and the main menu.
	 *
	 * Each screen is a UIFrontend::FrontendState whose entry function names the
	 * state that follows it, so a screen is skipped by standing in for that one
	 * entry function and naming the successor it would have reached. Three are
	 * offered, each independent of the others:
	 *
	 *  - EA_PARAMOUNT_LEGAL is the single entry point to the boot chain (it
	 *    names OPENING_MOVIE, which names LEGAL_SCREEN, which names
	 *    TITLE_SCREEN), so replacing it alone drops all three screens.
	 *
	 *  - TITLE_SCREEN waits for a button before StartButtonPressed picks what
	 *    comes next. Calling that directly carries the front end on without the
	 *    wait.
	 *
	 *  - UPDATE_CHECK is what StartButtonPressed names, so dropping the title
	 *    screen lands here rather than on a menu. It consults the patcher and
	 *    then either offers an update, sits on a progress message while it
	 *    downloads, or - with nothing to offer - hands straight to
	 *    PROFILE_SELECT. Only that last branch is reproduced, so the patcher is
	 *    never asked.
	 *
	 * PROFILE_SELECT is deliberately left alone: picking a profile is what
	 * drives the save-game setup state and the autosave notice after it, so
	 * jumping past it would reach the main menu with no profile loaded.
	 *
	 * Controlled by [Frontend] in gf2asi_settings.ini.
	 */
	class FrontendSkip
	{
	public:

		// Reads [Frontend] and redirects the states it asks for. Must run before
		// the front end does.
		static void StaticApplyHooks();
	};
}
