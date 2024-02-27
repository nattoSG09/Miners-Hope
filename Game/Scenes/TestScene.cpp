#include "TestScene.h"

#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Player/Player.h"
#include<iostream>
#include "../Objects/Ores/OreManager.h"
#include "../Objects/Images/CrossHair.h"


//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	srand((unsigned int)time(nullptr));

	// �X�e�[�W�𐶐�
	CreateStage<Stage>(static_cast<Cave_type>(rand() % CaveMax), this);

	// �z�΂𐶐�
	for (int i = 0; i <= 15; i++) {
		OreManager::CreateOre(static_cast<Ore_type>(rand() % Ore_Max), XMFLOAT3(rand() % 49 - 24, 0, rand() % 49 - 24), (rand() % 3) + 1, this);
	}

	// �v���C���[�𐶐�
	pPlayer_ = Instantiate<Player>(this);

	Instantiate<CrossHair>(this);
}

//�X�V
void TestScene::Update()
{
	// �v���C���[���o�������ړ�������...
	if (pPlayer_->IsExitCave() == true) {

		SceneManager* pSm = (SceneManager*)FindObject("SceneManager");
		// �V���̊m���ŒE�o
		if (rand() % 100 < 7) pSm->ChangeScene(SCENE_ID_RESULT, TID_WHITEOUT, 2.f);
		else pSm->ChangeScene(SCENE_ID_LOAD, TID_BLACKOUT, 1.f);
	}
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
	OreManager::AllRelease();
}
