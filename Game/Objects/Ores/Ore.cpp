#include "Ore.h"

#include "../../../Engine/Model.h"

Ore::Ore(GameObject* parent)
	:GameObject(parent,"Ore")
{
}

void Ore::Initialize()
{
	for (int i = 0; i < Ore_Max; i++) {
		hModels_.push_back(Model::Load("Models/Ores/Ore0" + std::to_string(i + 1) + ".fbx"));
	}

	transform_.rotate_.y = rand() % 271;
}

void Ore::Update()
{
	circle_.center_ = { transform_.position_.x,transform_.position_.z };
	circle_.radius_ = transform_.scale_.x * 3.f;
}

void Ore::Draw()
{
	Model::SetTransform(hModels_[type_], transform_);
	Model::Draw(hModels_[type_]);
}

void Ore::Release()
{
}
