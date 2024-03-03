#pragma once
#include "../../Engine/GameObject.h"

class LivingObject : public GameObject
{
protected:
	int hitPoint_;	// ‘Ì—Í
	int hModel_;	// ƒ‚ƒfƒ‹”Ô†
public:
	LivingObject(GameObject* parent,std::string name);
	virtual void Initialize() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Release() override = 0;

	int GatModelHandle() const { return hModel_; }
};