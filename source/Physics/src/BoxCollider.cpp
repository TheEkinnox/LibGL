#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "Arithmetic.h"
#include "Debug/Log.h"

using namespace LibMath;

namespace LibGL::Physics
{
	BoxCollider::BoxCollider(Entity& owner, const Vector3& center,
		const Vector3& size) : ICollider(owner, { center, size }), m_center(center), m_size(size)
	{
	}

	bool BoxCollider::check(const Vector3& point) const
	{
		const Vector3 min = m_center - m_size / 2;
		const Vector3 max = m_center + m_size / 2;

		return min.m_x <= point.m_x && max.m_x >= point.m_x &&
			min.m_y <= point.m_y && max.m_y >= point.m_y &&
			min.m_z <= point.m_z && max.m_z >= point.m_z;
	}

	bool BoxCollider::check(const ICollider& other) const
	{
		const auto otherTypeHash = typeid(other).hash_code();

		if (otherTypeHash == typeid(BoxCollider).hash_code())
			return checkBox(dynamic_cast<const BoxCollider&>(other));

		if (otherTypeHash == typeid(SphereCollider).hash_code())
			return checkSphere(dynamic_cast<const SphereCollider&>(other));

		if (otherTypeHash == typeid(CapsuleCollider).hash_code())
			return checkCapsule(dynamic_cast<const CapsuleCollider&>(other));

		DEBUG_LOG("WARNING: collisions between 'BoxCollider' and '%s' are not supported.\n", typeid(other).name());
		return false;
	}

	bool BoxCollider::checkBox(const BoxCollider& other) const
	{
		const auto [center, size] = getBounds();
		const auto [otherCenter, otherSize] = other.getBounds();

		const Vector3 min = center - size / 2;
		const Vector3 max = center + size / 2;
		const Vector3 otherMin = otherCenter - otherSize / 2;
		const Vector3 otherMax = otherCenter + otherSize / 2;

		return min.m_x <= otherMax.m_x && max.m_x >= otherMin.m_x &&
			min.m_y <= otherMax.m_y && max.m_y >= otherMin.m_y &&
			min.m_z <= otherMax.m_z && max.m_z >= otherMin.m_z;
	}

	bool BoxCollider::checkSphere(const SphereCollider& other) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(other))
			return false;

		const auto [center, size] = getBounds();
		const auto min = center - size;
		const auto max = center + size;

		const auto sphereCenter = other.getBounds().m_center;

		// Get the box's closest point to sphere's center
		const Vector3 closestPoint
		{
			clamp(sphereCenter.m_x, min.m_x, max.m_x),
			clamp(sphereCenter.m_y, min.m_y, max.m_y),
			clamp(sphereCenter.m_z, min.m_z, max.m_z)
		};

		// If the closest point and sphere collide, the box and sphere collide too
		return other.check(closestPoint);
	}

	bool BoxCollider::checkCapsule(const CapsuleCollider& other) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(other))
			return false;

		// TODO: Box-Capsule collisions
		return true;
	}
}
