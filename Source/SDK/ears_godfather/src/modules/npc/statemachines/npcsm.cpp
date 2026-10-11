#include "E:/My Projects/GF2ASI/build/CMakeFiles/GF2SDK.dir/Debug/cmake_pch.hxx"
#include "npcsm.h"

// SDK
#include "framework/core/base/base.h"
#include "modules/npc/npc.h"

namespace EARS::Modules
{
	NPCStateMachine::NPCStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: SentientSM(TableID, SMParams)
		, m_Flags(0)
		, m_Interrupt(INTERRUPT_NONE)
		, m_StateTimeLimit(0.0f)
	{
		// TODO: Expose class ID?
		m_NPC = EARS::Framework::_GetInterface<EARS::Modules::NPC>(m_Sentient, 0x369AC561);
	}

	bool NPCStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool>(0x76BC00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool>(0x76BCD0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCStateMachine::InitialiseChild(StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCStateMachine*, EARS::StateMachineSys::StateMachine*>(0x76C2F0, this, &ChildMachine);
	}
}
