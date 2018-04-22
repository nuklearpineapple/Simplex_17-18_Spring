#pragma once
/*----------------------------------------------
Programmer: Timothy Ascencio (ta3755@g.rit.edu)
Date: 04/18/2018
----------------------------------------------*/

#include "Definitions.h"

#include "MyEntityManager.h"

namespace Simplex
{
	// amount of nodes created in octree
	uint g_iEndNodeCount;

	// octree subdivision count
	uint g_CurrentSubdivision;
	
	// subdivision max
	uint g_iMaxSubdivision;

	// entity max
	uint g_iMaxEntity;

	// store the octree node ID's , values 0 to 7
	enum eOctreeNodes
	{
		LEFT_TOP_FRONT,     // enum 0, -x +y +z
		LEFT_TOP_BACK,      // enum 1, -x +y -z
		LEFT_BOTTOM_FRONT,  // enum 2, -x -y +z
		LEFT_BOTTOM_BACK,   // enum 3, -x -y -z
		RIGHT_TOP_FRONT,    // enum 4, +x +y +z
		RIGHT_TOP_BACK,     // enum 5, +x +y -z
		RIGHT_BOTTOM_FRONT, // enum 6, +x -y +z
		RIGHT_BOTTOM_BACK,  // enum 7, +x -y -z
	};

	class MyOctree
	{
		public:
			MyOctree();
			~MyOctree();
			void CreateOctreeDimensions(MyEntityManager* entityMngr);
			void CreateNewNode(MyEntityManager* entityMngr, std::vector<bool> vList, vector3 center, float width, uint nodeID);
			void CreateNode(MyEntityManager* entityMngr, vector3* nodeVertices, vector3 center, float width);
			void DrawOctree(MyOctree* node, MyEntityManager* entityMngr);
			vector3 GetNewNodeCenter(vector3 center, float width, uint nodeID);

		private:
			void InitOctree(); // initalize data members

			// eight nodes branch
			MyOctree* cOctreeNodes[8];

			// center point of node
			vector3 v3Center;

			// size of cube for current node
			float fWidth;

			// store amount of entities in current node
			uint iEntityCount;

			// if we have divided this node
			bool bIsSubdivided;
	};
}
