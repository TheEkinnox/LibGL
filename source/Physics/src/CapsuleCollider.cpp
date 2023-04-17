#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

#include "Arithmetic.h"
#include "Entity.h"
#include "Debug/Log.h"
#include "Matrix/Matrix4.h"
#include "Vector/Vector4.h"

using namespace LibMath;

namespace LibGL::Physics
{
	CapsuleCollider::CapsuleCollider(Entity& owner, const Vector3& center,
		const Vector3& upDir, const float height, const float radius) :
		ICollider(owner, calculateBounds(center, upDir.normalized(), max(height, radius * 2.f), radius)),
		m_center(center), m_upDirection(upDir.normalized()), m_height(max(height, radius * 2.f)),
		m_radius(radius)
	{
	}

	Vector3 CapsuleCollider::getUpDirection() const
	{
		return m_upDirection;
	}

	float CapsuleCollider::getHeight() const
	{
		return getBounds().m_sphereRadius * 2.f;
	}

	float CapsuleCollider::getRadius() const
	{
		const auto ownerScale = getOwner().getGlobalTransform().getScale();

		const Matrix4 rotationMat = Matrix4::rotationFromTo(Vector3::up(), m_upDirection);
		const Vector3 rightScale = static_cast<Vector3>(rotationMat * Vector4::right()) * ownerScale;
		const Vector3 frontScale = static_cast<Vector3>(rotationMat * Vector4::front()) * ownerScale;
		
		return (rightScale.isLongerThan(frontScale) ? rightScale : frontScale).magnitude() * m_radius;
	}

	bool CapsuleCollider::check(const Vector3& point) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(point))
			return false;

		const auto [center, _, halfHeight] = getBounds();
		const auto radius = getRadius();
		const auto orientedHeight = getUpDirection() * (halfHeight - radius) * 2.f;

		// Get the closest point on the capsule's center segment
		const Vector3 closestPoint = getClosestPointOnSegment(point,
			orientedHeight / 2.f - center,
			orientedHeight / 2.f + center);

		return point.distanceSquaredFrom(closestPoint) <= radius * radius;
	}

	bool CapsuleCollider::check(const Ray& ray) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(ray))
			return false;

		const auto [center, _, halfHeight] = getBounds();
		const float radius = getRadius();
		const auto orientedHeight = getUpDirection() * (halfHeight - radius) * 2.f;
		
		const Ray centerRay = { center, getUpDirection() };

		auto [ rayClosest, capsuleClosest ] = ray.getClosestPoints(centerRay);

		capsuleClosest = getClosestPointOnSegment(capsuleClosest,
			center - orientedHeight / 2.f,
			center + orientedHeight / 2.f);

		rayClosest = ray.getClosestPoint(capsuleClosest);

		return rayClosest.distanceSquaredFrom(capsuleClosest) <= radius * radius;
	}

	bool CapsuleCollider::check(const ICollider& other) const
	{
		const auto otherTypeHash = typeid(other).hash_code();

		if (otherTypeHash == typeid(BoxCollider).hash_code())
			return checkBox(dynamic_cast<const BoxCollider&>(other));

		if (otherTypeHash == typeid(SphereCollider).hash_code())
			return checkSphere(dynamic_cast<const SphereCollider&>(other));

		if (otherTypeHash == typeid(CapsuleCollider).hash_code())
			return checkCapsule(dynamic_cast<const CapsuleCollider&>(other));

		DEBUG_LOG("WARNING: collisions between 'CapsuleCollider' and '%s' are not supported.\n", typeid(other).name());
		return false;
	}

	bool CapsuleCollider::checkBox(const BoxCollider& other) const
	{
		return other.checkCapsule(*this);
	}

	bool CapsuleCollider::checkSphere(const SphereCollider& other) const
	{
		return other.checkCapsule(*this);
	}

	bool CapsuleCollider::checkCapsule(const CapsuleCollider& other) const
	{
		// Check the bounding spheres first to avoid unnecessary computation
		if (!ICollider::check(other))
			return false;

		const auto [center, _, halfHeight] = getBounds();
		const float radius = getRadius();
		const auto orientedHeight = getUpDirection() * (halfHeight - radius) * 2.f;

		const auto [otherCenter, _o, otherHalfHeight] = other.getBounds();
		const float otherRadius = other.getRadius();
		const auto otherOrientedHeight = other.getUpDirection() * (otherHalfHeight - otherRadius) * 2.f;

		const float totalRadius = radius + otherRadius;

		const Ray ray = { center, getUpDirection() };
		const Ray otherRay = { otherCenter, other.getUpDirection() };

		auto [closest, otherClosest] = ray.getClosestPoints(otherRay);

		closest = getClosestPointOnSegment(closest,
			center - orientedHeight / 2.f,
			center + orientedHeight / 2.f);

		otherClosest = getClosestPointOnSegment(closest,
			otherCenter - otherOrientedHeight / 2.f,
			otherCenter + otherOrientedHeight / 2.f);

		return closest.distanceSquaredFrom(otherClosest) <= totalRadius * totalRadius;
	}

	Vector3 CapsuleCollider::getClosestPoint(const Vector3& point) const
	{
		const auto [ center, _, halfHeight ] = getBounds();
		const float radius = getRadius();

		const Vector3 offset = getUpDirection() * (halfHeight - radius);
		const Vector3 base = center - offset;
		const Vector3 tip = center + offset;

		const Vector3 centerPoint = getClosestPointOnSegment(point, base, tip);

		return centerPoint + (point - centerPoint).normalized() * radius;
	}

	Bounds CapsuleCollider::calculateBounds(const Vector3& center, const Vector3& upDir,
		const float height, const float radius)
	{
		const Matrix4 rotationMat = Matrix4::rotationFromTo(Vector3::up(), upDir);
		const Vector3 rightDir = (rotationMat * Vector4::right()).xyz();
		const Vector3 frontDir = (rotationMat * Vector4::front()).xyz();

		return
		{
			center,
			upDir * height + rightDir * radius + frontDir * radius,
			height / 2.f
		};
	}
}
