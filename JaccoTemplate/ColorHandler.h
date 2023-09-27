#pragma once
#include "vec.h"

namespace Tmpl8
{
	class ColorHandler
	{
	public:
		void Init();
		void ResetFrame();

		void MapAndSampleColor(Surface* screen,MathUtil::vec3& color, MathUtil::vec3& outcolor, int x, int y, float exposure);
	
	
	private:
		Pixel Vec3ToPixel(MathUtil::vec3& color);
	
		//tone mapping functions
		float Luminance(MathUtil::vec3 v);
		MathUtil::vec3 ChangeLuminance(MathUtil::vec3 c_in, float l_out);
		MathUtil::vec3 UnchartedToneMap(MathUtil::vec3 x);

		//for sampling
		MathUtil::vec4* m_Colors = nullptr;
	};

}
