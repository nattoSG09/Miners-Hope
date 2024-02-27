#include "TitleScene.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Ores/OreManager.h"
#include "../Objects/Images/TitleImage.h"

#include "../../Engine/Camera.h"
#include "../../Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene")
{
}

void TitleScene::Initialize()
{


	// ステージを生成
	CreateStage<Stage>(static_cast<Cave_type>(rand() % CaveMax), this);

	// 鉱石を生成
	for (int i = 0; i <= 15; i++) {
		OreManager::CreateOre(static_cast<Ore_type>(rand() % Ore_Max), XMFLOAT3(rand() % 49 - 24, 0, rand() % 49 - 24), (rand() % 3) + 1, this);
	}

	Instantiate<TitleImage>(this);

	Camera::SetTarget(XMFLOAT3(0, 5, 0));
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE)) {
		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		pSm->ChangeScene(SCENE_ID_LOAD, TID_BLACKOUT, 2.f);

	}
	
	XMVECTOR sightline{ 0.f,0.f,-1.f,0.f };

	static float angle = 0;
	angle += 0.1f;
	XMMATRIX matRotate = XMMatrixRotationY(XMConvertToRadians(angle));
	sightline = XMVector3Transform(sightline, matRotate);

	sightline *= 15.f;

	XMFLOAT3 camTgt = Camera::GetTarget();
	XMVECTOR origin_To_camPos = XMLoadFloat3(&camTgt) + sightline;

	XMFLOAT3 camPos{};
	XMStoreFloat3(&camPos, origin_To_camPos);
	camPos.y += 2.f;

	Camera::SetPosition(camPos);
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
	OreManager::AllRelease();
}
