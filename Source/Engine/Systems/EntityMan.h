#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "Camera.h"

#include "Entity.h"



class Map;

class EntityMan {
private:
	Map* map = nullptr;

	std::vector<Entity*> entsToRemove{};
public:
	inline static std::unordered_map<std::string, JsonEntity> entityTemplates{};
	
	std::vector<std::shared_ptr<Entity>> GlobalEntities;



	EntityMan(Map* map);

	Entity* CreateEntity(const std::string& key, Point pos);

	void DeleteEntity(Entity* ent);

	void QueuedRemoval();

	void RegionUpdate();

	void LogicUpdate() {
		QueuedRemoval();
		RegionUpdate();

		for (auto &ent : GlobalEntities) {
			ent->Update();
		}
	}

};

