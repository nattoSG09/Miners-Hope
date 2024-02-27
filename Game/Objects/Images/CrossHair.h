#pragma once
#include "../../../Engine/GameObject.h"

class CrossHair : public GameObject
{
	int hPict_;
public:
	CrossHair(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

