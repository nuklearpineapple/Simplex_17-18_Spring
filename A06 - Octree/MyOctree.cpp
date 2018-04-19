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

	//for (uint i = 0; i < entityMngr->GetEntityCount(); i++) // all entities
	//	v3Center = v3Center + entityMngr->GetEntity()->GetPosition(); // use position of all entities to help find center

	v3Center.x /= entityMngr->GetEntityCount();
	v3Center.y /= entityMngr->GetEntityCount();
	v3Center.z /= entityMngr->GetEntityCount();

	//for(uint i=0; i < entityMngr->GetEntityCount(); i++)
	//{
	//	// get current dimensions via entities position and center of entities
	//	float currentWidth = glm::abs(entityMngr->GetEntity(i)->GetPosition().x - v3Center.x);
	//	float currentHeight = glm::abs(entityMngr->GetEntity(i)->GetPosition().y - v3Center.y);
	//	float currentDepth = glm::abs(entityMngr->GetEntity(i)->GetPosition().z - v3Center.z);

	//	// check if current value is greater than max value
	//	if (currentWidth > maxWidth) maxWidth = currentWidth;
	//	if (currentHeight > maxHeight) maxHeight = currentHeight;
	//	if (currentDepth > maxDepth) maxDepth = currentDepth;
	//}

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

void Simplex::MyOctree::CreateNode(MyEntityManager * entityMngr, std::vector<bool> vList, vector3 center, float width, uint nodeID)
{
	if (entityMngr->GetEntityCount()) // check entities exist
	{
		vector3 *nodeVertices = new vector3[entityMngr->GetEntityCount()];
		uint index = 0;
		/*for (uint i = 0; i < entityMngr->GetEntityCount(); i++)
		{
			if (vList[i]) 
			{
				nodeVertices[index] = entityMngr->GetEntity(i)->GetPosition();
				index++;
			}
		}*/

		// allocate a new node for this octree
		cOctreeNodes[nodeID] = new MyOctree;

		// 
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

