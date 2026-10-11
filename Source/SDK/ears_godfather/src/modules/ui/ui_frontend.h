#pragma once

// CPP
#include <stdint.h>

// SDK
#include "ears_common/bitflags.h"

namespace EARS::Apt
{
	/**
	 * The front end screen flow - everything from the language picker through the
	 * boot movies to the main menu.
	 *
	 * Each screen is a FrontendState with an entry function in s_aStateData. A
	 * screen does not call the next one: it writes the state it wants into
	 * s_eNextState and queues a sequence with AddNextStateSequence, and when that
	 * sequence fires GoToNextState calls ExecuteNextState, which looks the state
	 * up and calls its entry function. So the flow is a chain of "whoever is on
	 * screen names its successor", not a table of transitions.
	 */
	class UIFrontend
	{
	public:

		/** s_flags. Only the bits the reconstructed code reads or writes are named. */
		enum class UIFrontendFlags : uint32_t
		{
			// Raised by AddNextStateSequence and lowered again once the queued
			// sequence has fired, so a second request while one is outstanding is
			// dropped rather than queued twice. Every entry function tests it before
			// asking for its successor.
			FE_FLAG_STATE_SEQUENCE_QUEUED = 0x1,

			// MAIN_MENU raises it when it was entered from IN_GAME_TO_ONLINE, and
			// LOAD_GAME then hands off to RESTORE_ONLINE_DATA rather than coming up
			// on a menu. LEGAL_SCREEN lowers it on the way round the front end loop.
			FE_FLAG_RETURNING_FROM_ONLINE = 0x2,

			// ExecuteNextState returns without doing anything while this is set.
			// RESTART_FE_LOOP raises it for the duration of the restart.
			FE_FLAG_BLOCK_STATE_CHANGES = 0x10,

			// LOAD_GAME hands off to ONLINE_SCREEN rather than MAIN_MENU while this
			// is set, which is how the online menu survives a load. Raised by the
			// main menu's load item and by MAIN_MENU itself when it was entered from
			// START_ONLINE_GAME or IN_GAME_TO_ONLINE; lowered by TITLE_SCREEN and on
			// every other main-menu selection.
			FE_FLAG_RESUME_ONLINE_SCREEN = 0x400,

			// WAIT_FOR_PDLC skips its wait message while this is set.
			// TITLE_SCREEN lowers it alongside FE_FLAG_RESUME_ONLINE_SCREEN.
			FE_FLAG_PDLC_WAIT_DONE = 0x4000,
		};

		/** StateData::m_Flags. */
		enum class FrontendStateFlags : uint32_t
		{
			// ExecuteNextState passes the inverse of this to
			// PortLockManager::SetIsDialogAllowed, so a state carrying it runs with
			// the controller-port dialogs suppressed. Set on the boot screens, which
			// run before any pad is bound, and on nothing after the title screen.
			FE_STATE_FLAG_NO_PORT_DIALOGS = 0x1,
		};

		/**
		 * Values are the index into s_aStateData, so they are the PC build's and
		 * not the 360's.
		 *
		 * The PC build inserted eleven states of its own - a Plasma sign-in screen,
		 * a downloadable-content wait, a bonus movie, a return-to-title transition,
		 * the whole user-profile group and the patch check - so everything after
		 * each insertion shifted up. The counts line up exactly (26 states on the
		 * 360 plus eleven insertions), and the two builds' tables were aligned by
		 * following which state each one names as its successor, which is why the
		 * names below can be carried over from the 360 build's symbols at all.
		 */
		enum class FrontendState : uint32_t
		{
			NONE = 0,

			// Reads Locale out of the registry, maps it to a shipped language and
			// either goes on to LOAD_TEXT or straight to EA_PARAMOUNT_LEGAL.
			LANGUAGE_SELECT = 1,

			LOAD_TEXT = 2,

			// The three boot screens, in order. Each one names the next, so
			// EA_PARAMOUNT_LEGAL is the single entry point to all three.
			EA_PARAMOUNT_LEGAL = 3,
			OPENING_MOVIE = 4,
			LEGAL_SCREEN = 5,

