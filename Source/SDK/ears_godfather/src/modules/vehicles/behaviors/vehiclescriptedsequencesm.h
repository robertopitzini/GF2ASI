#pragma once

#include "modules/vehicles/behaviors/vehicleaism.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	/**
	 * Drives a vehicle through a scripted sequence: get to the sequence, face the orientation it
	 * asks for, wait there, then run the action and repeat or terminate.
	 */
	class VehicleScriptedSequenceSM : public EARS::Modules::VehicleAiStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_VehicleScriptedSequenceSM = 0x0A567A9E;

		VehicleScriptedSequenceSM() = delete;
		VehicleScriptedSequenceSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);

		virtual ~VehicleScriptedSequenceSM();

		//~ Begin VehicleAiStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_VehicleScriptedSequenceSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End VehicleAiStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleScriptedSequenceSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/**
		 * Describe and register the "vehicleScriptedSequenceTable" state table, along with the two
		 * child tables it depends on. Registering it without them would leave pathToPos spawning a
		 * table nothing had built, because the engine skips its own builder once this one has run.
		 */
		static void BuildStateTable();

		/** Bits of the SM's flag word. */
		enum VehicleScriptedSequenceSMFlags : uint32_t
		{
			SSSMFLAGS_STARTSS     = 0x1,
			SSSMFLAGS_STARTACTION = 0x2,
			SSSMFLAGS_TERMINATE   = 0x4,
		};

	private:

		// TransIDs extending VehicleAiStateMachineTransID (base TRANSID_LAST = 11).
		enum VehicleScriptedSequenceSMTransID : uint32_t
		{
			TRANSID_STARTSS              = 0xB,
			TRANSID_PATHTOPOS_WAITATSS   = 0xC,
			TRANSID_FACEORIENT_WAITATSS  = 0xD,
			TRANSID_WAITATSS             = 0xE,
			TRANSID_GOTOSS_PATHTOPOS     = 0xF,
			TRANSID_FACEORIENT           = 0x10,
			TRANSID_ACTIONDONE           = 0x11,
			TRANSID_REPEATACTION         = 0x12,
			TRANSID_STARTACTION          = 0x13,
			TRANSID_TERMINATESS          = 0x14,
			TRANSID_NO_SCRIPTED_SEQUENCE = 0x15,
			TRANSID_LAST                 = 0x16,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10); VehicleAiStateMachine
		// adds none of its own.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleScriptedSequenceSMMessageID : uint32_t
		{
			MESSAGE_SSINIT             = 0xA,
			MESSAGE_PROCESSMOVEPOSTYPE = 0xB,
			MESSAGE_ACTIONINIT         = 0xC,
			MESSAGE_PLAYWAITANIM       = 0xD,
			MESSAGE_PROCESSINPOSITION  = 0xE,
			MESSAGE_FACEORIENT_INIT    = 0xF,
			MESSAGE_FACEORIENT_UPDATE  = 0x10,
			MESSAGE_SSDONE             = 0x11,
			MESSAGE_LAST               = 0x12,
		};

		// TODO: the setter surface (SetScriptedSeq, StartScriptedSeq, StartAction) over ten
		// private helpers is not reconstructed. GetTransIDString is overridden in the original but
		// has no slot in the shipping vtable, so it is debug-only and deliberately absent.
		//
		// The members are an IScriptedSequenceTarget* at +0x60, a SafePtr to the sequence at
		// +0x64, a flag word at +0x6C, a count at +0x70 and a look-at position at +0x74. Not
		// confirmed against the shipping build; the 0x80 total is, from the allocation in the
		// factory.
		char m_Padding[0x20];
	};

	static_assert(sizeof(VehicleScriptedSequenceSM) == 0x80);
}
