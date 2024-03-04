#pragma once
#include "../../../Engine/GameObject.h"

class HpBar : public GameObject
{
	int hPict_gauge_;
	int hPict_frame_;
public:
	HpBar(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