			// Waits for any button, then StartButtonPressed picks the state after it.
			// Times out into ATTRACT_MOVIE, which comes back here.
			TITLE_SCREEN = 6,
			ATTRACT_MOVIE = 7,

			// PC only, and inserted ahead of SIGN_IN_CHECK rather than after it:
			// holds a "$dlc_wait_for_pdlc" message until the premium content is
			// resolved, then hands off to PROFILE_SELECT.
			WAIT_FOR_PDLC = 8,

			// Almost nothing is left of this on PC - there is no console sign-in to
			// check, so what remains raises a flag and hands off to WAIT_FOR_PDLC.
			SIGN_IN_CHECK = 9,

			// Its completion callback names PLASMA.
			BOOTUP_CHECK = 10,

			// The 360 build's trophy/content install slot. On PC it is entered from
			// the profile popup once a profile is chosen and its sequence hands off
			// to AUTOSAVE_NOTICE, so it is the save-game setup step here.
			TROPHY_INSTALL = 11,

			// Connects to Plasma, then hands off to LOAD_GAME. Its connect sequence
			// is what routes to PLASMA_SIGN_IN, TERMS_OF_SERVICE, CREATE_ACCOUNT,
			// ONLINE_SCREEN or KEYMASTER_CHECK depending on what the account needs.
			PLASMA = 12,

			// PC only - the e-mail and password prompt the console builds get from
			// the platform instead.
			PLASMA_SIGN_IN = 13,

			TERMS_OF_SERVICE = 14,
			CREATE_ACCOUNT = 15,

			// Hands off to MAIN_MENU, to ONLINE_SCREEN when
			// FE_FLAG_RESUME_ONLINE_SCREEN is set, or to RESTORE_ONLINE_DATA when
			// FE_FLAG_RETURNING_FROM_ONLINE is.
			LOAD_GAME = 16,

			MAIN_MENU = 17,

			OPTIONS = 18,
			CREDITS = 19,

			// PC only - plays deadspace_trailer.vp6 and returns to MAIN_MENU. The
			// main menu's bonus-features item.
			BONUS_MOVIE = 20,

			// Nothing writes this state on PC; the shipping build reaches the gamma
			// ramp from the options screen instead, and only the 360 build's
			// sign-in callback entered it as a state.
			GAMMA_CALIBRATION = 21,

			// On the shipping PC build this is a bare thunk to StartNewGame; the
			// demo-package popup that the 360 debug build puts here was compiled out.
			START_NEW_GAME = 22,

			// Rebuilds the crew's weapon licences and launches the online menu, or
			// shows "$mp_online_need_crew" and returns to MAIN_MENU when the player
			// has no crew.
			ONLINE_SCREEN = 23,

			START_ONLINE_GAME = 24,

			// All three are entered from gameplay code rather than from the front
			// end, which is why nothing in here names them. Each one does no more
			// than name where the front end should come up and queue the transition.
			IN_GAME_TO_ONLINE = 25,

			// PC only, and the odd one out of the three: it names LEGAL_SCREEN, so
			// it takes the front end back round to the title screen rather than to
			// a menu.
			IN_GAME_TO_TITLE = 26,

			IN_GAME_TO_MAIN = 27,

			RESTORE_ONLINE_DATA = 28,

			// The multiplayer entitlement code. Reached from the main menu's
			// multiplayer item once Plasma is connected, and from the Plasma connect
			// sequence; it puts up the "$fe_options_code_title" prompt.
			KEYMASTER_CHECK = 29,

			// Tears the front end down and brings it back up on the state
			// RestartFrontEndLoopToState stashed, which is how gameplay code returns
			// to a specific screen. Raises FE_FLAG_BLOCK_STATE_CHANGES so nothing
			// else can move the front end while it restarts.
			RESTART_FE_LOOP = 30,

