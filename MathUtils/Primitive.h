#pragma once
#include "Material.h"
#include "AABB.h"
namespace MathUtil
{
	struct Ray;
	class Material;
	class Primitive
	{
	public:
		Primitive() {};
		virtual ~Primitive();

		virtual AABB BoundingBox() const = 0;
		virtual bool Intersect(Ray& ray) const = 0;
		virtual HitData CalculateHitData(const Ray& ray) const = 0;

		inline const float& GetRadius() const { return m_Radius; }
		inline const vec3& GetPosition() const { return m_Position; }
	
	protected:
		float m_Radius = 0.f;
		vec3 m_Position = vec3(0.f);

	};

};
