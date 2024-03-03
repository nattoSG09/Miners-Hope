#include "Enemy.h"
#include "../../../Engine/Model.h"

Enemy::Enemy(GameObject* parent)
	:LivingObject(parent,"Enemy")
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Models/Enemy/Walking.fbx");
	assert(hModel_ >= 0);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
