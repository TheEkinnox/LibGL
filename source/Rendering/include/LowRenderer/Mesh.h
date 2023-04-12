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
	class Mesh : public Resources::SceneNode
	{
	public:
						Mesh() = delete;
		explicit	Mesh(SceneNode* parent, const Resources::Model& model, const Material& material);
						Mesh(const Mesh& other) = default;
						Mesh(Mesh&& other) noexcept = default;
						~Mesh() override = default;

		Mesh&		operator=(const Mesh& other) = default;
		Mesh&		operator=(Mesh&& other) noexcept = default;

		/**
		 * \brief Sets the mesh's model
		 * \param model The mesh's new model
		 */
		void setModel(const Resources::Model& model);

		/**
		 * \brief Sets the mesh's material
		 * \param material The mesh's new material
		 */
		void setMaterial(const Material& material);

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
