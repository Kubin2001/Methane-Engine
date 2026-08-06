#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "Basics.h"
#include "Addons.h"
#include "Entity.h"

class Camera;

class Map;

enum class TileLayer {
	Base,
	One,
	Two
};

enum class TileType {
	NullBlock,
	GroundBlock,
	Special,
	Plant,
	Air
};

class Tile {
	public:
	MT::Rect rect{};
	MT::Texture* tex = nullptr;
	std::string key;
	TileType type = TileType::GroundBlock;
	bool passable = true;

	virtual ~Tile() = default;
};

class TileBase : public Tile {
	public:

};

class TileOne : public Tile{
	public:

};

class TileTwo : public Tile {
	public:

};

class Region : public GameObject {
	private:
		std::vector<std::vector<TileBase>> TilesBase;
		std::vector<std::vector<TileOne>> TilesOne;
		std::vector<std::vector<TileTwo>> TilesTwo;

		std::vector<std::shared_ptr<Entity>> localEntities{};

	public:
		void Innit(size_t tileSize, size_t tilesPerRegion);

		std::vector<std::vector<TileBase>>& GetTilesBase() { return TilesBase; };
		std::vector<std::vector<TileOne>>& GetTilesOne() { return TilesOne; }
		std::vector<std::vector<TileTwo>>& GetTilesTwo() { return TilesTwo; }

		std::vector<std::shared_ptr<Entity>>& GetEntities() { return localEntities; }


		void RenderTiles(MT::Renderer* renderer, Camera* cam);

		void RenderEntities(MT::Renderer* renderer, Camera* cam);
};


class Map {
	private:
		std::vector<std::vector<Region>> RegionMap;
		std::vector<Region*> FlatRegions;
		MT::Renderer* ren = nullptr;
		Camera* camera = nullptr;

		unsigned char shadowValue = 0; // Alpha cienia |255 ca³e czarne|   |0 wszystko widaæ|
		MT::Texture* shadowTexture = nullptr;

		Point currentIterRegion{ 0,0 };

		inline static Tile baseTile;

	public:
		inline static std::unordered_map<std::string, Tile> tilesTypes;
		inline static int plantsWeight = 0;

		int GlobalHumanSize = 0;
		int GlobalZombieSize = 0;
		int GlobalAnimalSize = 0;


		Map();

		Map(MT::Renderer* ren, Camera *camera);

		std::vector<std::vector<Region>>& GetRegions();
		std::vector<Region*>& GetFlatRegions();

		void Init(size_t tileSize, size_t tilesPerRegion, size_t regionMapWidth, size_t regionMapHeight);

		static void SetTile(const std::string& key, Tile& tile, bool variantTextures = true, bool maxGrowth = false);

		//static void DestroyTile(Map* map, Tile& tile, bool drop = false); // TODO

		void Input(SDL_Event& event);

		void Render();

		//Funkcja która zwraca regiony wokó³ podanego ze sprawdzaniem granic mapy
		std::vector<Region*> GetNearbyRegions(int x, int y);

		std::vector<Point> GetNearbyRegionsIndexes(int x, int y);

		template <TileLayer layer = TileLayer::One>
		Tile* GetTile(const MapPos& mp) noexcept {
			if (!mp.CorrectnessAbsTileS()) { return nullptr; }

			if constexpr (layer == TileLayer::Base) {
				return &RegionMap[mp.rows][mp.column].GetTilesBase()[mp.rowsTile][mp.columnTile];
			}
			else if constexpr (layer == TileLayer::One) {
				return &RegionMap[mp.rows][mp.column].GetTilesOne()[mp.rowsTile][mp.columnTile];
			}
			else { // return layer two
				return &RegionMap[mp.rows][mp.column].GetTilesTwo()[mp.rowsTile][mp.columnTile];
			}
			return nullptr;
		}

		std::vector<Tile*> GetNearbyTiles(const Point& p, TileLayer layer) {
			MapPos mp(p.x - MapPos::tileSize, p.y - MapPos::tileSize);

			std::vector<Tile*> tiles;
			tiles.reserve(9);
			int rowMax = mp.absTileRows + 3;
			int colMax = mp.absTileColumn + 3;
			int colStart = mp.absTileColumn;
			for (int row = mp.absTileRows; row < rowMax; row++) {
				for (int col = colStart; col < colMax; col++) {
					mp.absTileColumn = col;
					Tile* tile = nullptr;
					switch (layer) {
						case TileLayer::Base:
							tile = GetTile<TileLayer::Base>(mp);
							break;
						case TileLayer::One:
							tile = GetTile<TileLayer::One>(mp);
							break;
						case TileLayer::Two:
							tile = GetTile<TileLayer::Two>(mp);
							break;
					}
					mp.RecalculateFromAbs();
				}
				mp.absTileRows = row;
				mp.RecalculateFromAbs();
			}

			return tiles;
		}

		std::vector<Tile>GetGlobalTilesAsVec(); 


		~Map();
};