#pragma once
#include "Primitive.h"

namespace MathUtil
{
    struct Ray;
    class pow4Sphere :
        public Primitive
    {
    public:
        pow4Sphere(vec3 position, float radius)
        {
            m_Radius = radius;
            m_Position = position;
        }
        virtual AABB BoundingBox() const override;
        virtual bool Intersect(Ray& ray) const override;
        virtual HitData CalculateHitData(const Ray& ray) const override;
    };
}

