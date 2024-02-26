#pragma once

#include "../../../Engine/GameObject.h"

class TitleImage : public GameObject
{
	int hPict_;
public:
	TitleImage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
};

