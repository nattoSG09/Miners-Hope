#pragma once
#include "../LivingObject.h"

class Enemy : public LivingObject
{
public:
	Enemy(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

