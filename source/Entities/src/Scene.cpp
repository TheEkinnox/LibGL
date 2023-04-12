#include "Scene.h"

using namespace LibMath;

namespace LibGL::Resources
{
	SceneNode::SceneNode(SceneNode* parent, const Transform& transform)
		: Node(parent), Transform(transform)
	{
	}

	Transform SceneNode::getGlobalTransform() const
	{
		return m_globalTransform;
	}

	void SceneNode::addChild(Node& child)
	{
		Node::addChild(child);
		reinterpret_cast<SceneNode&>(child).updateGlobalTransform();
	}

	void SceneNode::removeChild(Node& child)
	{
		Node::removeChild(child);
		reinterpret_cast<SceneNode&>(child).updateGlobalTransform();
	}

	void SceneNode::update()
	{
		for (Node* child : getChildren())
			reinterpret_cast<SceneNode*>(child)->update();
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
		m_globalTransform = static_cast<Transform>(*this);

		const SceneNode* castParent = reinterpret_cast<SceneNode*>(getParent());

		if (castParent != nullptr)
		{
			const Transform parentTransform = castParent->getGlobalTransform();
			m_globalTransform.translate(parentTransform.getPosition());
			m_globalTransform.rotate(parentTransform.getRotation());
			m_globalTransform.scale(parentTransform.getScale());
		}
	}

	void Scene::update()
	{
		for (const auto& node : getNodes())
			node->update();
	}
}
