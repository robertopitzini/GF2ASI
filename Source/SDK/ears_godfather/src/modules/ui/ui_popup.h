#pragma once

#include "ears_common/bitflags.h"
#include "ears_common/string.h"

// CPP
#include <stddef.h>

namespace EARS::Apt
{
	struct UIPopupInfo
	{
	public:

		enum class PopupStyle
		{
			STANDARD,
			STANDARD_NO_BACK_BUTTON,
			YES_NO,
			FULL_SCREEN,
			STANDARD2,
			CONTINUE2,
			FULL_SCREEN_NO_BACK_BUTTON,
			CONTINUE_LARGE,
			STANDARD_LARGE,
			YES_NO_LARGE,
			STANDARD_LARGE_NO_BACK_BUTTON
		};

		enum class PopupFlags : int32_t
		{
			USE_PAUSE_EVENT = 0x1,
			IGNORE_INPUT = 0x2,
			IGNORE_BACK_BUTTON = 0x4,
		};

		// A popup reports the option the user took by its user value, not its
		// index, and -1 for the back button. bAborted is raised when the popup never
		// made it on screen, in which case nothing was chosen.
		using PopupCallbackFunc = void(__cdecl*)(int, EARS::Apt::UIPopupInfo*, bool);

		explicit UIPopupInfo(PopupCallbackFunc InFunction = nullptr);

		// Defaulted on purpose, and it must stay that way. The image's destructor
		// (0x9823B0) does nothing but free the ten String members' buffers, which
		// is exactly what their own destructors do as this object is destroyed -
		// same buffers, same free function, same order. Forwarding to it as well
		// frees every buffer twice: it leaves the String pointers dangling rather
		// than clearing them, so the member destructors cannot tell the memory has
		// already gone, and the heap is corrupted by the time the popup closes.
		~UIPopupInfo() = default;

		// A popup is handed to the popup manager by Show() and lives until its
		// callback releases it, so it has to come from the game image's heap rather
		// than the mod's. Whole-program optimisation folded the image's operator new
		// and delete together with StringAllocator's, which is the name the one
		// surviving copy of each carries.
		//
		// noexcept because the image's allocator returns null rather than throwing,
		// and a non-throwing allocation function is what makes a new-expression
		// yield null instead of running the constructor on it.
		static void* operator new(size_t InSize) noexcept;
		static void operator delete(void* InBlock) noexcept;

		/** assign popup style. Call before Show(). */
		void SetStyle(PopupStyle InStyle) { m_Style = InStyle; }

		/** assign specific back button flag. Call before Show(). */
		void SetIgnoreBackButton(bool bInIgnoreBackButton) { m_Flags.Set((uint32_t)PopupFlags::IGNORE_BACK_BUTTON, bInIgnoreBackButton); }

		/** assign a string to the popup's title. Call before Show(). */
		void SetTitle(const char* InTitle);

		/** assign a string to the popup's description. Call before Show(). */
		void SetDescription(const char* InDescription);

		/** assign a callback function to trigger when the Popup's buttons has been pressed. */
		void SetCallbackFunction(PopupCallbackFunc InFunction) { m_CallbackFunction = InFunction; }

		/** Include an option into the popup. Call before Show(). Silently ignored
		 *  once MAX_OPTIONS are present, which is what the original does. */
		void AddOption(const char* InText, bool bInSelected, int32_t InUserValue);

		/** The user value AddOption was given for an option. The index is the one
		 *  the callback is passed, so the back button's -1 must be filtered out
		 *  before calling this. */
		int32_t GetUserValue(const uint32_t InOptionIndex) const;

		uint32_t GetNumOptions() const { return m_NumOptions; }

		/** Call when Popup is ready to be shown. When the popup manager is not up
		 *  this does not queue anything: it invokes the callback inline with option
		 *  -1 and bAborted raised, so the callback can run before Show() returns. */
		void Show();

	private:

		static const uint32_t MAX_OPTIONS = 8;

		Flags32 m_Flags;
		uint32_t m_NumOptions = 0;
		int32_t m_SelectedIdx = 0;
		PopupStyle m_Style = PopupStyle::STANDARD;
		void* m_UserData = nullptr;
		PopupCallbackFunc m_CallbackFunction = nullptr;
		uint32_t m_Unk18 = 0;
		uint32_t m_BackButtonHashId = 0;
		uint32_t m_AcceptButtonHashId = 0;
		uint8_t m_Unk24[8];
		String m_DescriptionStr;
		String m_TitleStr;
		String m_OptionStrArray[MAX_OPTIONS];
		int32_t m_UserValueArray[MAX_OPTIONS];
		uint8_t m_UnkC8[4];
	};

	static_assert(sizeof(UIPopupInfo) == 0xF0);
}
