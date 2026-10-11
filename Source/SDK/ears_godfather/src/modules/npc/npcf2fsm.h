#pragma once

#include "modules/npc/statemachines/npcsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCF2FStateMachine : public EARS::Modules::NPCStateMachine
	{
	public:

		NPCF2FStateMachine() = delete;
		NPCF2FStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCF2FStateMachine();

		//~ Begin NPCStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x187B7E2B; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End NPCStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCF2FStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	protected:

		//~ Begin NPCStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual EARS::StateMachineSys::StateMachineSnapshot* WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End NPCStateMachine Interface

	private:
		// TODO: sizeof is the shipping build's, from the allocation in S_NPCF2FStateMachine_FactoryFn.
		// The padding also covers the secondary bases, CommonF2FSMAudio and SafeObj, which are
		// not modelled.
		char m_Padding[0x4C];
	};
	static_assert(sizeof(NPCF2FStateMachine) == 0xBC);
}
