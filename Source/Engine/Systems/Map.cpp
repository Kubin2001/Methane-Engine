#include "Map.h"

#include <fstream>

#include "TextureManager.h"
#include "json.hpp"

#include "Camera.h"

//////Region




void Region::Innit(size_t tileSize, size_t tilesPerRegion) {
	TilesOne.reserve(tilesPerRegion);
	TilesTwo.reserve(tilesPerRegion);

	int startX = GetRectangle().x;
	int startY = GetRectangle().y;

	for (size_t i = 0; i < tilesPerRegion; i++){
		TilesOne.emplace_back();
		TilesOne[i].reserve(tilesPerRegion);
		TilesTwo.emplace_back();
		TilesTwo[i].reserve(tilesPerRegion);
		for (size_t j = 0; j < tilesPerRegion; j++){
			TilesOne[i].emplace_back();
			TilesOne[i][j].rect.Set(startX, startY, tileSize, tileSize);

			TilesTwo[i].emplace_back();
			TilesTwo[i][j].rect = TilesOne[i][j].rect;

			Map::SetTile("Core_Grass", TilesOne[i][j],true);
			Map::SetTile("Core_Air", TilesTwo[i][j]);
			startX += tileSize;
		}
		startX = GetRectangle().x;
		startY += tileSize;
	}
	texture = TexMan::GetTex("region");
}



void Region::RenderTiles(MT::Renderer* renderer,Camera *cam) {
	MT::Rect tempRect{ 0,0,0,0 };
	for (auto& tileRow : GetTilesOne()) {
		for (auto& tile : tileRow) {
			tempRect = cam->Transform(tile.rect);
			renderer->FLatRenderCopy(tempRect, tile.tex);
		}
	}
	renderer->FLatRenderCopyPresent();

	for (auto& tileRow : GetTilesTwo()) {
		for (auto& tile : tileRow) {
			tempRect = cam->Transform(tile.rect);
			renderer->FLatRenderCopy(tempRect, tile.tex);
		}
	}
	renderer->FLatRenderCopyPresent();
}

void Region::RenderEntities(MT::Renderer* renderer, Camera* cam) {
	MT::Rect tempRect{ 0,0,0,0 };
	for (auto& entity : localEntities) {
		tempRect = cam->Transform(entity->mc.rect);
		renderer->RenderCopy(tempRect, entity->tex);
	}
}


///////////Map

Map::Map() {

}

Map::Map(MT::Renderer* ren, Camera* camera) {
	this->ren = ren;
	this->camera = camera;

	baseTile.key = "Core_Null";
	baseTile.passable = false;
	baseTile.type =  TileType::NullBlock;
	baseTile.tex = TexMan::GetDefaultTex();
}

std::vector<std::vector<Region>>& Map::GetRegions() {
	return RegionMap;
}

std::vector<Region*>& Map::GetFlatRegions() {
	return FlatRegions;
}

void Map::Init(size_t tileSize, size_t tilesPerRegion, size_t regionMapWidth, size_t regionMapHeight) {
	shadowTexture = TexMan::GetTex("shadow");

	RegionMap.reserve(regionMapWidth);

	int startX = 0;
	int startY = 0;
	size_t regionSize = tileSize * tilesPerRegion;

	for (size_t i = 0; i < regionMapHeight; i++)
	{
		RegionMap.emplace_back();
		RegionMap[i].reserve(regionMapWidth);
		for (size_t j = 0; j < regionMapWidth; j++)
		{
			RegionMap[i].emplace_back();
			RegionMap[i][j].GetRectangle().Set(startX, startY, regionSize,regionSize);
			RegionMap[i][j].Innit(tileSize,tilesPerRegion);
			startX += regionSize;
		}
		startX = 0;
		startY += regionSize;
	}

	MapPos::FedData(RegionMap[0][0].GetRectangle().x, RegionMap[0][0].GetRectangle().y, tileSize, tilesPerRegion,
		RegionMap[0].size(), RegionMap.size());


	for (auto& regionRow : RegionMap) {
		for (auto& region : regionRow) {
			FlatRegions.emplace_back(&region);
		}
	}
}



void Map::SetTile(const std::string& key, Tile& tile, bool variantTextures, bool maxGrowth) {
	auto tileIter = tilesTypes.find(key);
	Tile* tempTile = nullptr;
	if (tileIter != tilesTypes.end()) {
		tempTile = &tileIter->second;
		Tile& temptile = tileIter->second;
	}
	else{
		tempTile = &baseTile;
	}
	tile.key = tempTile->key;
	tile.passable = tempTile->passable;
	tile.rect.w = tempTile->rect.w;
	tile.rect.h = tempTile->rect.h;
	tile.tex = tempTile->tex;
	tile.type = tempTile->type;
}

