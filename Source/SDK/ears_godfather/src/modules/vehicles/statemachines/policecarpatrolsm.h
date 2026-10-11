#pragma once

#include "modules/vehicles/statemachines/vehicleapproachpositionsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * A police car driving around until it has somewhere to be, then getting there.
	 *
	 * The two e-brake states are what makes a responding car arrive the way it does on screen:
	 * once the occupants can see the criminal, or the car is inside the crime scene, it stops
	 * obeying traffic rules and has two seconds to come to a halt before the cops get out
	 * regardless.
	 */
	class PoliceCarPatrolSM : public EARS::Modules::VehicleApproachPositionSM
	{
	public:

		static constexpr uint32_t kStateMachine_PoliceCarPatrolSM = 0x6DC822DC;

		PoliceCarPatrolSM() = delete;
		PoliceCarPatrolSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PoliceCarPatrolSM();

		//~ Begin VehicleApproachPositionSM Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_PoliceCarPatrolSM; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child) override;
		virtual void RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine) override;
		//~ End VehicleApproachPositionSM Interface

		static EARS::StateMachineSys::StateMachine* S_PoliceCarPatrolSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "PoliceCarRootST" state table. */
		static void BuildStateTable();

	protected:

		//~ Begin VehicleApproachPositionSM Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual const EARS::StateMachineSys::StateMachineSnapshot* ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End VehicleApproachPositionSM Interface

	private:

		// TransIDs extending VehicleApproachPositionSMTransID (base TRANSID_LAST = 21).
		enum PoliceCarPatrolSMTransID : uint32_t
		{
			TRANSID_WITHIN_CRIME_SCENE     = 0x15,
			TRANSID_OCCUPANTS_SEE_CRIMINAL = 0x16,
			TRANSID_VEHICLE_HIGH_VELOCITY  = 0x17,
			TRANSID_LAST                   = 0x18,
		};

		// MessageIDs extending VehicleApproachPositionSMMessageID (base MESSAGE_LAST = 18).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum PoliceCarPatrolSMMessageID : uint32_t
		{
			MESSAGE_ACT_AS_RULEBREAKER = 0x12,
			MESSAGE_LAST               = 0x13,
		};

		// The class adds no members of its own; it only raises the base's rule-breaker distance.
	};
	static_assert(sizeof(PoliceCarPatrolSM) == 0x84);
}
