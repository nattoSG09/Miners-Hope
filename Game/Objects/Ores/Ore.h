#pragma once
#include "../../../Engine/GameObject.h"
#include <vector>
#include "../../Others/Circle.h"

enum Ore_type { Ore_Iron,Ore_Gold,Ore_Diamond,Ore_Emerald,Ore_Ruby,Ore_Max};

class Ore : public GameObject
{
	std::vector<int> hModels_;
	Ore_type type_;
	int durability_;
	Circle circle_;
public:
	Ore(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetDurability(int _d) { durability_ = _d; }
	int GetDurability() { return durability_; }
	int GetModelHandle() { return hModels_[type_]; }
	Circle GetCircle() { return circle_; }
	void SetType(Ore_type _type) { type_ = _type; }
	Ore_type GetType() { return type_; }
};