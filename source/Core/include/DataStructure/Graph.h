#pragma once
#include <vector>

#include "Node.h"

namespace My::DataStructure
{
	template <class NodeT>
	class Graph
	{
		static_assert(std::is_base_of_v<Node, NodeT>);

	public:
		Graph() = default;
		Graph(const Graph& other) = default;
		Graph(Graph&& other) noexcept = default;
		virtual ~Graph();

		Graph& operator=(const Graph& other) = default;
		Graph& operator=(Graph&& other) noexcept = default;

		/**
		 * \brief Adds the given node to the graph
		 * \param node The node to add to the graph
		 */
		template <typename DataType>
		void addNode(DataType& node);

		/**
		 * \brief Removes the given node from the graph
		 * \param node The node to remove from the graph
		 */
		void removeNode(const NodeT& node);

		/**
		 * \brief Gets the graph's root nodes list
		 * \return A reference to the graph's root nodes list
		 */
		std::vector<NodeT*> getNodes();

	private:
		std::vector<NodeT*>	m_nodes;
	};
}

#include "Graph.inl"
