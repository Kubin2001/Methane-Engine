#include "EntityMan.h"

#include "Map.h"

EntityMan::EntityMan(Map* map) {
	this->map = map;
}

Entity* EntityMan::CreateEntity(const std::string& key, Point pos) {
	MapPos mp(pos);

	if (!mp.CorrectnessAbsTile()) {
		return nullptr;
	}

	auto jEntIter = entityTemplates.find(key);
	if (jEntIter == entityTemplates.end()) {
		return nullptr;
	}
	JsonEntity& jEnt = jEntIter->second;
	GlobalEntities.emplace_back(std::make_shared<Entity>(map, MT::Rect{ pos.x, pos.y, jEnt.size.x, jEnt.size.y }, jEnt.tex, 1.0f, 0.95f, jEnt.behaviour));
	Entity* newEntity = GlobalEntities.back().get();
	*newEntity = jEntIter->second;
	newEntity->mc.collider.x = pos.x;
	newEntity->mc.collider.y = pos.y;



	Region& reg = map->GetRegions()[mp.rows][mp.column];
	reg.GetEntities().emplace_back(GlobalEntities.back());
	newEntity->region = Point{ mp.rows, mp.column };

	return newEntity;
}

void EntityMan::DeleteEntity(Entity* ent) {
	entsToRemove.emplace_back(ent);
}

void EntityMan::QueuedRemoval() {
	if (entsToRemove.empty()) { return; }

	for (const auto& entPtr : entsToRemove) {
		auto removeHelper = [entPtr](std::vector<std::shared_ptr<Entity>>& globalVec, std::vector<std::shared_ptr<Entity>>& localVec) {
			auto localIter = std::find_if(localVec.begin(), localVec.end(), [entPtr](auto& regPawn) {return entPtr == regPawn.get(); });
			auto globalIter = std::find_if(globalVec.begin(), globalVec.end(), [entPtr](auto& regPawn) {return entPtr == regPawn.get(); });
			if (localIter != localVec.end()) {
				localVec.erase(localIter);
			}
			if (globalIter != globalVec.end()) {
				std::println("Entity Killed");
				globalVec.erase(globalIter);
			}
		};
		removeHelper(GlobalEntities, map->GetRegions()[entPtr->region.x][entPtr->region.y].GetEntities());
	}

	entsToRemove.clear();
}

void EntityMan::RegionUpdate() {
	for (auto& ent : GlobalEntities) {
		MapPos mp;
		Point center = ent->mc.rect.GetCenter();
		mp.CalcRegion(center.x, center.y);
		if (!mp.CorrectnessRegionS()) { continue; }

		if (mp.rows == ent->region.x && mp.column == ent->region.y) { continue; }

		Point oldRegion = ent->region;

		auto& oldRegEnt = map->GetRegions()[oldRegion.x][oldRegion.y].GetEntities();
		auto regIter = std::find_if(oldRegEnt.begin(), oldRegEnt.end(), [ent](const std::shared_ptr<Entity> &localEnt) {
			return localEnt.get() == ent.get();
		});
		
		if (regIter == oldRegEnt.end()) { // Remove when tested it should not be possible 
			std::println("Impossible error in EntMan Region Update");
			std::terminate();
		}

		auto &newEntVec = map->GetRegions()[mp.rows][mp.column].GetEntities();
		newEntVec.emplace_back(std::move(*regIter));
		oldRegEnt.erase(regIter);
		ent->region = Point{ mp.rows, mp.column };

	}
}