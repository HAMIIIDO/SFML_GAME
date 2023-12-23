#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager(){}

void EntityManager::update() {
	//todo add entities from m_enttoAdd the proper locations/ add to entityvec and antityMap

	for (auto& e : m_entitiesToAdd) {
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);


	}
	
	m_entitiesToAdd.clear();
	//remove dead entitie

	removeDeadEntities(m_entities);

	// remove dead from map
	// c++17 way to iterate through [key,value]pairs in map
	for (auto& pair : m_entityMap) {
		const std::string& tag = pair.first;  
		EntityVec& entityVec = pair.second;    // Access the value (entityVec) of the map

		removeDeadEntities(entityVec);
		
	}

}

void EntityManager::removeDeadEntities(EntityVec& vec) {
	
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[](const auto& e) { return !e->isActive(); }), vec.end());
}
std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag) {
		auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
		m_entitiesToAdd.push_back(entity);


		return entity;
}
const EntityVec& EntityManager::getEntities() {
	return m_entities;
}
const EntityVec& EntityManager::getEntities(const std::string& tag) {
	// todo the correct vector from the map
	return m_entityMap[tag];
}





