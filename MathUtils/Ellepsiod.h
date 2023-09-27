#pragma once
#include "Primitive.h"

namespace MathUtil
{
    class Ellepsoid :
        public Primitive
    {
    public:
        Ellepsoid(const vec3& position, vec3 radius)
        {
            m_EllepsoidRadius = radius;
            m_Position = position;
        }
        virtual AABB BoundingBox() const override;
        virtual bool Intersect(Ray& ray) const override;
        virtual HitData CalculateHitData(const Ray& ray) const override;
    private:
       vec3 m_EllepsoidRadius;
    };
}

