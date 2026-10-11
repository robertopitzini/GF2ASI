#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class CommonNPCSM : public EARS::Modules::NPCStateMachine
	{
	public:

		CommonNPCSM() = delete;
		CommonNPCSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CommonNPCSM();

		virtual uint32_t GetStateMachineID() const override { return 0x390CA3EC; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;

		static EARS::StateMachineSys::StateMachine* S_CommonNPCSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_CommonNPCSM_FactoryFn.
		char m_Padding[0x4];
	};
	static_assert(sizeof(CommonNPCSM) == 0x74);
}
