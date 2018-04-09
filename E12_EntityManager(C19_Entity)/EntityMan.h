#pragma once

#include "Simplex\Simplex.h"
#include "MyEntity.h"
#include <unordered_map>
#include <string>

namespace Simplex
{
	class EntityMan
	{

	public:
		EntityMan();
		~EntityMan();

		/*
		Usage: Add specified Entity to Map
		Arguments:
		-	MyEntity* otherEntity -> The Entity to Add to the Map
		Output: void
		*/
		void AddEntity(MyEntity* thisEntity);
		/*
		Usage: Remove an Entity from Map with unique ID
		Arguments:
		-	String id -> Unique ID of Entity to Remove from Map
		Output: void
		*/
		void RemoveEntity(String id);
		/*
		Usage: Get Entity by unique ID in Map
		Arguments:
		-	String id -> Unique ID of Entity to Return
		Output: Return Entity at specified ID
		*/
		MyEntity* GetEntityByID(String id);
		
	};

}