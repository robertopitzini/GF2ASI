#pragma once

#include "framework/toolkits/statemachine/animatesm.h"
#include "modules/vehicles/behaviors/vehiclebrain.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * The state machine every AI vehicle runs at its root.
	 *
	 * It owns no behaviour of its own: pickBrain reads the brain's current BrainType and spawns
	 * the matching child table, and any brain change sends it straight back to pickBrain so the
	 * child is torn down and replaced. Every AI brain in the game hangs off this one table.
	 */
	class RootVehicleSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_RootVehicleSM = 0x70182F64;

		RootVehicleSM() = delete;
		RootVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~RootVehicleSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_RootVehicleSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_RootVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/**
		 * Describe and register the "RootVehicleST" state table.
		 *
		 * Unlike most parents this one does not build its children: BuildStateMachines registers
		 * the five brain tables itself, each through its own class, so all six have to be called.
		 * The order does not matter, since a child is resolved by name hash when it spawns.
		 */
		static void BuildStateTable();

	private:

		// TransIDs extending AnimateSMTransID (base TRANSID_LAST = 10).
		enum RootVehicleSMTransID : uint32_t
		{
			TRANSID_BRAIN_CHANGE  = 0xA,
			TRANSID_IDLE          = 0xB,
			TRANSID_CIVILIAN      = 0xC,
			TRANSID_CHASE         = 0xD,
			TRANSID_POLICE_PATROL = 0xE,
			TRANSID_MOB_VENUE     = 0xF,
			TRANSID_LAST          = 0x10,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum RootVehicleSMMessageID : uint32_t
		{
			MESSAGE_NEW_STATE_ENTER = 0xA,
			MESSAGE_LAST            = 0xB,
		};

		EARS::Modules::VehicleBrain* m_Brain;
		BrainType                    m_CurrBrainType;
	};

	static_assert(sizeof(RootVehicleSM) == 0x58);
}
