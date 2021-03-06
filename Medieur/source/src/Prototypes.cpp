#include "Prototypes.h"

#include <map>

#include "SpriteManager.h"
#include "GroundEntity.h"
#include "Rectangle.h"
#include "units.h"
#include "GroundEntityPlantModule.h"
#include "GroundEntityStockpileModule.h"
#include "Sprite.h"
#include "InventoryItem.h"
#include "Tile.h"
#include "Character.h"

namespace Prototypes {
	namespace {
		std::map<int, std::unique_ptr<GroundEntity> > groundEntityPrototypes;
		std::map<int, std::unique_ptr<InventoryItem> > pickableItemPrototypes;
		std::map<int, std::unique_ptr<Character> > characterPrototypes;
		std::map<int, std::unique_ptr<Tile> > tilePrototypes;
		std::map<std::string, int> idsByName;

		int idCount = 0;
		// constants
		const Rectangle kWallSource(224, 384, 32, 32);
		const Rectangle kDoorSource(0, 0, 32, 32);
		const Rectangle kPlantSource(units::kTileSize * 6, 0, units::kTileSize, 64);
		const Rectangle kPlantItemSource(units::kTileSize * 6, 5 * 64, units::kTileSize, 64);
		const Rectangle kGrassSource(0, 0, 512, 512);
		const Rectangle kCharacterSource(0, 0, 32, 32);
		const Rectangle kStockpileSource(0, 0, 32, 32);
	}

	bool createPrototypes() {
		// Ground entity prototypes
		createGroundEntityPrototype("Door", 1, new Sprite("bad_door.png", kDoorSource));
		createGroundEntityPrototype("Wall", 0, new Sprite("walls1.png", kWallSource));
		createGroundEntityPrototype("Plant", 1, new Sprite("plants.png", kPlantSource));
		createGroundEntityPrototype("Stockpile", 1, new Sprite("stockpile.png", kStockpileSource));

		// Tile prototype
		createTilePrototype("GrassTile", 1, new Sprite("grass00.png", kGrassSource));

		// Character prototype
		createCharacterPrototype("Guy", 1, new Sprite("Ukko.png", kCharacterSource));

		// Pickable item prototypes
		createPickableItemPrototype("Item_Wheat", 20, new Sprite("plants.png", kPlantItemSource));

		// Ground entity modules and functions
		GroundEntityPlantModule* plantProto = GroundEntityPlantModule::createPrototype();
		plantProto->setDropItem(getIdByName("Item_Wheat"));
		getGroundEntityPrototypeByName("Plant")->mModule = std::unique_ptr<GroundEntityPlantModule>(
			plantProto
			);

		SpriteManager::setGroundEntityFunction(getIdByName("Plant"),
			[](GroundEntity* pGrEntity) {
			int growth = static_cast<GroundEntityPlantModule*>(pGrEntity->mModule.get())->getGrowth();
			Rectangle newRect = kPlantSource;
			newRect.addY((growth - 1) * 64);
			return newRect;
		});
		
		GroundEntityStockpileModule* stockProto = GroundEntityStockpileModule::createPrototype();
		stockProto->addItem(getIdByName("Item_Wheat"));
		getGroundEntityPrototypeByName("Stockpile")->mModule = std::unique_ptr<GroundEntityStockpileModule>(stockProto);

		return true;
	}

	void clearAll()
	{
		groundEntityPrototypes.clear();
		pickableItemPrototypes.clear();
		characterPrototypes.clear();
		tilePrototypes.clear();
		idsByName.clear();

		SpriteManager::clearAll();
		idCount = 0;
	}

#pragma region GroundEntity
	void createGroundEntityPrototype(
		const std::string & pName, const float pMovSpeed, // Entity stuff
		Sprite* pSprite)
	{
		int id = getNextId();
		SpriteManager::createSprite(id, pSprite);
		groundEntityPrototypes[id] = std::unique_ptr<GroundEntity>(
			GroundEntity::createPrototype(id, pMovSpeed)
			);
		idsByName[pName] = id;
	}

	GroundEntity* getGroundEntityPrototypeByName(const std::string & pName)
	{
		return getGroundEntityPrototypeById(idsByName[pName]);
	}

	GroundEntity* getGroundEntityPrototypeById(const int pId)
	{
		return groundEntityPrototypes.at(pId).get();
	}
#pragma endregion

#pragma region Pickable Item
	void createPickableItemPrototype(const std::string & pName, const int pMaxAmount, Sprite * pSprite)
	{
		int id = getNextId();
		SpriteManager::createSprite(id, pSprite);
		pickableItemPrototypes[id] = std::unique_ptr<InventoryItem>(
			InventoryItem::createPrototype(id, pMaxAmount)
			);
		idsByName[pName] = id;
	}

	InventoryItem * getPickableItemPrototypeByName(const std::string & pName)
	{
		return getPickableItemPrototypeById(idsByName[pName]);
	}

	InventoryItem * getPickableItemPrototypeById(const int pId)
	{
		return pickableItemPrototypes.at(pId).get();
	}
#pragma endregion

#pragma region Tile
	void createTilePrototype(const std::string & pName, const float pMovSpeed, Sprite * pSprite)
	{
		int id = getNextId();
		SpriteManager::createSprite(id, pSprite);
		tilePrototypes[id] = std::unique_ptr<Tile>(
			Tile::createPrototype(id, TileType::GRASS)
			);
		idsByName[pName] = id;
	}

	Tile * getTilePrototypeByName(const std::string & pName)
	{
		return getTilePrototypeById(idsByName[pName]);
	}

	Tile * getTilePrototypeById(const int pId)
	{
		return tilePrototypes.at(pId).get();
	}
#pragma endregion

#pragma region Character
	void createCharacterPrototype(const std::string & pName, const float pMovSpeed, Sprite * pSprite)
	{
		int id = getNextId();
		SpriteManager::createSprite(id, pSprite);
		characterPrototypes[id] = std::unique_ptr<Character>(
			Character::createPrototype(id)
			);
		idsByName[pName] = id;
	}

	Character * getCharacterPrototypeByName(const std::string & pName)
	{
		return characterPrototypes[getIdByName(pName)].get();
	}

	Character * getCharacterPrototypeById(const int pId)
	{
		return characterPrototypes[pId].get();
	}
#pragma endregion

	int getIdByName(const std::string & pName)
	{
		return idsByName[pName];
	}

	int getNextId()
	{
		return idCount++;
	}
}
