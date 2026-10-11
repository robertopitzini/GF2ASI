#pragma once

// SDK
#include "framework/core/base/base.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * Common base for the streamed entities that exist only to carry a
		 * filter for something else to match against. It adds no state of its
		 * own -- the derived class holds the filter -- so its only job is to
		 * give them a shared type.
		 *
		 * No destructor is declared: it would occupy Base's existing slot 0
		 * rather than adding one, and the manifest gives this class no .cpp to
		 * define it in.
		 */
		class FilterEntityBase : public EARS::Framework::Base
		{
		};

		static_assert(sizeof(FilterEntityBase) == 0x50, "EARS::Modules::FilterEntityBase must equal 0x50");
	} // Modules
} // EARS
