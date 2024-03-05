#pragma once
#include "../../../Engine/GameObject.h"
#include "../LivingObject.h"

class HPBar : public GameObject
{
	int hpFrame_;
	int hpGauge_;

	LivingObject* pObject_;
public:
	HPBar(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetObject(LivingObject* _obj) { pObject_ = _obj; }
};

