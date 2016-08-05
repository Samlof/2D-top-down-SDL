#pragma once

#include <memory>

#include "Character.h"
#include "Point.h"

class GroundEntity;

enum class TileType {
	GRASS,
	EMPTY
};

class Tile
{
public:
	Tile(World* pWorld, const int pId, TileType pTileType, int pX, int pY);
	// For prototype
	Tile(const int pId, TileType pTileType) : 
		mId(pId), mTileType(pTileType), mX(-1), mY(-1) {}

	TileType getTileType() const { return mTileType; }
	void setTileType(TileType pTileType);

	//Ground entity functions
	std::shared_ptr<GroundEntity> getGroundEntity();
	bool hasGroundEntity() const { return static_cast<bool>(mGroundEntity); }
	void setGroundEntity(std::shared_ptr<GroundEntity> pGroundEntity) { mGroundEntity = pGroundEntity; }
	void clearGroundEntity() { mGroundEntity.reset(); }

	// Character functions
	std::shared_ptr<Character> getCharacter() { return mCharacter; }
	bool isReservableForCharacter() { return !mCharacter; }
	void reserveFor(std::shared_ptr<Character> pCharacter) { if(mCharacterStandingOn == false) mCharacter = pCharacter; }
	void moveTo() { if(mCharacter) mCharacterStandingOn = true; }
	bool isCharacterOn() const { return mCharacterStandingOn; }
	void clearCharater() { mCharacter.reset(); mCharacterStandingOn = false; }

	bool isWalkable() const;
	int getX() const { return mX; }
	int getY() const { return mY; }
	World* getWorld() { return mWorld; }
	Point getXY() const { return Point{ mX, mY }; }
	int getId() const { return mId; }
private:
	World* mWorld;
	const int mId;
	const int mX, mY;
	TileType mTileType;
	std::shared_ptr<GroundEntity> mGroundEntity;
	std::shared_ptr<Character> mCharacter;
	bool mCharacterStandingOn;
	float mMovementCost;
};

