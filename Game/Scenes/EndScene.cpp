#include "EndScene.h"
#include "../../Engine/Image.h"
#include "../../Engine/SceneManager.h"

EndScene::EndScene(GameObject* parent)
	:GameObject(parent,"EndScene")
{
}

void EndScene::Initialize()
{
	hPict_ = Image::Load("Images/endImage.png");
	transform_.scale_ = { 0.68f,0.68f,1.f };
}

void EndScene::Update()
{
	static int time = 0;
	if (time >= 2 * 60) {
		time = 0;
		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		pSm->ChangeScene(SCENE_ID_TITLE, TID_BLACKOUT, 1.f);
	}

	time++;
}

void EndScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void EndScene::Release()
{
}
