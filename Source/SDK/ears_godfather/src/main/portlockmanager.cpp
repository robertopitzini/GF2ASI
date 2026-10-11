#include "portlockmanager.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::MainLoop
{
	void PortLockManager::SetIsDialogAllowed(const bool bInDialogAllowed)
	{
		MemUtils::CallClassMethod<void, PortLockManager*, bool>(0x0068C590, this, bInDialogAllowed);
	}

	PortLockManager* PortLockManager::GetInstance()
	{
		return *(PortLockManager**)0x01129814;
	}
}
