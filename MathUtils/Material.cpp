#include "vec.h"
#include "Material.h"
using namespace MathUtil;

bool Dielectric::Refract(const vec3& rd, const vec3& normal, float ni_over_nt, vec3& refracted) const
{
	vec3 uv = rd;
	float dt = dot(uv, normal);
	float disc = 1.0f - ni_over_nt * ni_over_nt * (1.f - dt * dt);
	if (disc < 0.f)
		return false;
	refracted = ni_over_nt * (uv - normal * dt) - normal * sqrtf(disc);
	return true;
}
vec3 Material::Reflect(const vec3& normal, const vec3& direction) const
{
	float DotIN = dot(direction, normal);
	vec3 reflected = direction - 2 * DotIN * normal;
	return reflected;
}
vec3 Material::Absorbance(vec3 color, float distance) const
{

	vec3 inversedcolor = vec3(1.0f, 1.0f, 1.0f) - color;
		const vec3 abscolor = inversedcolor * (2.5f) * -distance;
		const vec3 trans = vec3(
			expf(abscolor.r),
			expf(abscolor.g),
			expf(abscolor.b)
		);
		
		return color * trans;
}
float Dielectric::Schlik(float cosine, float ref_idx) const
{
	float r0 = (1.f - ref_idx) / (1.f + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.f - r0) * powf((1.f - cosine), 5.f);
}

bool Metallic::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{
	scattered_ray.Origin = hd.Position + hd.Normal * 0.001f;
	float DotIN = dot(ray_in.Direction, hd.Normal);
	scattered_ray.Direction = ray_in.Direction - 2 * DotIN * hd.Normal;
	scattered_ray.Direction += RandWithinUnitSphere() * m_Fuzzy;
	scattered_ray.Direction.normalize();
	color = m_Color;
	return true;
}

bool Diffuse::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{

	vec3 target = hd.Normal + normalize(RandWithinUnitSphere());
	scattered_ray.Origin = hd.Position + hd.Normal * 0.0001f;
	scattered_ray.Direction = normalize(target);
	color = m_Color;

	return true;
}

bool Checkerboard::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{
	vec3 target = hd.Position + hd.Normal + normalize(RandWithinUnitSphere());

	scattered_ray.Origin = hd.Position + hd.Normal * 0.0001f;
	scattered_ray.Direction = normalize(target - hd.Position);
	int movement_x = hd.Position.x < 0.001f ? 1 : 0;
	int movement_z = hd.Position.z < 0.001f ? 1 : 0;
	if (((static_cast<int>(hd.Position.x)+ movement_x) + static_cast<int>(hd.Position.z)+ movement_z) % 2 == 0)
	{
		color = m_Color_a;
	}
	else color = m_Color_b;
	
	return true;
}


bool Dielectric::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{
	vec3 refracted;

	float ni_over_nt;
	float reflect_prob;
	float cosine;
	vec3 outwardNormal;

	vec3 temp = Absorbance(m_color, ray_in.TravelDist);
	color =  temp;
	if(dot(ray_in.Direction, hd.Normal) > 0.f)
	{
		//ray origin inside
		outwardNormal = -hd.Normal;
		ni_over_nt = m_ref_idx;// +indexOffset;
		cosine = dot(ray_in.Direction, hd.Normal);
	}
	else 
	{
		outwardNormal = hd.Normal;
		ni_over_nt = 1.f / (m_ref_idx);
		cosine = -dot(ray_in.Direction, hd.Normal);
	}

	if (Refract(ray_in.Direction, outwardNormal, ni_over_nt, refracted  ))
	{
		reflect_prob = Schlik(cosine, m_ref_idx);
	}
	else reflect_prob = 1.f;

	if(Rand(1.f) < reflect_prob)
	{
		scattered_ray.Origin = hd.Position;
		scattered_ray.Direction = Reflect(hd.Normal, ray_in.Direction);
	}
	else 
	{
		scattered_ray.Origin = hd.Position + refracted * 0.001f;
		scattered_ray.Direction = refracted;
	}
	scattered_ray.Direction.normalize();
	return true;
}


bool Emissive::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{
	
	vec3 target = hd.Normal + normalize(RandWithinUnitSphere());
	// bloom here
	scattered_ray.Origin = hd.Position + hd.Normal * 0.001f;
	color = m_Color * m_Brigthness;

	return false;
}

bool MetalCheckerboard::ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const
{

	scattered_ray.Origin = hd.Position + hd.Normal * 0.001f;
	float DotIN = dot(ray_in.Direction, hd.Normal);
	scattered_ray.Direction = ray_in.Direction - 2 * DotIN * hd.Normal;
	scattered_ray.Direction += RandWithinUnitSphere() * m_Fuzzy;
	scattered_ray.Direction.normalize();
	color = m_Color_a;
	int movement_x = hd.Position.x < 0.001f ? 1 : 0;
	int movement_z = hd.Position.z < 0.001f ? 1 : 0;
	if (((static_cast<int>(hd.Position.x) + movement_x) + static_cast<int>(hd.Position.z) + movement_z) % 2 == 0)
	{
		color = m_Color_a;
	}
	else color = m_Color_b;

	return true;
}
