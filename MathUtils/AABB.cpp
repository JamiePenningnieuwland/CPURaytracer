#include "vec.h"
#include "AABB.h"
#include <algorithm>

using namespace MathUtil;


bool AABB::Intersect( const Ray& ray)
{
  
    vec3 dirfrac;
    
    if (ray.Direction.x != 0.f)
        dirfrac.x = 1.0f / ray.Direction.x;
    else dirfrac.x = 0;

    if (ray.Direction.y != 0.f)
        dirfrac.y = 1.0f / ray.Direction.y;
    else dirfrac.y = 0;

    if (ray.Direction.z != 0.f)
        dirfrac.z = 1.0f / ray.Direction.z;
    else dirfrac.z = 0;

   
    float t1 = (m_Minimum.x - ray.Origin.x) * dirfrac.x;
    float t2 = (m_Maximum.x - ray.Origin.x) * dirfrac.x;
    float t3 = (m_Minimum.y - ray.Origin.y) * dirfrac.y;
    float t4 = (m_Maximum.y - ray.Origin.y) * dirfrac.y;
    float t5 = (m_Minimum.z - ray.Origin.z) * dirfrac.z;
    float t6 = (m_Maximum.z - ray.Origin.z) * dirfrac.z;

    float tmin = Max(Max(Min(t1, t2), Min(t3, t4)), Min(t5, t6));
    float tmax = Min(Min(Max(t1, t2), Max(t3, t4)), Max(t5, t6));

   
    if (tmax < 0)
         return false;
    
    if (tmin > tmax)
         return false;
    

    return true;

}
float AABB::IntersectDist(const Ray& ray)
{

    vec3 dirfrac;

    if (ray.Direction.x != 0.f)
        dirfrac.x = 1.0f / ray.Direction.x;
    else dirfrac.x = 0;

    if (ray.Direction.y != 0.f)
        dirfrac.y = 1.0f / ray.Direction.y;
    else dirfrac.y = 0;

    if (ray.Direction.z != 0.f)
        dirfrac.z = 1.0f / ray.Direction.z;
    else dirfrac.z = 0;


    float t1 = (m_Minimum.x - ray.Origin.x) * dirfrac.x;
    float t2 = (m_Maximum.x - ray.Origin.x) * dirfrac.x;
    float t3 = (m_Minimum.y - ray.Origin.y) * dirfrac.y;
    float t4 = (m_Maximum.y - ray.Origin.y) * dirfrac.y;
    float t5 = (m_Minimum.z - ray.Origin.z) * dirfrac.z;
    float t6 = (m_Maximum.z - ray.Origin.z) * dirfrac.z;

    float tmin = Max(Max(Min(t1, t2), Min(t3, t4)), Min(t5, t6));
    float tmax = Min(Min(Max(t1, t2), Max(t3, t4)), Max(t5, t6));


    if (tmax < 0)
        return INFINITY;

    if (tmin > tmax)
        return INFINITY;


    return tmin;

}

AABB AABB::SurroundingBox(AABB box0, AABB box1)
{
    vec3 small = vec3(
        Min(box0.GetMin().x, box1.GetMin().x),
        Min(box0.GetMin().y, box1.GetMin().y),
        Min(box0.GetMin().z, box1.GetMin().z)
    );

    vec3 big = vec3(
        Max(box0.GetMax().x, box1.GetMax().x),
        Max(box0.GetMax().y, box1.GetMax().y),
        Max(box0.GetMax().z, box1.GetMax().z)
    );

    return AABB(small, big);
}
AABB AABB::SurroundingBox(AABB box0, vec3 p)
{
    vec3 small = vec3(
        Min(box0.GetMin().x, p.x),
        Min(box0.GetMin().y, p.y),
        Min(box0.GetMin().z, p.z)
    );

    vec3 big = vec3(
        Max(box0.GetMax().x, p.x),
        Max(box0.GetMax().y, p.y),
        Max(box0.GetMax().z, p.z)
    );

    return AABB(small, big);
}

//made for attempt on sah
float AABB::SurfaceArea() const
{
    const vec3 d = Diagonal();
    return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

bool AABB::IsInBox(const vec3& point)
{
    return point.x < m_Maximum.x&& point.x > m_Minimum.x &&
        point.y < m_Maximum.y&& point.y > m_Minimum.y &&
        point.z < m_Maximum.z&& point.z > m_Minimum.z;
}

vec3 AABB::Offset(const vec3& point) const
{
    vec3 o = point - m_Minimum;
    if (m_Maximum.x > m_Minimum.x) o.x /= m_Maximum.x - m_Minimum.x;
    if (m_Maximum.y > m_Minimum.y) o.y /= m_Maximum.y - m_Minimum.y;
    if (m_Maximum.z > m_Minimum.z) o.z /= m_Maximum.z - m_Minimum.z;
    return o;
}

