#include "LoadScene.h"
#include "../../Engine/SceneManager.h"
#include "../../Engine/Image.h"

LoadScene::LoadScene(GameObject* parent)
	:GameObject(parent,"LoadScene")
{
}

void LoadScene::Initialize()
{
	hPict_ = Image::Load("Images/loading01.png");
	assert(hPict_ <= 0);

	transform_.scale_ = { 0.65f,0.65f, 1.0f };
}

void LoadScene::Update()
{
	static int time = 0;
	if (time >= 2 * 60) {
		time = 0;
		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		pSm->ChangeScene(SCENE_ID_TEST, TID_BLACKOUT, 1.f);
	}
	
	time++;

}

void LoadScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void LoadScene::Release()
{
}