void Map::Input(SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
		Point mouse = camera->GetMouseUntransform();
		Tile* tile = GetTile<TileLayer::One>(MapPos{ mouse.x, mouse.y});
		if (tile) {
			SetTile("Core_Stone", *tile);
		}
	}
}


void Map::Render() {
	int minRow = 0; // Ile ma pomijaæ wierszy od góry nie trzeba tego skalowaæ z zoomem
	minRow = (camera->rectangle.y - MapPos::minY) / MapPos::regionSize;

	if (minRow < 0) { minRow = 0; } // Ogranicznia jak kamera jest za daleko
	else if (minRow > RegionMap.size() - 1) { minRow = RegionMap.size(); } // czyli wogóle nic siê nie renderuje bo pocz¹tek pêtli jest wiêkszy od koñca


	int maxRow = 0; // Ile ma pomijaæ wierszy od do³u to tak btw liczba ujemna
	maxRow = ((camera->rectangle.y + camera->GetScaledHeight()) - MapPos::maxY) / MapPos::regionSize;

	if (maxRow > 0) { maxRow = 0; } 
	else if (std::abs(maxRow) > RegionMap.size()) { maxRow = RegionMap.size() - (RegionMap.size() *2); }


	int minCol = 0; // Ile ma pomijaæ kolumn z lewej
	minCol = (camera->rectangle.x - MapPos::minX) / MapPos::regionSize;


	if (minCol < 0) { minCol = 0; } // Ogranicznia jak kamera jest za daleko
	else if (minCol > RegionMap.size() - 1) { minCol = RegionMap.size(); } // czyli wogóle nic siê nie renderuje bo pocz¹tek pêtli jest wiêkszy od koñca


	int maxCol = 0; // Ile ma kolumn od prawej to tak btw liczba ujemna
	maxCol = ((camera->rectangle.x + camera->GetScaledWidth()) - MapPos::maxX) / MapPos::regionSize;

	if (maxCol > 0) { maxCol = 0; }
	else if (std::abs(maxCol) > RegionMap[0].size()) { maxCol = RegionMap[0].size() - (RegionMap[0].size() * 2); }


	int renderedRegions = 0;
	MT::Rect tempRect;

	for (int i = minRow; i < RegionMap.size() + maxRow; ++i) { // eksperymentalnie renderuje od prawej do lewej moga byæ z tym problemy
		for (int j = (RegionMap[i].size() -1) + maxCol; j >= minCol; --j) {
			RegionMap[i][j].RenderTiles(ren, camera);
			renderedRegions++;
		}
	}

	for (int i = minRow; i < RegionMap.size() + maxRow; ++i) {
		for (int j = (RegionMap[i].size() - 1) + maxCol; j >= minCol; --j) {
			RegionMap[i][j].RenderEntities(ren, camera);
			renderedRegions++;
		}
	}
}

std::vector<Region*> Map::GetNearbyRegions(int x, int y) {
	MapPos mp;
	mp.CalcRegion(x, y);
	int row = mp.rows;
	int column = mp.column;

	std::vector<Region*> regions;
	regions.reserve(9); // Ile maksymalnie mo¿e zwróciæ regionów
	for (int i = row - 1; i < row + 2; i++) {
		for (int j = column - 1; j < column + 2; j++) {
			if (i > -1 && i < MapPos::regionsCountHeight && j > -1 && j < MapPos::regionsCountWidth) {
				regions.emplace_back(&RegionMap[i][j]);
			}
		}
	}
	return regions;
}

std::vector<Point> Map::GetNearbyRegionsIndexes(int x, int y) {
	MapPos mp;
	mp.CalcRegion(x,y);
	int row = mp.rows;
	int column = mp.column;

	std::vector<Point> regions;
	regions.reserve(9); // Ile maksymalnie mo¿e zwróciæ regionów
	for (int i = row - 1; i < row + 2; i++) {
		for (int j = column - 1; j < column + 2; j++) {
			if (i > -1 && i < MapPos::regionsCountHeight && j > -1 && j < MapPos::regionsCountWidth) {
				regions.emplace_back(i, j);
			}
		}
	}
	return regions;
}



std::vector<Tile> Map::GetGlobalTilesAsVec() {
	std::vector<Tile> tilesVec;
	tilesVec.reserve(Map::tilesTypes.size());

	for(const auto &it: Map::tilesTypes){
		tilesVec.emplace_back(it.second);
	}
	return tilesVec;
}

Map::~Map() {
	tilesTypes.clear();
}