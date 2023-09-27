#include "vec.h"
#include "Ellepsiod.h"

using namespace MathUtil;

AABB Ellepsoid::BoundingBox() const
{
    return AABB(m_Position - m_EllepsoidRadius, m_Position + m_EllepsoidRadius);
}
bool Ellepsoid::Intersect(Ray& ray)const
{
    vec3 ocn = (ray.Origin - m_Position) / m_EllepsoidRadius;
    vec3 rdn = ray.Direction / m_EllepsoidRadius;

    float a = dot(rdn, rdn);
    float b = dot(ocn, rdn);
    float c = dot(ocn, ocn) - 1.f;
    float h = b * b - a * c;

    if (h < 0.f)
        return false;

    if (h >= 0.f)
    {
        float OneOvera = 1.f / a;
        float sqrootdisc = sqrtf(h);
        float t1 = (-b - sqrootdisc) * OneOvera;
        float t2 = (-b + sqrootdisc) * OneOvera;
        vec3 p1 = ray.Origin + ray.Direction * t1;
        vec3 p2 = ray.Origin + ray.Direction * t2;
        ray.TravelDist = (p1 - p2).length();
        if (t1 < 0.f || t1 > ray.Distance)
        {
            if (t2 < 0.f || t2 > ray.Distance)
                return false;

            ray.Distance = t2;
            return true;
        }
        ray.Distance = t1;
        return true;
    }
    return false;
}
HitData Ellepsoid::CalculateHitData(const Ray& ray) const
{
    HitData hd;
    vec3 ro = ray.Origin - m_Position;
    vec3 intersectionPoint = (ro + ray.Direction * ray.Distance);
    hd.Position = (intersectionPoint) + m_Position;
    hd.Normal = normalize((intersectionPoint) / ((m_EllepsoidRadius * m_EllepsoidRadius)));

    return hd;
}