#include "ResultScene.h"
#include "../../Engine/Image.h"
#include "../../Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene")
{
}

void ResultScene::Initialize()
{	
	hPict_ = Image::Load("Images/resultImage.png");
	transform_.scale_ = { 0.68f,0.68f,1.f };
}

void ResultScene::Update()
{
	static int time = 0;
	if (time >= 2 * 60) {
		time = 0;
		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		pSm->ChangeScene(SCENE_ID_END, TID_BLACKOUT, 1.f);
	}

	time++;

}

void ResultScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void ResultScene::Release()
{
}
