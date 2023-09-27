#pragma once
#include <string>
#include "vec.h"
#include"Material.h"
namespace Tmpl8
{

	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& filePath);
		~Texture();
		MathUtil::vec3 SampleTexture(MathUtil::vec2 uv) const;
		inline int GetWidth()const;
		inline int GetHeight() const;

	private:
		int m_Width;
		int m_Heigth;
		float* m_data{nullptr};
		int numComponent;
	};
	//should not be here, material should be outside mathutil
	class Textured:public MathUtil::Material
	{
	public:
		Textured() = default;
		//set it to nullptr so it will compile even if this parameter is not set
		Textured(Sprite* texture, MathUtil::eMaterial thetype, Sprite* normalTexture = nullptr, Sprite* metalTexture = nullptr, float numtiles = 1.f)
		{
			m_DiffuseTexture = texture;
			myType = thetype;
			m_NormalTexture = normalTexture;
			m_MetalTexture = metalTexture;
			m_tiling = numtiles;

		}
		virtual bool ScatteredRay(const MathUtil::Ray& ray_in, const MathUtil::HitData& hd, MathUtil::vec3& color, MathUtil::Ray& scattered_ray) const override;
	private:
		MathUtil::vec2 getUV(const MathUtil::vec3& normal) const
		{
			MathUtil::vec3 n = normal;

			float u = atan2f(n.x, n.z) / static_cast<float>((2.f * MathUtil::PI)) + 0.5f;
			float v = n.y * 0.5f + 0.5f;
			return MathUtil::vec2(u, v);
		}
		MathUtil::vec3 IntToVec3(int color) const
		{
			float OneOver255 = 1.f / 255.f;

			int newR = (0xFF0000 & color) >> (8 * 2);
			int newG = (0x00FF00 & color) >> (8 * 1);
			int newB = (0x0000FF & color) >> (8 * 0);

			float R = (static_cast<float>(newR) * OneOver255);
			float G = (static_cast<float>(newG) * OneOver255);
			float B = (static_cast<float>(newB) * OneOver255);

			return MathUtil::vec3{ R,G,B };
		}
		MathUtil::mat4 CreateLocalMatrix(const MathUtil::vec3& forward, const MathUtil::vec3& Up, const MathUtil::vec3& rigth) const;

	private:
		Sprite* m_DiffuseTexture = nullptr;
		Sprite* m_NormalTexture = nullptr;
		Sprite* m_MetalTexture = nullptr;
		float m_tiling = 1.f;
	};
}
