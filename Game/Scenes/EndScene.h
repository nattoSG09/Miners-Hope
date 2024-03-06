#pragma once
#include "../../Engine/GameObject.h"

class EndScene : public GameObject
{
	int hPict_;
public:
	EndScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

