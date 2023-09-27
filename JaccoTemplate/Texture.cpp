#include "precomp.h"

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Tmpl8;

Texture::Texture(const std::string& filePath) 
{
	m_data = stbi_loadf(filePath.c_str(), &m_Width, &m_Heigth, &numComponent, 3);
};
Texture::~Texture() 
{
	delete[] m_data;
};
MathUtil::vec3 Texture::SampleTexture(MathUtil::vec2 uv) const
{
	if (m_data == nullptr)
		return MathUtil::vec3(0.f);

	int x, y;

	x = static_cast<int>(uv.x * static_cast<float>(m_Width));
	y = static_cast<int>(uv.y * static_cast<float>(m_Heigth));

	// do not write out of bounds
	if (x < 0.f || y < 0.f || x >= m_Width || y >= m_Heigth)
		return MathUtil::vec3(0.f);

	//times 3 because rgb 
	int index = (x + y * m_Width) * 3;

	MathUtil::vec3 returnVec;

	//4 bytes in a float and per pixel 3 floats - total of 4 * 3 = 12 bytes
	memcpy(&returnVec[0], &m_data[index], 12);

	return returnVec;
};
int Texture::GetWidth()const 
{
	return m_Width;
};
int Texture::GetHeight() const 
{
	return m_Heigth;
};
MathUtil::mat4 Textured::CreateLocalMatrix(const MathUtil::vec3& forward, const MathUtil::vec3& Up, const MathUtil::vec3& right) const
{
	MathUtil::mat4 temp;
	temp[0 ] = right.x; temp[1 ] = Up.x; temp[2 ] = forward.x; temp[3 ] = 0.;
	temp[4 ] = right.y; temp[5 ] = Up.y; temp[6 ] = forward.y; temp[7 ] = 0.;
	temp[8 ] = right.z; temp[9 ] = Up.z; temp[10] = forward.z; temp[11] = 0.;
	temp[12] = 0.; temp[13] = 0.; temp[14] = 0.; temp[15] = 1.;
	return temp;
}

bool Tmpl8::Textured::ScatteredRay(const MathUtil::Ray& ray_in, const MathUtil::HitData& hd, MathUtil::vec3& color, MathUtil::Ray& scattered_ray) const
{

	MathUtil::vec2 uv = getUV(-hd.Normal) * m_tiling;
	uv.x = uv.x - floorf(uv.x);
	uv.y = uv.y - floorf(uv.y);

	int x, y;
	x = static_cast<int>(uv.x * static_cast<float>(m_DiffuseTexture->GetWidth()));
	y = static_cast<int>(uv.y * static_cast<float>(m_DiffuseTexture->GetHeight()));
	x = MathUtil::Clamp(x, 0, m_DiffuseTexture->GetWidth()-1);
	y = MathUtil::Clamp(y, 0, m_DiffuseTexture->GetHeight()-1);

	MathUtil::vec3 normal = hd.Normal;
	if (m_NormalTexture != nullptr)
	{
		//sample from sprite
		MathUtil::vec3 samplednormal;
		samplednormal = IntToVec3(m_NormalTexture->GetBuffer()[x + y * m_NormalTexture->GetWidth()]) * 2.f -1.f;

		//possible scaling
		samplednormal *= MathUtil::vec3(2.f, 2.f, 1.f);

		//transform to normal space
		MathUtil::vec3 localRight = hd.Normal.cross(MathUtil::vec3(0.f, 1.f, 0.f));
		MathUtil::vec3 localUp = localRight.cross(hd.Normal);
		MathUtil::mat4 localMat = CreateLocalMatrix(hd.Normal, localUp, localRight);
		normal = samplednormal * localMat;

		normal.normalize();
	}
	MathUtil::eMaterial type = myType;
	MathUtil::vec3 sampleMetal = MathUtil::vec3(0.f);
	if(m_MetalTexture != nullptr)
	{
		sampleMetal = IntToVec3(m_MetalTexture->GetBuffer()[x + y * m_MetalTexture->GetWidth()]);
		if (sampleMetal.x > MathUtil::Rand(1.f))
		{
			type = MathUtil::eMaterial::metallic;
		}
		else type = MathUtil::eMaterial::diffuse;
	}
	switch (type)
	{
	case MathUtil::eMaterial::metallic:
	{

		float DotIN = dot(ray_in.Direction, normal);
		scattered_ray.Direction = ray_in.Direction - 2.f * DotIN * normal;
		scattered_ray.Direction += MathUtil::RandWithinUnitSphere() * sampleMetal;
		scattered_ray.Direction.normalize();
		scattered_ray.Origin = hd.Position + scattered_ray.Direction  * 0.01f;
		break;
	}
	case MathUtil::eMaterial::diffuse:
	{
		MathUtil::vec3 target = normal + normalize(MathUtil::RandWithinUnitSphere());
		scattered_ray.Direction = normalize(target);
		scattered_ray.Origin = hd.Position + scattered_ray.Direction * 0.01f;
		break;
	}

	}

	Pixel pixelColor = m_DiffuseTexture->GetBuffer()[x + y * m_DiffuseTexture->GetWidth()];
	color =  IntToVec3(pixelColor);
		
	return true;
}
