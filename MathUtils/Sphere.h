#pragma once
#include "Primitive.h"

namespace MathUtil
{
    struct Ray;
    class Sphere :
        public Primitive
    {
    public:
        Sphere() = default;
        Sphere(const vec3& position, float radius)
        {
            m_Position = position;
            m_Radius = radius;
           
        }
       
        virtual AABB BoundingBox() const override;
        virtual bool Intersect(Ray& ray) const override;
        virtual HitData CalculateHitData(const Ray& ray) const override;

    };
}

