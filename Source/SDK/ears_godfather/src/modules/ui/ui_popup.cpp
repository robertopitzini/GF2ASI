#include "ui_popup.h"

// Addons
#include "Platform/MemUtils.h"

// C++
#include <assert.h>
#include <string.h>

namespace EARS::Apt
{
	static const uintptr_t ADDR_OPERATOR_NEW = 0x009C8E50;
	static const uintptr_t ADDR_OPERATOR_DELETE = 0x009C8EB0;
	static const uintptr_t ADDR_SHOW = 0x00982E10;

	UIPopupInfo::UIPopupInfo(PopupCallbackFunc InFunction)
		: m_CallbackFunction(InFunction)
		, m_BackButtonHashId(0xB45419AB)
		, m_AcceptButtonHashId(0xDD340FEC)
	{
		memset(m_Unk24, 0, sizeof(m_Unk24));
		memset(m_UserValueArray, 0, sizeof(m_UserValueArray));
		memset(m_UnkC8, 0, sizeof(m_UnkC8));
	}

	void* UIPopupInfo::operator new(size_t InSize) noexcept
	{
		return MemUtils::CallCdeclMethod<void*, uint32_t>(ADDR_OPERATOR_NEW, (uint32_t)InSize);
	}

	void UIPopupInfo::operator delete(void* InBlock) noexcept
	{
		MemUtils::CallCdeclMethod<void, void*>(ADDR_OPERATOR_DELETE, InBlock);
	}

	void UIPopupInfo::SetTitle(const char* InTitle)
	{
		m_TitleStr.assign(InTitle);
	}

	void UIPopupInfo::SetDescription(const char* InDescription)
	{
		m_DescriptionStr.assign(InDescription);
	}

	void UIPopupInfo::AddOption(const char* InText, bool bInSelected, int32_t InUserValue)
	{
		if (m_NumOptions >= MAX_OPTIONS)
		{
			return;
		}

		m_OptionStrArray[m_NumOptions].assign(InText);
		m_UserValueArray[m_NumOptions] = InUserValue;

		if (bInSelected)
		{
			m_SelectedIdx = (int32_t)m_NumOptions;
		}

		m_NumOptions++;
	}

	int32_t UIPopupInfo::GetUserValue(const uint32_t InOptionIndex) const
	{
		assert(InOptionIndex < m_NumOptions);
		return m_UserValueArray[InOptionIndex];
	}

	void UIPopupInfo::Show()
	{
		MemUtils::CallClassMethod<void, UIPopupInfo*>(ADDR_SHOW, this);
	}
}
