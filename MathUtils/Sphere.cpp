#include "vec.h"
#include "Ray.h"
#include "Sphere.h"
using namespace MathUtil;

AABB Sphere::BoundingBox() const
{
	return AABB(m_Position - vec3(m_Radius), m_Position + vec3(m_Radius));
}
bool Sphere::Intersect(Ray& ray) const
{

		//dir == 1
		float b = 2.f * (dot(ray.Origin, ray.Direction) - dot(ray.Direction, m_Position));
		float c = ray.Origin.sqrLentgh() + m_Position.sqrLentgh() - 2.f * dot(ray.Origin, m_Position) - (m_Radius * m_Radius);
		float disc = (b * b) - 4.f  * c;


		if (disc < 0.f)
			return false;

		float sqrtDisc = sqrtf(disc);
		float oneOverATimes2 = 1.f / (2.f);
		float t0 = -1.f;

		float t1 = (-b + sqrtDisc) * oneOverATimes2;
		float t2 = (-b - sqrtDisc) * oneOverATimes2;

		t0 = t1;

		
		if (t1 < 0.f && t2 < 0.f)
			return false;
		
		if (t2 > 0.f || t1 < t2)
		{
			t0 = t2;
		}

		if (t0 > ray.Distance || t0 < 0.f)
			return false;

		ray.Distance = t0;
	
	return true;
}
HitData Sphere::CalculateHitData(const Ray& ray) const
{
	HitData hd;
	hd.Position = ray.Origin + ray.Direction * ray.Distance;
	hd.Normal = normalize(hd.Position - m_Position);

	return hd;
}