			// 31 through 36 are PC only. The profile group is the PC build's user
			// handling, which the console builds get from the platform instead.
			//
			// ONLINE_CREW_CHECK is the same crew guard ONLINE_SCREEN opens with,
			// split out so the main menu's continue and load items can run it before
			// connecting to Plasma.
			ONLINE_CREW_CHECK = 31,

			// Where the front end lands after UPDATE_CHECK on boot, and where the
			// main menu's change-user item goes. The popup it raises is what the
			// other three profile states hang off.
			PROFILE_SELECT = 32,
			PROFILE_DELETE = 33,
			PROFILE_DELETE_CONFIRM = 34,
			PROFILE_CREATE = 35,

			// "$pc_autosave_turned_on", then MAIN_MENU.
			AUTOSAVE_NOTICE = 36,

			// What StartButtonPressed selects. PC only - it offers the game's own
			// patch if one is pending, then hands off to PROFILE_SELECT.
			UPDATE_CHECK = 37,
		};

		/** Entry functions are statics taking the singleton explicitly, so a state
		 *  can be entered without the instance being the implicit receiver. */
		using StateFunc = void(__cdecl*)(EARS::Apt::UIFrontend*, EARS::Apt::UIFrontend::FrontendState);

		struct StateData
		{
			FrontendState m_State = FrontendState::NONE;
			StateFunc m_Func = nullptr;
			uint32_t m_Flags = 0;
		};

		static_assert(sizeof(StateData) == 0xC, "UIFrontend::StateData must equal 0xC");

		// s_aStateData is indexed by the state value itself and carries an entry for
		// NONE, so the count is one more than the highest state.
		static const uint32_t NUM_STATES = 38;

		/** Look up the state s_eNextState names and call its entry function, after
		 *  recording the state being left as the current one. Does nothing while
		 *  FE_FLAG_BLOCK_STATE_CHANGES is set. */
		void ExecuteNextState();

		/** Queue the transition to whatever SetNextState last named, after a delay
		 *  in milliseconds. Dropped when a transition is already queued. */
		void AddNextStateSequence(int32_t InDelayTimeMS);

		/** Tear the front end down and start the game: installs the DLC features,
		 *  hides the menu and leaves the front end main loop. */
		void StartNewGame();

		/** What the title screen runs once any button is pressed - stops the attract
		 *  video, hides the title and legal text and names the state that follows
		 *  the title screen. Does not itself queue the transition. */
		static void StartButtonPressed();

		/** Bring the front end back up on InState, by stashing it and running
		 *  RESTART_FE_LOOP. Calls ExecuteNextState directly rather than queueing a
		 *  sequence, so this is for gameplay code asking for a screen and not for a
		 *  state entry function naming its own successor. */
		void RestartFrontEndLoopToState(FrontendState InState);

		/** Entry function for START_NEW_GAME. */
		static void EnterState_StartNewGame(EARS::Apt::UIFrontend* InThis, FrontendState InPreviousState);

		static void SetNextState(FrontendState InState);

		static FrontendState GetNextState();

		/** The state whose entry function last ran, which is what an entry function
		 *  receives as its InPreviousState argument. */
		static FrontendState GetCurrentState();

		static Flags32& GetFlags();

		/** get game instance of this object */
		static UIFrontend* GetInstance();

		//=====================================================================
		// Not part of the original class.
		//
		// The table the game dispatches through is const data in the image, so a
		// mod cannot redirect a state in place. CopyStates takes a writable copy
		// of it and PatchState edits that copy; the copy is only reached by the
		// reimplemented ExecuteNextState above, so a detour on the game's
		// ExecuteNextState is what makes either of them take effect.
		//=====================================================================

		/** Redirect one state's entry function. Keeps the state's flags. */
		static void PatchState(FrontendState InState, StateFunc InFunction);

		/** The entry function a state had before any PatchState call, so a
		 *  replacement can still chain to the original. */
		static StateFunc GetOriginalStateFunc(FrontendState InState);

		/** Take the writable copy of the image's table. Call once, before the
		 *  detour on ExecuteNextState can fire. */
		static void CopyStates();

	private:

		static void SetCurrentState(FrontendState InState);
	};
}
