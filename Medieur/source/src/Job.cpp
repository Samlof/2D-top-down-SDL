#include "Job.h"

#include "JobManager.h"
#include "Character.h"
#include "InventoryItem.h"
#include "ItemManager.h"

#include <iostream>

namespace {
	const int kWorkReuiredTime = 10;
}

Job::Job(TargetFunc & pTargetFunc, JobFunc & pJobFunc,
	std::vector<InventoryItem*> pRequirements)
	:
	mManager(nullptr), mCharacter(nullptr),
	mJobFunc(pJobFunc), mTargetTile(pTargetFunc), mJobRequiredTime(kWorkReuiredTime), mJobTime(0),
	mRequirements(pRequirements)
{
}

Job::Job(TargetFunc & pTargetFunc, JobFunc & pJobFunc)
	: mManager(nullptr), mCharacter(nullptr),
	mJobFunc(pJobFunc), mTargetTile(pTargetFunc), mJobRequiredTime(kWorkReuiredTime), mJobTime(0)
{
}

Job::~Job() {
	for (auto it : mRequirements) {
		delete it;
	}
}


void Job::reserve(Character * pCharacter)
{
	if (isReserved()) throw "Job already reserved!";
	mCharacter = pCharacter;
	mManager->removeJobFromOpen(this);
}

bool Job::hasAllMaterials()
{
	for (auto it : mRequirements) {
		if (it->isFull() == false) {
			return false;
		}
	}
	return true;
}

InventoryItem * Job::getRequirement()
{
	for (auto it : mRequirements) {
		if (it->isFull() == false) {
			return it;
		}
	}
	return nullptr;
}

void Job::fillRequirement(InventoryItem * pItem)
{
	for (auto it : mRequirements) {
		if (it->isSameType(pItem)) {
			it->takeFrom(pItem);
			if (mChangedFunc.empty() == false) {
				for (auto it : mChangedFunc) {
					it();
				}
			}
			return;
		}
	}
}

void Job::addReq(const int pId, const int pAmount)
{
	InventoryItem* tempItem = ItemManager::createLocalPickableItem(pId, pAmount);
	mRequirements.push_back(tempItem);
}

void Job::clearCharacter()
{
	if (mCharacter == nullptr) return;
	mCharacter->cancelPath();
	mCharacter->clearJob();
	mCharacter = nullptr;
}

void Job::cancelReserve()
{
	clearCharacter();
	mManager->addJobToOpen(this);
}

void Job::cancelJob()
{
	// TODO: Drop items to ground?
	clearCharacter();
	mManager->deleteJob(this);
}

bool Job::doWork()
{
	if (mCharacter->getTile() != mTargetTile()) {
		throw "Character not at target yet!";
	}
	mJobTime++;
	if (mJobTime >= mJobRequiredTime) {
		mJobFunc();
		return true;
	}
	return false;
}
