#pragma once
#include "../../../Engine/GameObject.h"

class Pickaxe : public GameObject
{
	int hModel_;
public:
	Pickaxe(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};


