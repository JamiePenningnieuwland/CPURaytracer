#pragma once
#include "Ray.h"

namespace MathUtil
{
	class AABB
	{
	public:
		AABB()= default;
		AABB(const vec3& a, const vec3& b) { m_Minimum = a;  m_Maximum = b;}
		
		bool Intersect(const Ray& ray);
		float IntersectDist(const Ray& ray);
		bool IsInBox(const vec3& point);
	
		vec3 Offset(const vec3& point) const;
		inline vec3 Diagonal() const { return m_Maximum - m_Minimum; };
		inline vec3 GetCenter() const { return (m_Minimum + m_Maximum) / 2.f;}
	
		inline const vec3& GetMin() const { return m_Minimum; };
		inline const vec3& GetMax() const { return m_Maximum; };

		static AABB SurroundingBox(AABB box0, AABB box1);
		
		static AABB SurroundingBox(AABB box0, vec3 p);
		float SurfaceArea() const;

	private:

		vec3 m_Maximum;
		vec3 m_Minimum;
		
	};

}


