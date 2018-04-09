#include "EntityMan.h"
using namespace Simplex;

typedef std::unordered_map<String, Entity*> EntityMap;

EntityMap entityMap;

Simplex::EntityMan::EntityMan()
{
}

Simplex::EntityMan::~EntityMan()
{
}

// Attempt to make class to store entitys into map. Irritable errors prevent functionality
// ERROR: "declaration is incompatible with "void Simplex::EntityManager::AddEntity(Simplex::String a_sFileName, Simplex::String a_sUniqueID = "NA")"
//void Simplex::EntityManager::AddEntity(MyEntity* thisEntity)
//{
//	entityMap.insert(std::pair<String, Entity*>(thisEntity->GetUniqueID(), thisEntity->GetEntity));
//}

// Attempt to remove entity by id. Functional for some reason
void Simplex::EntityManager::RemoveEntity(String id)
{
	entityMap.erase(id);
}

// Attempt to make method to return entity in question. Irritable errors prevent functionality
// ERROR: "class "Simplex::EntityManager" has no member "GetEntityByID" "
//MyEntity * Simplex::EntityManager::GetEntityByID(String id)
//{
//	return entityMap.at(id);
//}