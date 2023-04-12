#pragma once
#include "Transform.h"
#include "DataStructure/Graph.h"

namespace LibGL::Resources
{
	class SceneNode : public DataStructure::Node, public LibMath::Transform
	{
	public:
		SceneNode() = default;
		SceneNode(SceneNode* parent, const Transform& transform);

		SceneNode(const SceneNode&) = default;
		SceneNode(SceneNode&&) = default;
		~SceneNode() override = default;

		SceneNode& operator=(const SceneNode&) = default;
		SceneNode& operator=(SceneNode&&) = default;

		/**
		 * \brief Gets the scene node's global transformation matrix
		 * \return The scene node's global transformation matrix
		 */
		Transform getGlobalTransform() const;

		/**
		 * \brief Adds the given node as a child of the current node
		 * \param child A pointer to the child to add to the current node
		 */
		void addChild(Node& child) override;

		/**
		 * \brief Removes the given node from this node's children
		 * \param child A pointer to the child to remove from the node's children
		 */
		void removeChild(Node& child) override;

		/**
		 * \brief Updates the scene node (does nothing by default)
		 */
		virtual void update();

	protected:
		/**
		 * \brief Updates the scene node's transformation matrices
		 */
		void onChange() override;

	private:
		Transform	m_globalTransform;

		void updateGlobalTransform();
	};

	class Scene : public DataStructure::Graph<SceneNode>
	{
	public:
		void update();
	};
}
