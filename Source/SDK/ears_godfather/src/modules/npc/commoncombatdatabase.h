#pragma once

namespace EARS::Modules
{
	class CommonCombatDatabase
	{
		char m_Padding[0x3A6];
	};

	static_assert(sizeof(CommonCombatDatabase) == 0x3A6);
}
