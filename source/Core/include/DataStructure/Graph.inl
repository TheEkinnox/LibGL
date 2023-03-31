#pragma once
#include "Graph.h"

namespace My::DataStructure
{
	template <class NodeT>
	Graph<NodeT>::~Graph()
	{
		for (const auto node : m_nodes)
			delete node;

		m_nodes.clear();
	}

	template <class NodeT>
	template <typename DataType>
	void Graph<NodeT>::addNode(DataType& node)
	{
		static_assert(std::is_same_v<NodeT, DataType> || std::is_base_of_v<NodeT, DataType>);
		m_nodes.push_back(new DataType(node));
	}

	template <class NodeT>
	void Graph<NodeT>::removeNode(const NodeT& node)
	{
		const auto nodeIter = std::find(m_nodes.begin(), m_nodes.end(), &node);

		if (nodeIter != m_nodes.end())
		{
			m_nodes.erase(nodeIter);
			delete* nodeIter;
		}
	}

	template <class NodeT>
	std::vector<NodeT*> Graph<NodeT>::getNodes()
	{
		return m_nodes;
	}
}
