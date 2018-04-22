#include "MyOctree.h"

#include "AppClass.h"
using namespace Simplex;

MyOctree::MyOctree()
{
}


MyOctree::~MyOctree()
{
}

void Simplex::MyOctree::InitOctree()
{
	// initialize variables: zero , false, allocate memory appropriately, etc.
	memset(cOctreeNodes[8], 0, sizeof(cOctreeNodes));
	v3Center = ZERO_V3;
	fWidth = 0.0f;
	iEntityCount = 0;
	bIsSubdivided = false; Octant octant;
}
void Simplex::MyOctree::CreateOctreeDimensions(MyEntityManager* entityMngr)
{
	float maxWidth, maxHeight, maxDepth;
	maxWidth = maxHeight = maxDepth = 0;

	if (entityMngr->GetEntityCount() <= 0) // no entities = no octree
		return;

	for (uint i = 0; i < entityMngr->GetEntityCount(); i++) // all entities
		v3Center = v3Center + entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal(); // use position of all entities to help find center

	v3Center.x /= entityMngr->GetEntityCount();
	v3Center.y /= entityMngr->GetEntityCount();
	v3Center.z /= entityMngr->GetEntityCount();

	for(uint i=0; i < entityMngr->GetEntityCount(); i++)
	{
		// specific rigidbody center
		float xRigidBody = entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().x;
		float yRigidBody = entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().y;
		float zRigidBody = entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().z;
		// get current dimensions via entities position and center of entities
		float currentWidth = glm::abs(xRigidBody - v3Center.x);
		float currentHeight = glm::abs(yRigidBody - v3Center.y);
		float currentDepth = glm::abs(zRigidBody - v3Center.z);

		// check if current value is greater than max value
		if (currentWidth > maxWidth) maxWidth = currentWidth;
		if (currentHeight > maxHeight) maxHeight = currentHeight;
		if (currentDepth > maxDepth) maxDepth = currentDepth;
	}

	// recalculate dimensions due to calculating from center
	maxWidth *= 2; 
	maxHeight *= 2; 
	maxDepth *= 2;

	// assign whatever value is the highest to cube dimensions
	if (maxWidth > maxHeight && maxWidth > maxDepth)
		fWidth = maxWidth;
	else if (maxHeight > maxWidth && maxHeight > maxDepth)
		fWidth = maxHeight;
	else
		fWidth = maxDepth;
}

void Simplex::MyOctree::CreateNewNode(MyEntityManager * entityMngr, std::vector<bool> vList, vector3 center, float width, uint nodeID)
{
	if (entityMngr->GetEntityCount()) // check entities exist
	{
		vector3 *nodeVertices = new vector3[entityMngr->GetEntityCount()];
		uint index = 0;
		for (uint i = 0; i < entityMngr->GetEntityCount(); i++)
		{
			if (vList[i]) 
			{
				nodeVertices[index] = entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal();
				index++;
			}
		}

		// allocate a new node for this octree
		cOctreeNodes[nodeID] = new MyOctree;

		// get new node center based on node Index
		vector3 vNodeCenter = GetNewNodeCenter(center, width, nodeID);

		g_CurrentSubdivision++;

		// subdivide if necessary through recursion
		cOctreeNodes[nodeID]->CreateNode(entityMngr, nodeVertices, vNodeCenter, width / 2);

		g_CurrentSubdivision--;

		delete[] nodeVertices;
	}
}

