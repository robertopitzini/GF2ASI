#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * Common base for the AI behaviours that drive a vehicle.
		 *
		 * It holds the four handles every vehicle behaviour works through — the brain, the proxy,
		 * the knowledge store and the car behaviour itself — and contributes the stuck check that
		 * all of them transition on.
		 *
		 * It adds a transition but no messages, so a derived behaviour numbers its own transitions
		 * from TRANSID_LAST below and its own messages from AnimateSMMessageID::MESSAGE_LAST.
		 */
		class VehicleAiStateMachine : public EARS::Framework::AnimateStateMachine
		{
		public:

			static constexpr uint32_t kStateMachine_VehicleAiStateMachine = 0xE38D81E2;

			VehicleAiStateMachine() = delete;

			VehicleAiStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
				: EARS::Framework::AnimateStateMachine(TableID, SMParams)
			{
			}

			// DEVIATION: the original's destructor is not reconstructed. Each derived behaviour
			// routes construction and destruction through an engine stub that covers this layer.
			virtual ~VehicleAiStateMachine() = default;

		protected:

			// Named by every derived behaviour's BuildStateTable, so these are protected rather
			// than private.

			// TransIDs extending AnimateSMTransID (base TRANSID_LAST = 10).
			enum VehicleAiStateMachineTransID : uint32_t
			{
				TRANSID_ISSTUCK = 0xA,  // the vehicle has not made progress for long enough
				TRANSID_LAST    = 0xB,
			};

			// This layer adds no messages of its own; the terminator is here because derived
			// behaviours continue from it.
			enum VehicleAiStateMachineMessageID : uint32_t
			{
				MESSAGE_LAST = 0xA,
			};

		private:

			// TODO: the members are four protected pointers at +0x50 — VehicleBrain,
			// VehicleProxy, VehicleKnowledge and AICarBehavior — reached through GetBrain,
			// GetVehicleProxy and GetVehicle. Not confirmed against the shipping build.
			//
			// The 0x60 total is confirmed: CivilianVehicleStateMachine and
			// VehicleScriptedSequenceSM both start their own members at +0x60, and the shipping
			// sizes in the derived factories match the debug build.
			char m_Padding[0x10];
		};

		static_assert(sizeof(VehicleAiStateMachine) == 0x60);
	}
}
