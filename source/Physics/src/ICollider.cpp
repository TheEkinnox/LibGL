#include "ICollider.h"

#include "Arithmetic.h"
#include "Entity.h"
#include "Interpolation.h"
#include "Vector/Vector4.h"

using namespace LibMath;

namespace LibGL::Physics
{
	Vector3 Ray::getClosestPoint(const Vector3& point) const
	{
		Vector3 projPoint = point - m_origin;
		projPoint.projectOnto(m_direction);

		return m_origin + projPoint;
	}

	std::pair<Vector3, Vector3> Ray::getClosestPoints(const Ray& other) const
	{
		const Vector3 toOther = m_origin - other.m_origin;

		const float sqrLen = m_direction.magnitudeSquared();
		const float otherSqrLen = other.m_direction.magnitudeSquared();

		const float angle = m_direction.dot(other.m_direction);
		const float angleToDiff = m_direction.dot(toOther);
		const float otherAngleToDiff = other.m_direction.dot(toOther);

		const float denominator = sqrLen * otherSqrLen - angle * angle;

		if (floatEquals(denominator, 0.f))
		{
			return
			{
				m_origin,
				other.m_origin + otherAngleToDiff / otherSqrLen * other.m_direction
			};
		}
		else
		{
			return
			{
				m_origin + (angle * otherAngleToDiff - otherSqrLen * angleToDiff) / denominator * m_direction,
				other.m_origin + (sqrLen * otherAngleToDiff - angle * angleToDiff) / denominator * other.m_direction
			};
		}
	}

	float Ray::distanceSquaredFrom(const LibMath::Vector3& point) const
	{
		return point.distanceSquaredFrom(getClosestPoint(point));
	}

	float Ray::distanceSquaredFrom(const Ray& other) const
	{
		const auto [ closest, otherClosest ] = getClosestPoints(other);
		return closest.distanceSquaredFrom(otherClosest);
	}

	ICollider::~ICollider()
	{
		m_colliders.erase(std::ranges::find(m_colliders, this));
	}

	Bounds ICollider::getBounds() const
	{
		const Transform transform = getOwner().getGlobalTransform();
		const Vector3 worldCenter = transform.getMatrix() * Vector4(m_bounds.m_center, 1.f);

		const Vector3 scale = transform.getScale();
		const Vector3 worldSize = m_bounds.m_boxSize * scale;

		const float radiusScale = max(max(scale.m_x, scale.m_y), scale.m_z);
		const float worldRadius = m_bounds.m_sphereRadius * radiusScale;

		return { worldCenter, worldSize, worldRadius };
	}

	bool ICollider::check(const Vector3& point) const
	{
		const auto [center, size, radius] = getBounds();
		return point.distanceSquaredFrom(center) <= radius * radius;
	}

	bool ICollider::check(const Ray& ray) const
	{
		const auto [center, _, radius] = getBounds();
		return ray.distanceSquaredFrom(center) <= radius * radius;
	}

	bool ICollider::check(const ICollider& other) const
	{
		const auto [center, _, sphereRadius] = getBounds();
		const auto [otherCenter, _o, otherSphereRadius] = other.getBounds();

		const float totalRadius = sphereRadius + otherSphereRadius;

		return center.distanceSquaredFrom(otherCenter) <= totalRadius * totalRadius;
	}

	std::vector<ICollider*> ICollider::getColliders()
	{
		return m_colliders;
	}

	ICollider::ICollider(Entity& owner, const Bounds& bounds) :
		Component(owner), m_bounds(bounds)
	{
		m_colliders.push_back(this);
	}

	Vector3 getClosestPointOnSegment(const Vector3& point,
		const Vector3& lineStart, const Vector3& lineEnd)
	{
		const Vector3 segmentVec = lineEnd - lineStart;
		const float t = (point - lineStart).dot(segmentVec) / segmentVec.magnitudeSquared();

		return lerp(lineStart, lineEnd, clamp(t, 0.f, 1.f));
	}
}
