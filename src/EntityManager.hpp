#pragma once

#include "Entity.hpp"

#include <map>
#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
    EntityVec                        m_entities;          // all entities
    EntityVec                        m_entitiesToAdd;     // entities to add next update
    std::map<std::string, EntityVec> m_entityMap;         // map from entity tag to vectors
    size_t                           m_totalEntities = 0; // total entities created

    void removeDeadEntities(EntityVec &vec);

  public:
    EntityManager() = default;

    void update();

    std::shared_ptr<Entity> addEntity(const std::string &tag);

    const EntityVec &getEntities();
    const EntityVec &getEntities(const std::string &tag);
};