#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"

#include "Debug/Log.h"

using namespace LibMath;

namespace LibGL::Physics
{
	SphereCollider::SphereCollider(Entity& owner, const Vector3& center,
		const float radius) : ICollider(owner, Bounds{ center, Vector3(radius * 2)}),
		m_center(center), m_radius(radius)
	{
	}

	bool SphereCollider::check(const Vector3& point) const
	{
		return point.distanceSquaredFrom(m_center) <= m_radius * m_radius;
	}

	bool SphereCollider::check(const ICollider& other) const
	{
		const auto otherTypeHash = typeid(other).hash_code();

		if (otherTypeHash == typeid(BoxCollider).hash_code())
			return checkBox(dynamic_cast<const BoxCollider&>(other));

		if (otherTypeHash == typeid(SphereCollider).hash_code())
			return checkSphere(dynamic_cast<const SphereCollider&>(other));

		if (otherTypeHash == typeid(CapsuleCollider).hash_code())
			return checkCapsule(dynamic_cast<const CapsuleCollider&>(other));

		DEBUG_LOG("WARNING: collisions between 'SphereCollider' and '%s' are not supported.\n", typeid(other).name());
		return false;
	}

	bool SphereCollider::checkBox(const BoxCollider& other) const
	{
		return other.checkSphere(*this);
	}

	bool SphereCollider::checkSphere(const SphereCollider& other) const
	{
		return ICollider::check(other);
	}

	bool SphereCollider::checkCapsule(const CapsuleCollider& other) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(other))
			return false;

		const auto center = getBounds().m_center;
		const auto radius = getBounds().getSphereRadius();

		const auto capsuleCenter = other.getBounds().m_center;
		const auto capsuleRadius = other.getRadius();
		const auto capsuleOrientedHeight = other.getUpDirection() *
			(other.getHeight() - capsuleRadius);

		const float totalRadius = radius + capsuleRadius;

		// Get the closest point to the sphere's center on the capsule's center segment
		const Vector3 closestPoint = getClosestPointOnSegment(center,
			capsuleOrientedHeight / 2.f - capsuleCenter,
			capsuleOrientedHeight / 2.f + capsuleCenter);

		return center.distanceSquaredFrom(closestPoint) <= totalRadius * totalRadius;
	}
}
