#include "FrontendSkip.h"

// Scripthook
#include "Scripthook/SH_Frontend/FrontendStateTable.h"

// Addons
#include "Addons/ConfigFile.h"
#include "Addons/tConsole.h"

// SDK
#include "modules/ui/ui_frontend.h"

// C++
#include <stdint.h>

// Everything here is one class's states, so the shorthand is worth it.
using EARS::Apt::UIFrontend;

namespace
{
	/**
	 * [Frontend] in gf2asi_settings.ini. The member initialisers are the
	 * defaults, so a key that is not in the file leaves that screen alone.
	 */
	struct FrontendTuning
	{
		bool bSkipIntro = false;
		bool bSkipTitleScreen = false;
		bool bSkipUpdateCheck = false;
	};

	FrontendTuning Tuning;

	void LoadTuning()
	{
		const Mod::ConfigFile& File = Mod::Config::SettingsFile();

		Tuning.bSkipIntro = File.GetBool(L"Frontend", L"SkipIntro", Tuning.bSkipIntro);
		Tuning.bSkipTitleScreen = File.GetBool(L"Frontend", L"SkipTitleScreen", Tuning.bSkipTitleScreen);
		Tuning.bSkipUpdateCheck = File.GetBool(L"Frontend", L"SkipUpdateCheck", Tuning.bSkipUpdateCheck);
	}

	// Stands in for EnterState_EAParamountLegal, whose own successor chain is the
	// EA/Paramount card, the opening movie and the legal screen.
	void __cdecl EnterState_SkipIntroScreens(UIFrontend* pThis, UIFrontend::FrontendState ePreviousState)
	{
		UIFrontend::SetNextState(UIFrontend::FrontendState::TITLE_SCREEN);
		pThis->AddNextStateSequence(0);
	}

	// Stands in for EnterState_TitleScreen. What is deliberately not reproduced
	// from it is the title text, the attract-movie timeout and the controller
	// setup its input sequence needs - none of which has a reader left once the
	// screen never appears. The two flags it lowers do have readers after it, so
	// those are cleared here.
	void __cdecl EnterState_SkipTitleScreen(UIFrontend* pThis, UIFrontend::FrontendState ePreviousState)
	{
		UIFrontend::GetFlags().Clear(
			(uint32_t)UIFrontend::UIFrontendFlags::FE_FLAG_RESUME_ONLINE_SCREEN |
			(uint32_t)UIFrontend::UIFrontendFlags::FE_FLAG_PDLC_WAIT_DONE);

		UIFrontend::StartButtonPressed();
		pThis->AddNextStateSequence(0);
	}

	// Stands in for the branch of the patch check that has nothing to offer.
	void __cdecl EnterState_SkipUpdateCheck(UIFrontend* pThis, UIFrontend::FrontendState ePreviousState)
	{
		UIFrontend::SetNextState(UIFrontend::FrontendState::PROFILE_SELECT);
		pThis->AddNextStateSequence(0);
	}
}

void Mod::FrontendSkip::StaticApplyHooks()
{
	LoadTuning();

	if (!Tuning.bSkipIntro && !Tuning.bSkipTitleScreen && !Tuning.bSkipUpdateCheck)
	{
		return;
	}

	Mod::FrontendStateTable::Install();

	if (Tuning.bSkipIntro)
	{
		UIFrontend::PatchState(UIFrontend::FrontendState::EA_PARAMOUNT_LEGAL, &EnterState_SkipIntroScreens);
	}

	if (Tuning.bSkipTitleScreen)
	{
		UIFrontend::PatchState(UIFrontend::FrontendState::TITLE_SCREEN, &EnterState_SkipTitleScreen);
	}

	if (Tuning.bSkipUpdateCheck)
	{
		UIFrontend::PatchState(UIFrontend::FrontendState::UPDATE_CHECK, &EnterState_SkipUpdateCheck);
	}

	tConsole::fPrintf("Frontend skip: intro=%u title=%u update=%u",
		Tuning.bSkipIntro, Tuning.bSkipTitleScreen, Tuning.bSkipUpdateCheck);
}
