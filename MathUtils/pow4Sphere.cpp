#include "vec.h"
#include "Ray.h"
#include "pow4Sphere.h"


using namespace MathUtil;

AABB pow4Sphere::BoundingBox() const
{
    return AABB(m_Position - vec3(m_Radius), m_Position + vec3(m_Radius));
}
bool pow4Sphere::Intersect(Ray& ray) const
{
    vec3 ro = ray.Origin - m_Position;
    vec3 rd = ray.Direction;
    float ra = m_Radius;
 
    // quartic equation
    float r2 = ra * ra;

    vec3 d2 = rd * rd; vec3 d3 = d2 * rd;
    vec3 o2 = ro * ro; vec3 o3 = o2 * ro;

    float ka = 1.0f / dot(d2, d2);

    float k3 = ka * dot(ro, d3);
    float k2 = ka * dot(o2, d2);
    float k1 = ka * dot(o3, rd);
    float k0 = ka * (dot(o2, o2) - r2 * r2);

 
    // cubic
    float c2 = k2 - k3 * k3;
    float c1 = k1 + 2.0f * k3 * k3 * k3 - 3.0f * k3 * k2;
    float c0 = k0 - 3.0f * k3 * k3 * k3 * k3 + 6.0f * k3 * k3 * k2 - 4.0f * k3 * k1;

    float p = c2 * c2 + c0 / 3.0f;
    float q = c2 * c2 * c2 - c2 * c0 + c1 * c1;

    float h = q * q - p * p * p;

    if (h < 0.0f) return false;
    float sh = sqrtf(h);

    float s = sign(q + sh) * powf(abs(q + sh), 1.0f / 3.0f); // cuberoot
    float t = sign(q - sh) * powf(abs(q - sh), 1.0f / 3.0f); // cuberoot
    vec2  w = vec2(s + t, s - t);

    vec2  v = vec2(w.x + c2 * 4.0f, w.y * sqrtf(3.0)) * 0.5f;
    float r = v.length();


    float d = -abs(v.y) / sqrtf(r + v.x) - c1 / r - k3;

    if (d < 0.f || d > ray.Distance)
        return false;

    ray.Distance = d;
    return true;
}
HitData pow4Sphere::CalculateHitData(const Ray& ray)const
{
    HitData hd;
    vec3 ro = ray.Origin - m_Position;
    vec3 intersectionPoint = ro + ray.Direction * ray.Distance;
    hd.Position = intersectionPoint + m_Position;
    hd.Normal = normalize(intersectionPoint * intersectionPoint * intersectionPoint);
  
    return hd;
}
    