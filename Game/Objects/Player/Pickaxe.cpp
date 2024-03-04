#include "Pickaxe.h"
#include "../../../Engine/Model.h"

Pickaxe::Pickaxe(GameObject* parent)
	:GameObject(parent,"Pickaxe")
{
}

void Pickaxe::Initialize()
{
	hModel_ = Model::Load("Models/Player/toolPickaxe.fbx");
}

void Pickaxe::Update()
{
}

void Pickaxe::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Pickaxe::Release()
{
}
