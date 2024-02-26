#include "Stage.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../Ores/OreManager.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
}

void Stage::Initialize()
{
	for (int i = 0; i < CaveMax; i++) {
		hModels_.push_back(Model::Load("Models/Cave/test_Cave0" + std::to_string(i + 1) + ".fbx"));
	}
}

void Stage::Update()
{
	circle_.center_ = { 0.f,0.f };
	circle_.radius_ = 25.f;

	ImGui::Text("stage : type_ = %d", type_);
}

void Stage::Draw()
{
	Model::SetTransform(hModels_[type_], transform_);
	Model::Draw(hModels_[type_]);
}

void Stage::Release()
{
}
