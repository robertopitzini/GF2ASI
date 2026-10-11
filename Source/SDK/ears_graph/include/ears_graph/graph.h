#pragma once

// SDK
#include "ears_common/rwtypes.h"
#include "ears_common/safeptr.h"

// C++
#include <stdint.h>

/**
 * Which navigation graph a node belongs to. Used as a bitmask: a spawner is
 * built against one of these and only ever walks nodes from it.
 *
 * Not inside a namespace in the original.
 */
enum GraphType
{
	GraphType_REF					= -1,
	GRAPHFLAGS_GROUNDPATHING		= 0x1,
	GRAPHFLAGS_PATROL				= 0x2,
	GRAPHFLAGS_VEHICULAR			= 0x4,
	GRAPHFLAGS_PED					= 0x8,
	GRAPHFLAGS_VEHICULAR_PARKING	= 0x10,
	GRAPHFLAGS_AUDIO_OCCLUSION		= 0x20,
};

namespace EARS
{
	namespace Framework
	{
		namespace Navigation
		{
			/**
			 * One navigation graph: a set of Nodes joined by Connections.
			 *
			 * TODO: only the nested types the traffic managers need are
			 * recovered. Graph's own members are not, so this must not be
			 * instantiated or used by value -- it is a name for pointers and
			 * for SafePtr to hang off.
			 */
			class Graph
			{
			public:

				class Node;

				// A link from one node to another within the same graph.
				class Connection;

				// A link that crosses into an adjoining graph.
				class NeighborConnection;

				/**
				 * A node borrowed from a graph, held safely: the graph half is
				 * a SafePtr, so the handle reports empty once the graph it came
				 * from has gone, rather than dereferencing freed nodes.
				 */
				class NodeHandle
				{
				public:

					NodeHandle() = default;

					const Node* Dereference() const { return m_Graph.IsValid() ? m_Node : nullptr; }

					void Clear()
					{
						m_Graph.Clear();
						m_Node = nullptr;
					}

					explicit operator bool() const { return Dereference() != nullptr; }

					// TODO: Set, GetPosition and the comparison operators are
					// declared on the original and not recovered here.

				private:

					SafePtr<Graph> m_Graph;
					const Node* m_Node = nullptr;
				};

				static_assert(sizeof(NodeHandle) == 0xC, "EARS::Framework::Navigation::Graph::NodeHandle must equal 0xC");
			};
		} // Navigation
	} // Framework
} // EARS
