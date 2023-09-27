#pragma once
#include "Material.h"
#include "AABB.h"
#include "Primitive.h"
#include "Texture.h"
#include <vector>

namespace Tmpl8
{
	class Object
	{
	public:
		//index used in bvh
		int myIndex = NULL;

		Object() = default;
		Object(MathUtil::Primitive* theShape, MathUtil::Material* theMaterial)
		{
			m_Material = theMaterial;
			m_Shape = theShape;
			m_AABB = m_Shape->BoundingBox();
		}
	
		inline void Delete()
		{
			delete m_Shape;	
		}
		
		inline const MathUtil::Material& GetMaterial() const { return *m_Material; }
		inline MathUtil::Material& GetMaterial() { return *m_Material; }

		inline const MathUtil::AABB& GetAABB() const { return m_AABB; }
		inline const MathUtil::Primitive& GetPrimitive() const { return *m_Shape; }

		inline const bool operator ==(const Object& other) const 
		{ return (m_Material == other.m_Material && m_Shape == other.m_Shape); }


	private:
		MathUtil::AABB m_AABB;
		MathUtil::Material* m_Material;
		MathUtil::Primitive* m_Shape;


	};

}

