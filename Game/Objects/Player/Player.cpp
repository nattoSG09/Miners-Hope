#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"

#include "../../Others/Rect.h"
#include "../Stage/Stage.h"
#include "../Ores/OreManager.h"
#include <fstream>
#include <map>
#include <iostream>

namespace {
    const float sensitivity = 0.5f;// �}�E�X���x

    // ��̃x�N�g������p�x�����߂�֐�(���W�A��)
    float AngleBetweenVectors(XMVECTOR& _vec1, XMVECTOR& _vec2) {
        // �x�N�g���𐳋K������
        XMVECTOR normVec1 = XMVector3Normalize(_vec1);
        XMVECTOR normVec2 = XMVector3Normalize(_vec2);

        // ���ς��v�Z����
        float dotProduct = XMVectorGetX(XMVector3Dot(normVec1, normVec2));

        // �O�ς��v�Z����
        XMVECTOR crossProduct = XMVector3Cross(normVec1, normVec2);
        float crossLength = XMVectorGetX(XMVector3Length(crossProduct));

        // atan2���g�p���Ċp�x���v�Z���A�Ԃ�
        return atan2(crossLength, dotProduct);
    }
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Models/Player/Walking.fbx");
	assert(hModel_ >= 0);

	pStage_ = (Stage*)FindObject("Stage");
    
    direction_ = Camera::GetSightline();
    direction_ = XMVectorSetY(direction_, 0);

    myInventory_.Load("inventory.ini");
}

void Player::Update()
{
    // �J�����̈ʒu�𑀍�
    CalcCameraMove();

    // �L�����N�^�[�̈ʒu�𑀍�
    Move();

    // �̌@����
    Mining();

    // �L�����N�^�[�̌�����ݒ�
    {
        XMVECTOR sightDir = Camera::GetSightline();
        sightDir = XMVectorSetY(sightDir, 0);
        if (XMVector3Equal(direction_, sightDir) == false) {
            float angle = AngleBetweenVectors(direction_, sightDir);
            transform_.rotate_.y += XMConvertToDegrees(angle);
            direction_ = sightDir;
        }
    }
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
    myInventory_.Save("inventory.ini");
}

