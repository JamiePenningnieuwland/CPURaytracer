#pragma once
#include "Primitive.h"
#include "Object.h"
#include "vec.h"
#include "AABB.h"
#include <vector>
#include <algorithm>
namespace Tmpl8
{
	//first implementation
	struct Node
	{
		Node() {};
		~Node() {};
		virtual bool Intersect(const MathUtil::Ray& ray, std::vector<Object*>& HittableObjects) = 0;
		virtual void GenerateAABB(std::vector<Object*> in_Objects) = 0;
	protected:
		MathUtil::AABB m_aabb;
	};
	struct LeafNode : public Node
	{
		virtual bool Intersect(const MathUtil::Ray& ray, std::vector<Object*>& HittableObjects)
		{
			if(m_aabb.Intersect(ray))
			{
				HittableObjects.push_back(object);
				return true;
			}
			return false;
		}
		virtual void GenerateAABB(std::vector<Object*> in_Objects)
		{
			object = in_Objects[0];
			m_aabb = object->GetAABB();
		}
	private:
		Object* object;
	};
	struct BranchNode : public Node
	{
		virtual bool Intersect(const MathUtil::Ray& ray, std::vector<Object*>& HittableObjects)
		{
			if(m_aabb.Intersect(ray))
			{
				bool a = N1->Intersect(ray, HittableObjects);
				bool b = N2->Intersect(ray, HittableObjects);
				return a || b;
			}
			return false;
		}
		virtual void GenerateAABB(std::vector<Object*> in_Objects);
		
	private:
		Node* N1 = nullptr;
		Node* N2 = nullptr;
	};
	class BVH
	{
	public:
	
		void Generate(std::vector<Object*> Objects);
		bool Intersect(const MathUtil::Ray& ray, std::vector<Object*>& HittableObjects)
		{
			if (m_RootNode == nullptr) return false;

			return m_RootNode->Intersect(ray, HittableObjects);
		}

	private:
		Node* m_RootNode;
		
	};

	//latest implementation
	struct BVHNode
	{
		MathUtil::AABB Bounds = MathUtil::AABB(-INFINITY, INFINITY);
		uint32_t First = 0;
		uint32_t Type = 0;
	};
	class bvh
	{
	public:
		void Generate(std::vector<Object*> Objects)
		{
			m_Nodes.push_back(BVHNode());
			CreateNode(Objects, 0);
		}
		
		bool Intersect(const MathUtil::Ray& ray, std::vector<int>& HittableObjects, int indexNode)
		{
			if (m_Nodes[indexNode].Type == 1)
			{
				if (m_Nodes[indexNode].Bounds.Intersect(ray))
				{
					HittableObjects.push_back(m_Nodes[indexNode].First);
					return true;
				}

				return false;
			}
			else
			{
				if (m_Nodes[indexNode].Bounds.Intersect(ray))
				{
					bool a = Intersect(ray, HittableObjects, m_Nodes[indexNode].First);
					bool b = Intersect(ray, HittableObjects, m_Nodes[indexNode].First + 1);
					return a || b;
				}
				return false;

			}
		}
	
	private:
		std::vector<BVHNode> m_Nodes;
		void CreateNode(std::vector<Object*> Objects, int index);
		
	};
}

