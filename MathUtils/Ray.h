#pragma once
#include "vec.h"
namespace MathUtil
{

		class vec3;
		class Material;
		struct HitData
		{
			vec3 Position;
			vec3 Normal;
			Material* material = nullptr;
		};
		struct Ray
		{
		public:
			Ray() { Distance = INFINITY; }
			vec3 Direction = vec3(0.f);
			vec3 Origin = vec3(0.f);
			float Distance;
			float TravelDist = 0;
			int WaveLengthFlag = -1;
		};
}
