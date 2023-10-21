#pragma once
#include <Entity.h>

#include "Resources/Material.h"
#include "Resources/Model.h"

namespace LibGL::Rendering
{
	class Mesh : public Entity
	{
	public:
		Mesh() = delete;
		explicit Mesh(Entity* parent, const Resources::Model& model, Resources::Material material);

		Mesh(const Mesh& other) = default;
		Mesh(Mesh&& other) noexcept = default;

		~Mesh() override = default;

		Mesh& operator=(const Mesh& other) = default;
		Mesh& operator=(Mesh&& other) noexcept = default;

		/**
		 * \brief Gets the mesh's model
		 * \return The mesh's current model
		 */
		const Resources::Model* getModel() const;

		/**
		 * \brief Sets the mesh's model
		 * \param model The mesh's new model
		 */
		void setModel(const Resources::Model& model);

		/**
		 * \brief Gets the mesh's material
		 * \return A copy of the mesh's material
		 */
		Resources::Material getMaterial() const;

		/**
		 * \brief Gets the mesh's material
		 * \return A reference to the mesh's material
		 */
		Resources::Material& getMaterial();

		/**
		 * \brief Draws the mesh
		 */
		void draw() const;

		/**
		 * \brief Updates the mesh
		 */
		void update() override;

	private:
		const Resources::Model*	m_model = nullptr;
		Resources::Material		m_material;
	};
}
