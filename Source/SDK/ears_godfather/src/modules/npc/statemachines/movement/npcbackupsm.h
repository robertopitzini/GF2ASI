#pragma once

#include "modules/npc/statemachines/pathing/pathingbasesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCBackupSM : public EARS::Modules::PathingBaseSM
	{
	public:

		NPCBackupSM() = delete;
		NPCBackupSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCBackupSM();

		//~ Begin PathingBaseSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x53C8CA9D; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End PathingBaseSM Interface

		static EARS::StateMachineSys::StateMachine* S_NPCBackupSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCBackupSM_FactoryFn.
		char m_Padding[0x20];
	};
	static_assert(sizeof(NPCBackupSM) == 0x114);
}
