#include "precomp.h"

#include "BVH.h"
using namespace Tmpl8;

void BVH::Generate(std::vector<Object*> Objects)
{
	if (Objects.size() <= 0)
		return;

	if (Objects.size() > 1)
	{
		m_RootNode = new BranchNode();
	}
	else m_RootNode = new LeafNode();

	m_RootNode->GenerateAABB(Objects);
	
}
void BranchNode :: GenerateAABB(std::vector<Object*> in_Objects)
{
	MathUtil::vec3 min = MathUtil::vec3(INFINITY);
	MathUtil::vec3 max = MathUtil::vec3(-INFINITY);

	for (auto& object : in_Objects)
	{
		if (object->GetAABB().GetMin().x < min.x) min.x = object->GetAABB().GetMin().x;
		if (object->GetAABB().GetMin().y < min.y) min.y = object->GetAABB().GetMin().y;
		if (object->GetAABB().GetMin().z < min.z) min.z = object->GetAABB().GetMin().z;

		if (object->GetAABB().GetMax().x > max.x) max.x = object->GetAABB().GetMax().x;
		if (object->GetAABB().GetMax().y > max.y) max.y = object->GetAABB().GetMax().y;
		if (object->GetAABB().GetMax().z > max.z) max.z = object->GetAABB().GetMax().z;
	}
	m_aabb = MathUtil::AABB(min, max);

	float xW = m_aabb.GetMax().x - m_aabb.GetMin().x;
	float yW = m_aabb.GetMax().y - m_aabb.GetMin().y;
	float zW = m_aabb.GetMax().z - m_aabb.GetMin().z;

	if (xW >= yW && xW >= zW)
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().x < b->GetAABB().GetCenter().x; });


	}
	else if (yW >= zW)
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().y < b->GetAABB().GetCenter().y; });

	}
	else
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().z < b->GetAABB().GetCenter().z; });

	}

	size_t centerIndex = in_Objects.size() / 2;
	std::vector<Object*> object1;
	std::vector<Object*> object2;
	for (size_t i = 0; i < centerIndex; i++)
	{
		object1.push_back(in_Objects[i]);
	}

	for (size_t i = centerIndex; i < in_Objects.size(); i++)
	{
		object2.push_back(in_Objects[i]);
	}

	if (object1.size() == 1) N1 = new LeafNode();
	else N1 = new BranchNode();

	if (object2.size() == 1)N2 = new LeafNode();
	else N2 = new BranchNode();

	N1->GenerateAABB(object1);
	N2->GenerateAABB(object2);

};

void bvh::CreateNode(std::vector<Object*> in_Objects, int index)
{
	if (in_Objects.size() < 1)
		return;

	MathUtil::vec3 min = MathUtil::vec3(INFINITY);
	MathUtil::vec3 max = MathUtil::vec3(-INFINITY);

	for (auto& object : in_Objects)
	{
		if (object->GetAABB().GetMin().x < min.x) min.x = object->GetAABB().GetMin().x;
		if (object->GetAABB().GetMin().y < min.y) min.y = object->GetAABB().GetMin().y;
		if (object->GetAABB().GetMin().z < min.z) min.z = object->GetAABB().GetMin().z;

		if (object->GetAABB().GetMax().x > max.x) max.x = object->GetAABB().GetMax().x;
		if (object->GetAABB().GetMax().y > max.y) max.y = object->GetAABB().GetMax().y;
		if (object->GetAABB().GetMax().z > max.z) max.z = object->GetAABB().GetMax().z;
	}
	m_Nodes[index].Bounds = MathUtil::AABB(min, max);

	float xW = m_Nodes[index].Bounds.GetMax().x - m_Nodes[index].Bounds.GetMin().x;
	float yW = m_Nodes[index].Bounds.GetMax().y - m_Nodes[index].Bounds.GetMin().y;
	float zW = m_Nodes[index].Bounds.GetMax().z - m_Nodes[index].Bounds.GetMin().z;

	if (xW >= yW && xW >= zW)
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().x < b->GetAABB().GetCenter().x; });

	}
	else if (yW >= zW)
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().y < b->GetAABB().GetCenter().y; });

	}
	else
	{
		std::sort(in_Objects.begin(), in_Objects.end(), [](Object* a, Object* b) {return a->GetAABB().GetCenter().z < b->GetAABB().GetCenter().z; });

	}

	if (in_Objects.size() < 2)
	{
		m_Nodes[index].Type = 1;
		m_Nodes[index].First = in_Objects[0]->myIndex;
	}
	else
	{
		size_t centerIndex = in_Objects.size() / 2;

		std::vector<Object*> object1;
		std::vector<Object*> object2;

		for (size_t i = 0; i < centerIndex; i++)
		{
			object1.push_back(in_Objects[i]);
		}

		for (size_t i = centerIndex; i < in_Objects.size(); i++)
		{
			object2.push_back(in_Objects[i]);
		}

		int leftIndex = static_cast<int>(m_Nodes.size());
		m_Nodes.push_back(BVHNode());

		int rightIndex = static_cast<int>(m_Nodes.size());
		m_Nodes.push_back(BVHNode());

		CreateNode(object1, leftIndex);
		CreateNode(object2,rightIndex);

		m_Nodes[index].Type = 0;
		m_Nodes[index].First = leftIndex;
	}
}
