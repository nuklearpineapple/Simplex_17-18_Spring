#include "EntityManager.h"
using namespace Simplex;

typedef std::unordered_map<String, Entity*> EntityMap;

EntityMap entityMap;

Simplex::EntityManager::EntityManager()
{
}

Simplex::EntityManager::~EntityManager()
{
}

// Attempt to make class to store entitys into map. Irritable errors prevent functionality
//void Simplex::EntityManager::AddEntity(MyEntity* otherEntity)
//{
//	entityMap.insert(std::pair<String, Entity*>(otherEntity->GetUniqueID(), otherEntity->GetEntity));
//}

void Simplex::EntityManager::RemoveEntity(String id)
{
	Entity* entity = entity->GetEntity(id);
	//remove entity from entityMap
}

//
//MyEntity * Simplex::EntityManager::GetEntityByID(String id)
//{
//	return entityMap.at(id);
//}

