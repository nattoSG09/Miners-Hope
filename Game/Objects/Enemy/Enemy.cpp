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

	SetHP(200);
}

void Enemy::Update()
{
	if (hitPoint_ <= 0) {
		KillMe();
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
