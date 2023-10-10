#pragma once
#include "Resources/Material.h"
#include "Scene.h"

namespace LibGL::Resources
{
	class Model;
	class Texture;
}

namespace LibGL::Rendering
{
	class Mesh : public Entity
	{
	public:
		Mesh() = delete;
		explicit Mesh(Entity* parent, const Resources::Model& model, const Material& material);

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
		Material getMaterial() const;

		/**
		 * \brief Gets the mesh's material
		 * \return A reference to the mesh's material
		 */
		Material& getMaterial();

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
		Material				m_material;
	};
}
