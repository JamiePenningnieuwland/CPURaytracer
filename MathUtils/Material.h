#pragma once
#include "Ray.h"
namespace MathUtil
{
	enum class eMaterial
	{
		none = 0,
		metallic,
		emissive,
		diffuse,
		checkerboard,
		dielectric,
		metalCheckerboard,
		Texture
	};
	class Material
	{
	public:
	
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const = 0;
		vec3 Reflect(const vec3& normal, const vec3& direction) const;
		vec3 Absorbance(vec3 color, float distance) const;
		eMaterial myType = eMaterial::diffuse;
		
	};
	class Emissive :public Material
	{
	public:
		Emissive() = default;
		Emissive(vec3 color, float brigtness)
		{
			m_Color = color;
			m_Brigthness = brigtness;
			myType = eMaterial::emissive;
		};
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;
		const vec3 GetColor() const { return m_Color; };
		const float GetBrightness() const { return  m_Brigthness; };

	private:
		vec3 m_Color;
		float m_Brigthness;
	};
	class Metallic : public Material
	{
	public:
		Metallic() = default;
		Metallic(vec3 color, float fuzzy)
		{
			m_Color = color;
			m_Fuzzy = fuzzy;
			myType = eMaterial::metallic;
		}
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;

	private:
		vec3 m_Color = { 0,0,0 };
		float m_Fuzzy = { 1.f };
	};
	class MetalCheckerboard :public Material
	{
	public:
		MetalCheckerboard() = default;
		MetalCheckerboard(vec3 color_a, vec3 color_b, float fuzzy)
		{
			m_Color_a = color_a;
			m_Color_b = color_b;
			m_Fuzzy = fuzzy;
			myType = eMaterial::metalCheckerboard;
		}
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;

	private:
		vec3 m_Color_a = { 0,0,0 };
		vec3 m_Color_b = { 0,0,0 };
		vec3 m_Fuzzy = { 0,0,0 };
	};
	class Diffuse : public Material
	{
	public:
		Diffuse() = default;
		Diffuse(vec3 color)
		{
			m_Color = color;
			myType = eMaterial::diffuse;
		}
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;

	private:
		
		vec3 m_Color = { 0,0,0 };
	};
	class Checkerboard :public Material
	{
	public:
		Checkerboard() = default;
		Checkerboard(vec3 color_a, vec3 color_b)
		{
			m_Color_a = color_a;
			m_Color_b = color_b;
			myType = eMaterial::checkerboard;
		}
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;

	private:
		vec3 m_Color_a = { 0,0,0 };
		vec3 m_Color_b = { 0,0,0 };
	};
	class Dielectric :public Material
	{
	public:
		Dielectric() = default;
		Dielectric(vec3 color, float ri)
		{
			m_ref_idx = ri;
			m_color = color;
			myType = eMaterial::dielectric;
		};
		virtual bool ScatteredRay(const Ray& ray_in, const HitData& hd, vec3& color, Ray& scattered_ray) const override;

		//public for unittest
		bool Refract(const vec3& rd, const vec3& normal, float ni_over_nt, vec3& refracted) const;

	private:
		float Schlik(float cosine, float ref_idx) const;
		float m_ref_idx;
		vec3 m_color;
	};
};
