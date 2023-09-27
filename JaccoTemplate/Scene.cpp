#include "precomp.h"
#include "Scene.h"

#define BVH2    1
#define BVH1    0
#define NOBVH   0

using namespace Tmpl8;

Scene::Scene(Texture* thesky)
{
	EnviromentTexture = thesky;
}
void Scene::CreateObject(MathUtil::Primitive* shape, std::string material)
{
	if(m_Materials[material]->myType == MathUtil::eMaterial::emissive)
	{
		theLights.push_back(Object(shape, m_Materials[material]));
	}
	theObjects.push_back(Object(shape, m_Materials[material]));

	//get last created object
	theObjects[theObjects.size() - 1].myIndex = static_cast<int>(theObjects.size()) - 1;
}
void Scene::RayDistanceDephtofField(Camera& cam)
{
	int index = -1;
	float fd = 0.f;
	float rays_hit = 0.f;
	MathUtil::Ray rays[5];
	rays[0] = cam.ShootRay(MathUtil::vec2(0.5f, 0.5f), true);
	rays[1] = cam.ShootRay(MathUtil::vec2(0.75f, 0.75f), true);
	rays[2] = cam.ShootRay(MathUtil::vec2(0.25f, 0.25f), true);
	rays[3] = cam.ShootRay(MathUtil::vec2(0.25f, 0.75f), true);
	rays[4] = cam.ShootRay(MathUtil::vec2(0.75f, 0.25f), true);

	for (int j = 0; j < 5; j++)
	{
		for (size_t i = 0; i < theObjects.size(); i++)
		{

			if (theObjects[i].GetPrimitive().Intersect(rays[j]))
			{
				index = static_cast<int>(i);
			}

		}
		if (index > 0)
		{
			rays_hit += 1.f;
			fd += rays[j].Distance;
		}
	}
	if (rays_hit > 0.f && fd / rays_hit > 0.f)
	{
		cam.SetFd(fd / rays_hit);
	}


}
void Scene::Init()
{
	std::vector<Object*> objectPtr;
	for(auto& object: theObjects)
	{
		objectPtr.push_back(&object);
	}
#if BVH2
	m_BVH2.Generate(objectPtr);

#elif BVH1
	m_BVH.Generate(objectPtr);
#endif // bvh1


}
std::vector<MathUtil::vec3> Scene::IntersectLight(const MathUtil::HitData& hd)
{
	std::vector<MathUtil::vec3> colors;
	if (theLights.size() <= 0)
		return colors;

	for( auto& light :theLights)
	{
		MathUtil::Ray Shadowray;
		MathUtil::vec3 random = light.GetPrimitive().GetPosition() + (MathUtil::RandWithinUnitSphere() * light.GetPrimitive().GetRadius());
		Shadowray.Origin = hd.Position + hd.Normal * 0.001f;
		MathUtil::vec3 substracthd = random - hd.Position;
		Shadowray.Direction = normalize(substracthd);

		//not correct because dielectred should not cause shadow
		int index = -1;

		for (size_t i = 0; i < theObjects.size(); i++)
		{
			if (theObjects[i].GetPrimitive().Intersect(Shadowray))

			{
				index = static_cast<int>(i);
			}
		}

		if (index > 0)
		{
			MathUtil::HitData emissiveHitData;
			emissiveHitData = theObjects[index].GetPrimitive().
				CalculateHitData(Shadowray);

			if (theObjects[index] == light)
			{
				float sqrdis = Shadowray.Distance * Shadowray.Distance;
				float n = MathUtil::dot(hd.Normal, -Shadowray.Direction) / sqrdis;

				MathUtil::vec3 intesity = ((MathUtil::Emissive*)(&light.GetMaterial()))->GetColor() 
					* ((MathUtil::Emissive*)(&light.GetMaterial()))->GetBrightness();
				MathUtil::vec3 newcolor;
				newcolor = n * intesity;
				colors.push_back(newcolor);
			}
		}
	}
	return colors;
}
bool Scene::Intersect(MathUtil::Ray& ray, MathUtil::HitData& hd)
{
#if BVH2


	std::vector<int> HittableObjects;
	HittableObjects.reserve(32);
	if (m_BVH2.Intersect(ray, HittableObjects, 0))
	{
		int index = -1;
		for (size_t i = 0; i < HittableObjects.size(); i++)
		{
			if (theObjects[HittableObjects[i]].GetPrimitive().Intersect(ray))
			{
				index = static_cast<int>(i);
			}
		}

		if (index >= 0)
		{
			hd = theObjects[HittableObjects[index]].GetPrimitive().CalculateHitData(ray);
			hd.material = &theObjects[HittableObjects[index]].GetMaterial();
			return true;
		}

	}
	return false;



#elif BVH1
	std::vector<Object*> HittableObjects;
	HittableObjects.reserve(32);

	if (m_BVH.Intersect(ray, HittableObjects))
	{

		int index = -1;
		for (size_t i = 0; i < HittableObjects.size(); i++)
		{
			if (HittableObjects[i]->GetPrimitive().Intersect(ray))
			{
				index = static_cast<int>(i);
			}
		}
		if (index >= 0)
		{
			hd = HittableObjects[index]->GetPrimitive().CalculateHitData(ray);
			hd.material = &HittableObjects[index]->GetMaterial();
			return true;
		}
	}
	return false;
#elif NOBVH

		int index = -1;
		for (size_t i = 0; i < theObjects.size(); i++)
		{
			if (theObjects[i].GetPrimitive().Intersect(ray))
			{
				index = static_cast<int>(i);
			}

		}
		if (index >= 0)
		{
			hd = theObjects[index].GetPrimitive().CalculateHitData(ray);
			hd.material = &theObjects[index].GetMaterial();
			return true;
		}
	
	return false;
#endif // NOBVH

}
