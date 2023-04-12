#include "ICollider.h"

#include "Arithmetic.h"
#include "Entity.h"
#include "Interpolation.h"
#include "Vector/Vector4.h"

using namespace LibMath;

namespace LibGL::Physics
{
	float Bounds::getSphereRadius() const
	{
		return max(max(m_size.m_x, m_size.m_y), m_size.m_z) / 2;
	}

	ICollider::~ICollider()
	{
		m_colliders.erase(std::ranges::find(m_colliders, this));
	}

	bool ICollider::check(const ICollider& other) const
	{
		const Bounds bounds = getBounds();
		const Bounds otherBounds = other.getBounds();

		const float boundsSphereRadius = bounds.getSphereRadius();
		const float otherBoundsSphereRadius = otherBounds.getSphereRadius();
		const float totalRadius = boundsSphereRadius + otherBoundsSphereRadius;

		return bounds.m_center.distanceSquaredFrom(otherBounds.m_center) <= totalRadius * totalRadius;
	}

	std::vector<const ICollider*> ICollider::getColliders()
	{
		return { m_colliders.begin(), m_colliders.end() };
	}

	ICollider::ICollider(Entity& owner, const Bounds& bounds) :
		Component(owner), m_bounds(bounds)
	{
		m_colliders.push_back(this);
	}

	Bounds ICollider::getBounds() const
	{
		const Vector3 worldCenter = getOwner().getGlobalTransform().getMatrix() * Vector4(m_bounds.m_center, 1.f);
		const Vector3 worldSize = m_bounds.m_size * getOwner().getGlobalTransform().getScale();
		return { worldCenter, worldSize };
	}

	Vector3 getClosestPointOnSegment(const Vector3& point,
		const Vector3& lineStart, const Vector3& lineEnd)
	{
		const Vector3 segmentVec = lineEnd - lineStart;
		const float t = (point - lineStart).dot(segmentVec) / segmentVec.magnitudeSquared();

		return lerp(lineStart, lineEnd, clamp(t, 0.f, 1.f));
	}
}
