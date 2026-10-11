#pragma once

#include "modules/vehicles/statemachines/vehicleapproachpositionsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * A mob car bringing its occupants to a venue and dropping them off there.
	 *
	 * The plainest of the approach behaviours: no e-brake states and no rule breaking of its
	 * own. followGraph is the urgent approach to a known venue; normalVehicleFollowGraph is what
	 * it falls back to while no venue has been assigned yet.
	 */
	class MobCarApproachVenueSM : public EARS::Modules::VehicleApproachPositionSM
	{
	public:

		static constexpr uint32_t kStateMachine_MobCarApproachVenueSM = 0xBDDC09CD;

		MobCarApproachVenueSM() = delete;
		MobCarApproachVenueSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~MobCarApproachVenueSM();

		//~ Begin VehicleApproachPositionSM Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_MobCarApproachVenueSM; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child) override;
		virtual void RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine) override;
		//~ End VehicleApproachPositionSM Interface

		static EARS::StateMachineSys::StateMachine* S_MobCarApproachVenueSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "MobCarRootST" state table. */
		static void BuildStateTable();

		/** Bit of the base flag word that this layer adds. */
		enum MobCarApproachVenueSMFlags : uint32_t
		{
			APPROACH_MOBCAR_IS_DEFENDING_FROM_PLAYER = 0x40,
			APPROACH_LAST                            = 0x40,
		};

	protected:

		//~ Begin VehicleApproachPositionSM Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual const EARS::StateMachineSys::StateMachineSnapshot* ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End VehicleApproachPositionSM Interface

	private:

		// This layer adds no transitions of its own; TRANSID_LAST is the base value.

		// MessageIDs extending VehicleApproachPositionSMMessageID (base MESSAGE_LAST = 18).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum MobCarApproachVenueSMMessageID : uint32_t
		{
			MESSAGE_MOB_INIT = 0x12,
			MESSAGE_LAST     = 0x13,
		};

		uint32_t m_VenueID;
	};
	static_assert(sizeof(MobCarApproachVenueSM) == 0x88);
}
