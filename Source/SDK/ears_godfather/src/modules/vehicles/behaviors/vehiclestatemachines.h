#pragma once

#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * What ordinary traffic runs: follow the road graph until told to stop.
	 *
	 * The graph following is all in the update message -- the table itself is two states, and the
	 * only way out is the brain deciding this vehicle is done.
	 */
	class CivilianVehicleStateMachine : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_CivilianVehicleStateMachine = 0x64EA38FF;

		CivilianVehicleStateMachine() = delete;
		CivilianVehicleStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CivilianVehicleStateMachine();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_CivilianVehicleStateMachine; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CivilianVehicleStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "civilianVehicleMainTable" state table. */
		static void BuildStateTable();

	protected:

		//~ Begin VehicleAiStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		//~ End VehicleAiStateMachine Interface

	private:

		// TransIDs extending VehicleAiStateMachineTransID (base TRANSID_LAST = 11).
		enum CivilianVehicleStateMachineTransID : uint32_t
		{
			TRANSID_FINDGRAPH_FOLLOWGRAPH = 0xB,
			TRANSID_DONE                  = 0xC,
			TRANSID_LAST                  = 0xD,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum CivilianVehicleStateMachineMessageID : uint32_t
		{
			MESSAGE_FOLLOWGRAPH_UPDATE = 0xA,
			MESSAGE_DONE_INIT          = 0xB,
			MESSAGE_LAST               = 0xC,
		};

		// TODO: the padding is the AICarBehavior handle at +0x60. Left as padding because it is a
		// SafePtr, whose destructor would run alongside the engine destructor's.
		char m_Padding[0x8];
	};
	static_assert(sizeof(CivilianVehicleStateMachine) == 0x68);
}
