#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"

#include "../../Others/Rect.h"
#include "../Stage/Stage.h"
#include "../Ores/OreManager.h"

namespace {
    const float SENSITIVITY = 0.5f;// �}�E�X���x
    const float PLAYER_TO_CAMERA_DISTANCE = 10.f;

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

    // ���`�C�[�W���O�֐�
    XMFLOAT3 linearEasing(const XMFLOAT3& start, const XMFLOAT3& end, float duration, float currentTime) {
        float t = currentTime / duration;
        return XMFLOAT3(
            start.x + (end.x - start.x) * t,
            start.y + (end.y - start.y) * t,
            start.z + (end.z - start.z) * t
        );
    }

    // �C�[�W���O�֐�
    XMFLOAT3 easing(const XMFLOAT3& start, const XMFLOAT3& end, float duration, float currentTime) {
        return linearEasing(start, end, duration, currentTime); // �����Ő��`�C�[�W���O���g�p
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
    
}

void Player::Update()
{
    // �J�����̈ʒu�𑀍�
    CalcCameraMove();

    // �L�����N�^�[�̈ʒu�𑀍�
    Move();
   
    // �̌@����
    Mining();

    
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
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
    // �}�E�X�̈ړ��ʂ����]�ʂ��擾
    XMFLOAT3 mouseMove = Input::GetMouseMove();
    cameraRotate_.x += mouseMove.x * SENSITIVITY;
    cameraRotate_.y += mouseMove.y * SENSITIVITY;

    // �����l�������x�N�g����p�Ӂi���{�����x�N�g���j
    XMVECTOR sightDirection = { 0.f,0.f,1.f,0.f };

    XMFLOAT3 tmpCameraRotate = {cameera}
    
}

void Player::Mining()
{
    ImGui::Begin("miningData");

    int i = 0;
    for (auto& ore : OreManager::ores_) {
        // �����x�N�g�����擾
        XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

        // ���C�L���X�g�𔭎�
        RayCastData sightRay; {
            XMStoreFloat3(&sightRay.dir, sightline);
            sightRay.start = Camera::GetPosition();
            Model::RayCast(ore->GetModelHandle(), &sightRay);

            // debug
            ImGui::Text("Ore[%d].sightRay.hit = %s",i,sightRay.hit ? "true" : "false");
        }

        // �̌@�\���ǂ���
        bool isMining = false;
        if (ore->GetCircle().ContainsPoint(transform_.position_.x, transform_.position_.z)
            && sightRay.hit) {

            isMining = true;
            if (Input::IsMouseButtonDown(0)) {

                if (ore->GetDurability() <= 0) {
                    OreManager::Destroy(ore);
                }
                else {
                    ore->SetDurability(ore->GetDurability() - 1);
                }

                isMining = false;
            }
        }
        ImGui::Text("Ore[%d].duravity = %d",i, ore->GetDurability());
        ImGui::Text("Ore[%d].isMining = %s",i, isMining ? "true" : "false");

        i++;
    }

    ImGui::End();
}
