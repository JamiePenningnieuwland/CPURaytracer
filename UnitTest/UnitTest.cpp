#include "pch.h"
#include "../MathUtils/vec.h"
#include "../MathUtils/Ray.h"
#include "../MathUtils/Primitive.h"
#include "../MathUtils/Ellepsiod.h"
#include "../MathUtils/Material.h"
#include "../MathUtils/Sphere.h"
#include "../MathUtils/AABB.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MathUtil;

namespace UnitTest
{
	TEST_CLASS(Intersections)
	{
	public:
		void vec3AreEqual(vec3 expected, vec3 result)
		{
			Assert::AreEqual(expected.x, result.x);
			Assert::AreEqual(expected.y, result.y);
			Assert::AreEqual(expected.z, result.z);
		}

		//spheres
		TEST_METHOD(sphere)
		{
			Sphere sphere = Sphere(vec3(0.f, 0.f, 0.f), 1.f);
			Ray ray0;
			Ray ray1;

			ray0.Origin = vec3(0.f, 0.f, -4.f);
			ray1.Origin = vec3(0.f, 0.f, -4.f);

			ray0.Direction = normalize(sphere.GetPosition() - ray0.Origin);
			ray1.Direction = normalize(sphere.GetPosition() + ray1.Origin);

			bool test0 = sphere.Intersect(ray0);
			bool test1 = sphere.Intersect(ray1);

			Assert::IsTrue(test0);
			Assert::IsFalse(test1);
		
			Ray ray2;
			HitData hd0;
			ray2.Origin = vec3(0.f, 0.f, 0.f);
			ray2.Direction = vec3(1.f, 0.f, 0.f);
			
			sphere.Intersect(ray2);
			hd0 = sphere.CalculateHitData(ray2);
			
			// test distance
			Assert::AreEqual(1.f, ray2.Distance);

			//test position of intersection
			vec3AreEqual(vec3(1.f, 0.f, 0.f), hd0.Position);
			
		}
		//ellipsoids
		TEST_METHOD(ellipsoid)
		{

			Ellepsoid ellep = Ellepsoid((0.f, 0.f, 0.f), vec3(1.f,2.f,1.f));	
			Ray ray0;
			Ray ray1;
			ray0.Origin = vec3(0.f, 0.f, -4.f);
			ray1.Origin = vec3(0.f, 0.f, -4.f);
			ray0.Direction = normalize(ellep.GetPosition() - ray0.Origin);
			ray1.Direction = normalize(ellep.GetPosition() + ray0.Origin);

			bool test0 = ellep.Intersect(ray0);
			bool test1 = ellep.Intersect(ray1);
			Assert::IsTrue(test0);
			Assert::IsFalse(test1);

			Ray ray2;
			HitData hd0;
			ray2.Origin = vec3(0.f, 0.f, 0.f);
			ray2.Direction = normalize(vec3(1.f, 0.f, 0.f));

			ellep.Intersect(ray2);
			hd0 = ellep.CalculateHitData(ray2);
			vec3 test2 = vec3(1.f, 0.f, 0.f);

			//test distance
			Assert::AreEqual(1.f, ray2.Distance);

			//test position of intersection
			vec3AreEqual(vec3(1.f, 0.f, 0.f), hd0.Position);
	
		}

		//aabb
		TEST_METHOD(aabb)
		{
			AABB aabb = AABB(vec3(-1.f, -1.f, -1.f), vec3(1.f, 1.f, 1.f));
			Ray ray0;
			Ray ray1;
			ray0.Origin = vec3(1.f, 1.f, -4.f);
			ray1.Origin = vec3(0.f, 0.f, 0.f);
			ray0.Direction = normalize(aabb.GetCenter() - ray0.Origin);
			ray1.Direction = vec3(1.f, 0.f, 0.f);

			bool test0 = aabb.Intersect(ray0);
			bool test1 = aabb.Intersect(ray1);

			Assert::IsTrue(test0);
			Assert::IsTrue(test1);
		}
		
	};
	TEST_CLASS(scatter)
	{
	public:
		bool IsInRange(vec3 v, float min, float max)
		{
			
			if (!(min - v.x <= v.x && v.x <= max + v.x))
				return false;
			
			if (!(min - v.y <= v.y && v.y <= max + v.y))
				return false;

			if (!(min - v.z <= v.z && v.z <= max + v.z))
				return false;

			return true;
		}
		void vec3AreEqual(vec3 expected, vec3 result)
		{
			Assert::AreEqual(expected.x, result.x);
			Assert::AreEqual(expected.y, result.y);
			Assert::AreEqual(expected.z, result.z);
		}

		TEST_METHOD(reflection)
		{
			vec3 normal = vec3(0.f, 1.f, 0.f);
			vec3 dir = normalize(vec3(1.f, -1.f, 0.f));
		
			//perfect reflection
			Metallic mat0 = Metallic(vec3(0.5f), 0.f);
			Dielectric mat1 = Dielectric(vec3(1.f), 1.f);
			vec3 result = mat0.Reflect(normal, dir);
			vec3 test1 = vec3(0.707106781f, 0.707106781f, 0.f);
			
			//refraction
			vec3 result1 = vec3(0.f);
			float ni_over_nt = 1.f;
			mat1.Refract(dir, normal, ni_over_nt, result1);
			vec3 test2 = vec3(0.70710678118655f, -0.70710678118655f, 0.f);

			vec3AreEqual(test1, result);
			vec3AreEqual(test2, result1);
			
		}
	};

















}