void Simplex::MyOctree::CreateNode(MyEntityManager * entityMngr, vector3 * nodeVertices, vector3 center, float width)
{
	v3Center = center;
	fWidth = width;
	uint entityCount = entityMngr->GetEntityCount();

	if ((entityMngr->GetEntityCount() > g_iMaxEntity) && (g_CurrentSubdivision < g_iMaxSubdivision))
	{
		bIsSubdivided = true;

		std::vector<bool> list1(entityCount); // LEFT_TOP_FRONT
		std::vector<bool> list2(entityCount); // LEFT_TOP_BACK
		std::vector<bool> list3(entityCount); // LEFT_BOTTOM_FRONT
		std::vector<bool> list4(entityCount); // LEFT_BOTTOM_BACK
		std::vector<bool> list5(entityCount); // RIGHT_TOP_FRONT
		std::vector<bool> list6(entityCount); // RIGHT_TOP_BACK
		std::vector<bool> list7(entityCount); // RIGHT_BOTTOM_FRONT
		std::vector<bool> list8(entityCount); // RIGHT_BOTTOM_BACK

		for (uint i = 0; i < entityCount; i++) 
		{
			vector3 entityCenter = entityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal();

			if (entityCenter.x <= center.x && entityCenter.y >= center.y && entityCenter.z >= center.z)
				list1[i] = true;
			if (entityCenter.x <= center.x && entityCenter.y >= center.y && entityCenter.z <= center.z)
				list2[i] = true;
			if (entityCenter.x <= center.x && entityCenter.y <= center.y && entityCenter.z >= center.z)
				list3[i] = true;
			if (entityCenter.x <= center.x && entityCenter.y <= center.y && entityCenter.z <= center.z)
				list4[i] = true;
			if (entityCenter.x >= center.x && entityCenter.y >= center.y && entityCenter.z >= center.z)
				list5[i] = true;
			if (entityCenter.x >= center.x && entityCenter.y >= center.y && entityCenter.z <= center.z)
				list6[i] = true;
			if (entityCenter.x >= center.x && entityCenter.y <= center.y && entityCenter.z >= center.z)
				list7[i] = true;
			if (entityCenter.x >= center.x && entityCenter.y <= center.y && entityCenter.z <= center.z)
				list8[i] = true;
		}

		uint count1, count2, count3, count4, count5, count6, count7, count8;
		count1 = count2 = count3 = count4 = count5 = count6 = count7 = count8 = 0;

		for (uint i = 0; i < entityCount; i++)
		{
			if (list1[i]) count1++;
			if (list2[i]) count2++;
			if (list3[i]) count3++;
			if (list4[i]) count4++;
			if (list5[i]) count5++;
			if (list6[i]) count6++;
			if (list7[i]) count7++;
			if (list8[i]) count8++;
		}

		CreateNewNode(entityMngr, list1, center, width, LEFT_TOP_FRONT);
		CreateNewNode(entityMngr, list2, center, width, LEFT_TOP_BACK);
		CreateNewNode(entityMngr, list3, center, width, LEFT_BOTTOM_FRONT);
		CreateNewNode(entityMngr, list4, center, width, LEFT_BOTTOM_BACK);
		CreateNewNode(entityMngr, list5, center, width, RIGHT_TOP_FRONT);
		CreateNewNode(entityMngr, list6, center, width, RIGHT_TOP_BACK);
		CreateNewNode(entityMngr, list7, center, width, RIGHT_BOTTOM_FRONT);
		CreateNewNode(entityMngr, list8, center, width, RIGHT_BOTTOM_BACK);

	}
	else
	{
		bIsSubdivided = false;
		g_iEndNodeCount++;
	}
}

void Simplex::MyOctree::DrawOctree(MyOctree * node, MyEntityManager* entityMngr)
{
	// valid node check
	if (!node) return;

	if (bIsSubdivided) 
	{
		DrawOctree(node->cOctreeNodes[LEFT_TOP_FRONT], entityMngr);
		DrawOctree(node->cOctreeNodes[LEFT_TOP_BACK], entityMngr);
		DrawOctree(node->cOctreeNodes[LEFT_BOTTOM_FRONT], entityMngr);
		DrawOctree(node->cOctreeNodes[LEFT_BOTTOM_BACK], entityMngr);
		DrawOctree(node->cOctreeNodes[RIGHT_TOP_FRONT], entityMngr);
		DrawOctree(node->cOctreeNodes[RIGHT_TOP_BACK], entityMngr);
		DrawOctree(node->cOctreeNodes[RIGHT_BOTTOM_FRONT], entityMngr);
		DrawOctree(node->cOctreeNodes[RIGHT_BOTTOM_BACK], entityMngr);
	}
}

vector3 Simplex::MyOctree::GetNewNodeCenter(vector3 center, float width, uint nodeID)
{
	vector3 v3NodeCenter(0,0,0);

	// based on node we are currently in, switch case to set appropriate v3 coordinates
	switch (nodeID) 
	{
	case LEFT_TOP_FRONT:
		v3NodeCenter = vector3(center.x - width / 4, center.y + width / 4, center.z + width / 4);
		break;
	case LEFT_TOP_BACK:
		v3NodeCenter = vector3(center.x - width / 4, center.y + width / 4, center.z - width / 4);
		break;
	case LEFT_BOTTOM_FRONT:
		v3NodeCenter = vector3(center.x - width / 4, center.y - width / 4, center.z + width / 4);
		break;
	case LEFT_BOTTOM_BACK:
		v3NodeCenter = vector3(center.x - width / 4, center.y - width / 4, center.z - width / 4);
		break;
	case RIGHT_TOP_FRONT:
		v3NodeCenter = vector3(center.x + width / 4, center.y + width / 4, center.z + width / 4);
		break;
	case RIGHT_TOP_BACK:
		v3NodeCenter = vector3(center.x + width / 4, center.y + width / 4, center.z - width / 4);
		break;
	case RIGHT_BOTTOM_FRONT:
		v3NodeCenter = vector3(center.x + width / 4, center.y - width / 4, center.z + width / 4);
		break;
	case RIGHT_BOTTOM_BACK:
		v3NodeCenter = vector3(center.x + width / 4, center.y - width / 4, center.z - width / 4);
		break;
	}

	return v3NodeCenter;
}

