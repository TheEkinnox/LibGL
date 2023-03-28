#include "Scene.h"

namespace My::Resources
{
	SceneNode::SceneNode(SceneNode* parent, const Transform& transform)
		: Node(parent), Transform(transform)
	{
	}

	Matrix4 SceneNode::getGlobalTransform() const
	{
		return m_globalTransform;
	}

	void SceneNode::addChild(Node* child)
	{
		Node::addChild(child);
		reinterpret_cast<SceneNode*>(child)->updateGlobalTransform();
	}

	void SceneNode::removeChild(Node* child)
	{
		Node::removeChild(child);
		reinterpret_cast<SceneNode*>(child)->updateGlobalTransform();
	}

	void SceneNode::onChange()
	{
		Transform::onChange();

		updateGlobalTransform();

		for (auto* child : getChildren())
			if (child != nullptr)
				reinterpret_cast<SceneNode*>(child)->updateGlobalTransform();
	}

	void SceneNode::updateGlobalTransform()
	{
		m_globalTransform = getMatrix();

		if (getParent() != nullptr)
			m_globalTransform *= reinterpret_cast<SceneNode*>(getParent())->getGlobalTransform();
	}
}
