#include "FrontendStateTable.h"

// Addons
#include "Addons/tConsole.h"

// SDK
#include "modules/ui/ui_frontend.h"

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

// C++
#include <stdint.h>

namespace
{
	// UIFrontend::ExecuteNextState. Receives the singleton in ECX and takes no
	// stack arguments, which is what __fastcall with one pointer gives us.
	constexpr uintptr_t EXECUTE_NEXT_STATE = 0x0931010;

	uint64_t ExecuteNextState_Old = 0;

	bool bInstalled = false;

	// The reimplementation reads the same statics as the original, so the two can
	// be swapped either way round without the front end losing track of where it
	// is. The trampoline is never called: the whole point is to dispatch through
	// the patched table instead.
	void __fastcall HOOK_UIFrontend_ExecuteNextState(EARS::Apt::UIFrontend* pThis)
	{
		pThis->ExecuteNextState();
	}
}

void Mod::FrontendStateTable::Install()
{
	if (bInstalled)
	{
		return;
	}

	bInstalled = true;

	// Has to be taken before the detour can fire.
	EARS::Apt::UIFrontend::CopyStates();

	PLH::ZydisDisassembler Disassembler(PLH::Mode::x86);
	PLH::x86Detour Detour((char*)EXECUTE_NEXT_STATE, (char*)&HOOK_UIFrontend_ExecuteNextState, &ExecuteNextState_Old, Disassembler);
	Detour.hook();

	tConsole::fWriteLine("Frontend: state dispatch now runs from the patchable table");
}
