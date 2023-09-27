#include "precomp.h"
#include "ColorHandler.h"
using namespace Tmpl8;
#define UNCHARTED_2      0
#define LUMINCANCE_MAP   1
#define JODI_MAP_COMBO   0
#define REINHARD         0
#define EXPONENTIAL_MAP  0



void ColorHandler:: Init()
{
	//vec4 with all pixels and w as frames
	m_Colors = new MathUtil::vec4[ScreenWidth * ScreenHeight];
	for(int i = 0; i < ScreenWidth * ScreenHeight; i++)
	{
		m_Colors[i] = MathUtil::vec4(0.f);
	}
}

void ColorHandler::ResetFrame()
{
	for (int i = 0; i < ScreenWidth * ScreenHeight; i++)
	{
		m_Colors[i] = MathUtil::vec4(0.f);
	}
}

//functions needed for mapping
float ColorHandler::Luminance(MathUtil::vec3 v)
{
	return MathUtil::dot(v, MathUtil::vec3(0.2126f, 0.7152f, 0.0722f));
}
MathUtil::vec3 ColorHandler::ChangeLuminance(MathUtil::vec3 c_in, float l_out)
{
	float l_in = Luminance(c_in);
	return c_in * (l_out / l_in);
}
MathUtil::vec3 ColorHandler::UnchartedToneMap(MathUtil::vec3 x)
{
	float A = 0.15f;
	float B = 0.50f;
	float C = 0.10f;
	float D = 0.20f;
	float E = 0.02f;
	float F = 0.30f;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

Pixel ColorHandler::Vec3ToPixel(MathUtil::vec3& color)
{
	//gamma correction
	float OneOvergamma = 1.f / 1.5f;

	float R = powf(color.r, OneOvergamma);
	float G = powf(color.g, OneOvergamma);
	float B = powf(color.b, OneOvergamma);
	
	//bit shift into integer 
	byte newR = static_cast<byte>(R * 255.f);
	byte newG = static_cast<byte>(G * 255.f);
	byte newB = static_cast<byte>(B * 255.f);
	int newColor = (newR << (8 * 2)) | (newG << (8 * 1)) | (newB << (8 * 0));
	return newColor;
}

void ColorHandler::MapAndSampleColor( Surface* screen, MathUtil::vec3& color, MathUtil::vec3& outcolor, int x, int y, float exposure)
{
	//mapping
#if	UNCHARTED_2

	float exposure_bias = exposure * 3.f;
	MathUtil::vec3 current = UnchartedToneMap(color * exposure_bias);
	MathUtil::vec3 X = MathUtil::vec3(11.2f);
	MathUtil::vec3 whiteScale = MathUtil::vec3(1.0f) / UnchartedToneMap(X);
	outcolor = current * whiteScale;



#elif LUMINCANCE_MAP

	float old = Luminance(color);
	float numerator = old * (1.0f + (old / (exposure * exposure)));
	float New = numerator / (1.0f + old);
	outcolor = ChangeLuminance(color, New);

#elif JODI_MAP_COMBO
	//to prevent warning
	static_cast<void>(exposure);
	float l = Luminance(color);
	MathUtil::vec3 tv = color / (color + 1.f);
	outcolor = MathUtil::Lerp(color / (1.0f + l), tv, tv);



#elif REINHARD
	outcolor = color * ((color / MathUtil::vec3(exposure * exposure)) + 1.f);
	outcolor = outcolor / (color + 1.f);

#elif EXPONENTIAL_MAP
	float OneOverExp = 1.f / exposure;
	outcolor.r = 1.f - expf(-color.r * OneOverExp);
	outcolor.g = 1.f - expf(-color.g * OneOverExp);
	outcolor.b = 1.f - expf(-color.b * OneOverExp);
#else

	static_cast<void>(exposure);
	outcolor = color;
#endif

	//clamp to prevent too bright pixels --fireflies
	outcolor.r = MathUtil::Clamp(outcolor.r, 0.0f, 1.f);
	outcolor.g = MathUtil::Clamp(outcolor.g, 0.0f, 1.f);
	outcolor.b = MathUtil::Clamp(outcolor.b, 0.0f, 1.f);

	//sampling
	m_Colors[x + y * ScreenWidth] += MathUtil::vec4(outcolor.r, outcolor.g, outcolor.b, 1.f);
	outcolor = m_Colors[x + y * ScreenWidth] / m_Colors[x + y * ScreenWidth].w;

	//writing to screen
	screen->Plot(x, y, Vec3ToPixel(outcolor));
}