#pragma once
#include "../../Engine/GameObject.h"

class LivingObject : public GameObject
{
protected:
	int hitPoint_;	// 体力
	int hModel_;	// モデル番号
public:
	LivingObject(GameObject* parent,std::string name);
	virtual void Initialize() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Release() override = 0;

	int GatModelHandle() const { return hModel_; }
	int GetHP()const { return hitPoint_; }
	void SetHP(int _hp) { hitPoint_ = _hp; }
};