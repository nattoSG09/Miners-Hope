#include "TestScene.h"

#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Player/Player.h"
#include<iostream>
#include "../Objects/Ores/OreManager.h"


//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	srand((unsigned int)time(nullptr));

	// ステージを生成
	CreateStage<Stage>(static_cast<Cave_type>(rand() % CaveMax), this);

	// 鉱石を生成
	for (int i = 0; i <= 15; i++) {
		OreManager::CreateOre(static_cast<Ore_type>(rand() % Ore_Max), XMFLOAT3(rand() % 49 - 24, 0, rand() % 49 - 24), (rand() % 3) + 1, this);
	}

	// プレイヤーを生成
	pPlayer_ = Instantiate<Player>(this);

}

//更新
void TestScene::Update()
{
	int i = 0;
	for (auto ore : OreManager::ores_) {

		ImGui::Text("ore[%d].Type = %d",i,ore->GetType() );
		i++;
	}


	// プレイヤーが出入口を移動したら...
	if (pPlayer_->IsExitCave() == true) {

		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		// ７％の確率で脱出
		if (rand() % 100 < 7) pSm->ChangeScene(SCENE_ID_RESULT, TID_WHITEOUT, 2.f);
		else pSm->ChangeScene(SCENE_ID_LOAD, TID_BLACKOUT, 1.f);
	}
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
	OreManager::AllRelease();
}
