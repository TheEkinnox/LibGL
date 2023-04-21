#include "Arithmetic.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "Vector/Vector3.h"
#include "Debug/Log.h"

using namespace LibMath;

namespace LibGL::Physics
{
	BoxCollider::BoxCollider(Entity& owner, const Vector3& center,
		const Vector3& size) : ICollider(owner, calculateBounds(center, size)), m_center(center), m_size(size)
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

	bool BoxCollider::check(const Ray& ray) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(ray))
			return false;

		// TODO: Box-Ray collisions
		return true;
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
		const auto [center, size, radius] = getBounds();
		const auto [otherCenter, otherSize, otherRadius] = other.getBounds();

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

		const auto sphereCenter = other.getBounds().m_center;

		// If the closest point to the sphere's center is in the sphere, the box and sphere collide
		return other.check(getClosestPoint(sphereCenter));
	}

	bool BoxCollider::checkCapsule(const CapsuleCollider& other) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(other))
			return false;

		const auto [center, size, radius] = getBounds();
		const auto [otherCenter, otherSize, otherRadius] = other.getBounds();

		const Vector3 min = center - size / 2;
		const Vector3 max = center + size / 2;
		const Vector3 otherMin = otherCenter - otherSize / 2;
		const Vector3 otherMax = otherCenter + otherSize / 2;

		// Check AABB to avoid unnecessary capsule collision computation
		if (min.m_x > otherMax.m_x || max.m_x < otherMin.m_x ||
			min.m_y > otherMax.m_y || max.m_y < otherMin.m_y ||
			min.m_z > otherMax.m_z || max.m_z < otherMin.m_z)
			return false;

		// TODO: Box-Capsule collisions
		return true;
	}

	Vector3 BoxCollider::getClosestPoint(const Vector3& point) const
	{
		const auto [center, size, _] = getBounds();
		const auto min = center - size;
		const auto max = center + size;

		return clamp(point, min, max);
	}

	Vector3 BoxCollider::getClosestPointOnSurface(const Vector3& point) const
	{
		const auto [center, size, _] = getBounds();
		const auto minCorner = center - size;
		const auto maxCorner = center + size;

		if (!(minCorner.m_x <= point.m_x && maxCorner.m_x >= point.m_x &&
			minCorner.m_y <= point.m_y && maxCorner.m_y >= point.m_y &&
			minCorner.m_z <= point.m_z && maxCorner.m_z >= point.m_z))
			return clamp(point, minCorner, maxCorner);

		const Vector3 snappedX
		{
			snap(point.m_x, minCorner.m_x, maxCorner.m_x),
			point.m_y,
			point.m_z
		};

		const Vector3 snappedY
		{
			point.m_x,
			snap(point.m_y, minCorner.m_y, maxCorner.m_y),
			point.m_z
		};

		const Vector3 snappedZ
		{
			point.m_x,
			point.m_y,
			snap(point.m_z, minCorner.m_z, maxCorner.m_z)
		};

		const float minDist = min(point.distanceSquaredFrom(snappedX),
			min(point.distanceSquaredFrom(snappedY),
				point.distanceSquaredFrom(snappedZ)));

		return floatEquals(minDist, point.distanceSquaredFrom(snappedX)) ? snappedX :
			floatEquals(minDist, point.distanceSquaredFrom(snappedY)) ? snappedY :
			snappedZ;
	}

	Bounds BoxCollider::calculateBounds(const Vector3& center, const Vector3& size)
	{
		return { center, size, size.magnitude() * 2.f };
	}
}
