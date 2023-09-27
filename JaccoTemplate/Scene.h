#pragma once
#include "vec.h"
#include "Ray.h"
#include "Object.h"
#include "Primitive.h"
#include "Material.h"
#include "Camera.h"
#include "Texture.h"
#include "BVH.h"
#include <memory>
#include <string>
#include <iterator>
#include <vector>
#include <map>

namespace Tmpl8
{

	class Scene
	{
	public:
		Scene() = default;
		Scene(MathUtil::vec3 color) { skyColor = color; }
		Scene(Texture* skybox);

		void Init();
		void Delete()
		{
			for (size_t i = 0; i < theObjects.size(); i++)
			{
				theObjects[i].Delete();
			}
		
		}
		
		void CreateObject(MathUtil::Primitive* shape, std::string material);
		inline void CreateMaterial(std::string name, MathUtil::Material* mat){ m_Materials[name] = mat;}

		std::vector<MathUtil::vec3> IntersectLight(const MathUtil::HitData& hd) ;
		bool Intersect(MathUtil::Ray& ray, MathUtil::HitData& data) ;
		
		void RayDistanceDephtofField(Camera& cam);
		
		inline const Texture* GetTexture() const { return EnviromentTexture; };
		inline const MathUtil::vec3 GetSkyColor() const { return skyColor; };
		
	
	private:
		//used map to reuse mat and have it nicely organized
		std::map<std::string, MathUtil::Material*> m_Materials;

		//made a distinqish between emmisive objects 
		//and not emmisive for directlight sampling
		std::vector<Object> theObjects;
		std::vector<Object> theLights;

		Texture* EnviromentTexture = nullptr;
		MathUtil::vec3 skyColor = MathUtil::vec3(0.f);

		//have two bvh (first try and improved one)
		//to be able to switch between them with a single define
		BVH m_BVH;
		bvh m_BVH2;
	};
}

