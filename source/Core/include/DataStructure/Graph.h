#pragma once
#include <memory>
#include <vector>

#include "Node.h"

namespace LibGL::DataStructure
{
	template <class NodeT>
	class Graph
	{
		static_assert(std::is_base_of_v<Node, NodeT>);

	public:
		Graph() = default;
		Graph(const Graph& other) = default;
		Graph(Graph&& other) noexcept = default;
		virtual ~Graph() = default;

		Graph& operator=(const Graph& other) = default;
		Graph& operator=(Graph&& other) noexcept = default;

		/**
		 * \brief Adds the given node to the graph
		 * \param node The node to add to the graph
		 */
		template <typename DataT>
		void addNode(DataT& node);

		/**
		 * \brief Removes the given node from the graph
		 * \param node The node to remove from the graph
		 */
		void removeNode(const NodeT& node);

		/**
		 * \brief Gets the graph's root nodes list
		 * \return A reference to the graph's root nodes list
		 */
		std::vector<std::shared_ptr<NodeT>> getNodes();

		/**
		 * \brief Removes all nodes from the graph
		 */
		void clear();

	private:
		std::vector<std::shared_ptr<NodeT>>	m_nodes;
	};
}

#include "Graph.inl"
