#pragma once
#include <string>
#include <vector>

#include "IResource.h"
#include "Vertex.h"

namespace My::Resources
{
	class Model final : public IResource
	{
	public:
		Model() = default;
		Model(const Model& other);
		Model(Model&& other) noexcept;
		~Model() override = default;

		Model&	operator=(const Model& other);
		Model&	operator=(Model&& other) noexcept;

		bool	loadFromFile(const std::string& fileName) override;

	public:
		std::vector<Vertex>		m_vertices;
		std::vector<size_t>		m_indices;
	};
}
