#pragma once

#include "IGroundEntityModule.h"
#include "CountdownCounter.h"

class GroundEntity;
class Job;

class GroundEntityPlantModule : public IGroundEntityModule {
public:
	~GroundEntityPlantModule();

	// Inherited via IGroundEntityModule
	virtual void update() override;
	virtual void interact() override;
	virtual void pickup() override;

	virtual GroundEntityPlantModule* clone(IGroundEntityModule* pPrototype, GroundEntity * pThisEntity) override {
		return new GroundEntityPlantModule(static_cast<GroundEntityPlantModule*>(pPrototype), pThisEntity);
	}
	int getGrowth() const { return mGrowth; }
	void setDropItem(const int pId) { mDropItemId = pId; }

	void cleanJobs();
private:
	int mGrowth;
	int mHealth;
	CountdownCounter mGrowthCounter;
	CountdownCounter mHealthCounter;
	Job* mInteractJob;
	Job* mPickupJob;

	int mDropItemId;
	void rot();

	GroundEntityPlantModule(GroundEntityPlantModule* pPrototype, GroundEntity * pThisEntity);
	GroundEntityPlantModule();
public:
	static GroundEntityPlantModule* createPrototype() {
		return new GroundEntityPlantModule();
	}
};