bool Player::IsExitCave()
{
	if (pStage_ == nullptr)pStage_ = (Stage*)FindObject("Stage");


	switch (pStage_->GetCaveType())
	{
	case Cave01: {
		Rect rect1(0, 24, 12, 5);
		if (rect1.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect2(0, -24, 12, 5);
		if (rect2.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect3(24, 0, 5, 12);
		if (rect3.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect4(-24, 0, 5, 12);
		if (rect4.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;
	}return false;
	case Cave02: {
		Rect rect3(24, 0, 5, 12);
		if (rect3.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect4(-24, 0, 5, 12);
		if (rect4.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;
	}return false;
	case Cave03: {
		Rect rect2(0, -24, 12, 5);
		if (rect2.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect3(24, 0, 5, 12);
		if (rect3.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;
	}return false;
	case Cave04: {
		Rect rect2(0, -24, 12, 5);
		if (rect2.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect3(24, 0, 5, 12);
		if (rect3.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;

		Rect rect4(-24, 0, 5, 12);
		if (rect4.ContainsPoint(transform_.position_.x, transform_.position_.z))return true;
	}return false;
	}
	return false;
}

void Player::Move()
{
    // �A�j���[�V����������Ă��邩�ǂ���
    bool isAnim = false;

    // �v���C���[�̑��x
    float speed = 0.1f;

    // ���K�����������x�N�g�����擾
    XMVECTOR moveDir = XMVector3Normalize(Camera::GetSightline());

    // Y�����ւ̈ړ��𐧌��������̂ŁAY�v�f���O�ɂ���
    moveDir = XMVectorSetY(moveDir, 0);

    // �X�s�[�h����Z
    moveDir *= speed;

    // �ړ������x�N�g����p��
    XMVECTOR move{ 0,0,0,0 };

    // �u�v�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_W)) {

        // ��ʑO���ɐi��
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�`�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_A)) {

        // ��ʉE���ɐi��
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�r�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_S)) {

        // ��ʌ���ɐi��
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�c�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_D)) {

        // ��ʍ����ɐi��
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �A�j���[�V�������s��
    static bool prevAnim = false;
    if (isAnim == true) {
        if (prevAnim == false)Model::SetAnimFrame(hModel_, 0, 60, 1);
        prevAnim = true;
    }
    else {
        Model::SetAnimFrame(hModel_, 0, 0, 0);
        prevAnim = false;
    }
}

void Player::CalcCameraMove()
{
    // �v���C���[�̓����̈ʒu��ݒ�
    XMFLOAT3 playerHead_position = transform_.position_;
    playerHead_position.y += 4.f;

    // �}�E�X�̏����擾
    XMFLOAT3 mouseMove = Input::GetMouseMove();

    // �J�����̈ʒu�̉�]
    XMFLOAT3 camera_position = Camera::GetPosition();
    {

        // ���K���ς݂̌����x�N�g����p��
        XMVECTOR player_To_camPos = XMLoadFloat3(&camera_position) - XMLoadFloat3(&playerHead_position);
        player_To_camPos = XMVector3Normalize(player_To_camPos);

        // ��]�s����}�E�X�̈ړ��ʂ���ɍ쐬
        XMMATRIX matRotate =
            XMMatrixRotationX(XMConvertToRadians(mouseMove.y * sensitivity)) * XMMatrixRotationY(XMConvertToRadians(mouseMove.x * sensitivity));

        // ��]�s����|���āA�����x�N�g������]
        player_To_camPos = XMVector3Transform(player_To_camPos, matRotate);

        // ������ύX
        player_To_camPos *= 10.f;

        // ���_�O�C�O�����]��̃J�����̈ʒu�ɐL�т�x�N�g�����쐬���A�ʒu�ɑ��
        XMVECTOR origin_To_camPos = player_To_camPos + XMLoadFloat3(&playerHead_position);
        XMStoreFloat3(&camera_position, origin_To_camPos);

    }

    // �J�����̏œ_�̉�]
    XMFLOAT3 camera_target = Camera::GetTarget();
    {

        // ���K���ς݂̌����x�N�g����p��
        XMVECTOR player_To_camPos = XMLoadFloat3(&camera_position) - XMLoadFloat3(&playerHead_position);
        player_To_camPos = XMVector3Normalize(player_To_camPos);

        // ��]�s����}�E�X�̈ړ��ʂ���ɍ쐬
        XMMATRIX matRotate =
            XMMatrixRotationX(XMConvertToRadians(mouseMove.y * sensitivity) + 45.f) * XMMatrixRotationY(XMConvertToRadians(mouseMove.x * sensitivity) + 45.f);

        // ��]�s����|���āA�����x�N�g������]
        XMVECTOR player_To_camTgt = XMVector3Transform(player_To_camPos, matRotate);

        // ������ύX
        player_To_camTgt *= 10.f;

        // ���_�O�C�O�����]��̃J�����̈ʒu�ɐL�т�x�N�g�����쐬���A�ʒu�ɑ��
        XMVECTOR origin_To_camTgt = XMLoadFloat3(&playerHead_position) - player_To_camTgt;
        XMStoreFloat3(&camera_target, origin_To_camTgt);

    }

    Camera::SetPosition(camera_position);

    // �œ_�������̎��͂��܂������B
    Camera::SetTarget(playerHead_position);
    //Camera::SetTarget(camera_target);

}

void Player::Mining()
{
    ImGui::Begin("Inventory"); {
        ImGui::Text("num_Iron = %d", myInventory_.num_Iron);
        ImGui::Text("num_Gold = %d", myInventory_.num_Gold);
        ImGui::Text("num_Diamond = %d", myInventory_.num_Diamond);
        ImGui::Text("num_Emerald = %d", myInventory_.num_Emerald);
        ImGui::Text("num_Ruby = %d", myInventory_.num_Ruby);
    }
    ImGui::End();

    int i = 0;
    for (auto& ore : OreManager::ores_) {
        // �����x�N�g�����擾
        XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

        // ���C�L���X�g�𔭎�
        RayCastData sightRay; {
            XMStoreFloat3(&sightRay.dir, sightline);
            sightRay.start = Camera::GetPosition();
            Model::RayCast(ore->GetModelHandle(), &sightRay);

        }

        // �̌@�\���ǂ���
        bool isMining = false;
        if (ore->GetCircle().ContainsPoint(transform_.position_.x, transform_.position_.z)
            && sightRay.hit) {

            isMining = true;
            if (Input::IsMouseButtonDown(0)) {

                if (ore->GetDurability() <= 0) {
                    switch (ore->GetType()) {
                    case Ore_Iron: myInventory_.num_Iron++; break;
                    case Ore_Gold: myInventory_.num_Gold++; break;
                    case Ore_Diamond: myInventory_.num_Diamond++; break;
                    case Ore_Emerald: myInventory_.num_Emerald++; break;
                    case Ore_Ruby: myInventory_.num_Ruby++; break;
                    }
                    OreManager::Destroy(ore);
                }
                else {
                    switch (ore->GetType()) {
                    case Ore_Iron: myInventory_.num_Iron++; break;
                    case Ore_Gold: myInventory_.num_Gold++; break;
                    case Ore_Diamond: myInventory_.num_Diamond++; break;
                    case Ore_Emerald: myInventory_.num_Emerald++; break;
                    case Ore_Ruby: myInventory_.num_Ruby++; break;
                    }
                    ore->SetDurability(ore->GetDurability() - 1);
                }

                isMining = false;
            }
        }
        
    }
}

void Inventory::Load(std::string _filePath)
{
    std::ifstream file(_filePath);

    if (!file.is_open())return;

    std::string line;
    std::map<std::string, int*> itemMap = {
        {"Iron", &num_Iron},
        {"Gold", &num_Gold},
        {"Diamond", &num_Diamond},
        {"Emerald", &num_Emerald},
        {"Ruby", &num_Ruby}
    };

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';' || line[0] == '#') // Ignore comments and empty lines
            continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (itemMap.find(key) != itemMap.end()) {
                *(itemMap[key]) = std::stoi(value);
            }
        }
    }

    file.close();
}

void Inventory::Save(std::string _filePath)
{
    std::ofstream file(_filePath);

    if (!file.is_open())return;

    file << "[numbers]\n";
    file << "Iron=" << num_Iron << "\n";
    file << "Gold=" << num_Gold << "\n";
    file << "Diamond=" << num_Diamond << "\n";
    file << "Emerald=" << num_Emerald << "\n";
    file << "Ruby=" << num_Ruby << "\n";

    file.close();
}
