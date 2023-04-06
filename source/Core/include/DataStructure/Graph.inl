#pragma once
#include "Graph.h"

namespace LibGL::DataStructure
{
	template <class NodeT>
	template <typename DataT>
	void Graph<NodeT>::addNode(DataT& node)
	{
		static_assert(std::is_same_v<NodeT, DataT> || std::is_base_of_v<NodeT, DataT>);
		m_nodes.push_back(std::make_shared<DataT>(node));
	}

	template <class NodeT>
	void Graph<NodeT>::removeNode(const NodeT& node)
	{
		const auto nodeIter = std::find(m_nodes.begin(), m_nodes.end(), std::make_shared<NodeT>(&node));

		if (nodeIter != m_nodes.end())
			m_nodes.erase(nodeIter);
	}

	template <class NodeT>
	std::vector<std::shared_ptr<NodeT>> Graph<NodeT>::getNodes()
	{
		return m_nodes;
	}

	template <class NodeT>
	void Graph<NodeT>::clear()
	{
		m_nodes.clear();
	}
}
