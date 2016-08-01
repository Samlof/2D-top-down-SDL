#include "PathFinder.h"

#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <set>

#include "World.h"
#include "Point.h"
#include "Tile.h"
#include "units.h"

namespace PathFinder {
	namespace {
		World* worldPtr;

		int heuristicCost(Tile* pTileA, Tile* pTileB) {
			return pTileA->getXY().distanceTo(pTileB->getXY());
		}

		std::vector<Tile*> getNeighbours(Tile* pTile) {
			std::vector<Tile*> neighbours;
			int x = pTile->getX();
			int y = pTile->getY();
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					const int tX = x + i, tY = y + j;
					if ((i == 0 && j == 0) || (tX < 0) || (tY < 0) || (tX > worldPtr->getWidth()-1) || (tY > worldPtr->getHeight()-1)) continue;
					neighbours.push_back(worldPtr->getTile(x + i, y + j));
				}
			}
			return neighbours;
		}
	}

	void setWorld(World * pWorld)
	{
		worldPtr = pWorld;
	}
	std::stack<Tile*> FindPath(Tile* pStartTile, Tile* pEndTile)
	{
		if (pEndTile->isWalkable() == false) {
			printf("Bad end tile!\n");
			return std::stack<Tile*>();
		}
		// Init arrays
		using opensetTuple = std::tuple<Tile*, int>;
		auto cmp = [](opensetTuple lhs, opensetTuple rhs) {return std::get<1>(lhs) > std::get<1>(rhs); };
		std::priority_queue < opensetTuple, std::vector<opensetTuple>, decltype(cmp)> openSet(cmp);

		std::unordered_set<Tile*> openSetTiles;
		std::unordered_set<Tile*> closedSet;
		std::unordered_map<Tile*, Tile*> cameFrom;

		std::unordered_map<Tile*, int> gScores;
		std::unordered_map<Tile*, int> fScores;

		// First node
		openSetTiles.insert(pStartTile);
		gScores[pStartTile] = 1;
		fScores[pStartTile] = heuristicCost(pStartTile, pEndTile);
		openSet.emplace(std::make_tuple(pStartTile, fScores[pStartTile]));

		// Start loop
		while (openSet.empty() == false) {
			// Check next tile
			Tile* current = std::get<0>(openSet.top());
			if (current == pEndTile) {
				printf("Path found!\n");
				// Goal
				break;
			}
			if (std::get<1>(openSet.top()) != fScores[current]) {
				// Extra one
				openSet.pop();
				continue;
			}
			openSet.pop();
			openSetTiles.erase(current);
			closedSet.insert(current);
			// Neighbours
			std::vector<Tile*> neighbours = getNeighbours(current);
			for (auto childIt : neighbours) {
				Tile* child = childIt;
				if (closedSet.find(child) != closedSet.end() || child->isWalkable() == false) {
					continue;
				}
				int tempGScore = gScores[current] + 1;
				if (openSetTiles.find(child) == openSetTiles.end()) {
					// New tile
					openSetTiles.insert(child);
				}
				else if (tempGScore >= gScores[child]) {
					// Worse path
					continue;
				}
				// Better path
				cameFrom[child] = current;
				gScores[child] = tempGScore;
				fScores[child] = tempGScore + heuristicCost(child, pEndTile);
				openSet.emplace(std::make_tuple(child, fScores[child]));
			}
		}

		if (cameFrom[pEndTile] == nullptr) {
			printf("No path!\n");
			return std::stack<Tile*>();
		}
		std::stack<Tile*> tiles;
		Tile* next = pEndTile;
		while (next != pStartTile) {
			tiles.push(next);
			next = cameFrom[next];
		}
		return tiles;
	}
}