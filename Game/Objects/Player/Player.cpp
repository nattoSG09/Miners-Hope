#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"

#include "../../Others/Rect.h"
#include "../Stage/Stage.h"
#include <fstream>
#include <map>
#include <iostream>


namespace {

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
	hModel_ = Model::Load("Models/Player/Walking02.fbx");
	assert(hModel_ >= 0);

    hPoint_ = Model::Load("DebugCollision/Point.fbx");
    assert(hPoint_ >= 0);

	pStage_ = (Stage*)FindObject("Stage");
    
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

    // �L�����N�^�[�̌�����ύX
    ImGui::Text("transform_.rotate_ = %f,%f,%f", transform_.rotate_.x, transform_.rotate_.y, transform_.rotate_.z);
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

    Transform t;
    t.position_ = newCenter_;
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

    // �����x�N�g�����擾
    XMVECTOR moveDir = Camera::GetSightline();

    // Y�����ւ̈ړ��𐧌��������̂ŁAY�v�f���O�ɂ���
    moveDir = XMVectorSetY(moveDir, 0);
    moveDir = XMVector3Normalize(moveDir);

    // �X�s�[�h����Z
    moveDir *= speed;

    // �ړ������x�N�g����p��
    XMVECTOR move{ 0,0,0,0 };

    // �u�v�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_W)) {

        // ��ʑO���ɐi��
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);
        transform_.rotate_.y = angle_.y - 25;
        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�`�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_A)) {

        // ��ʉE���ɐi��
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);
        transform_.rotate_.y = (angle_.y - 25) - 90;

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�r�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_S)) {

        // ��ʌ���ɐi��
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);
        transform_.rotate_.y = (angle_.y - 25) + 180;

        // �A�j���[�V�����𓮍삳����
        isAnim = true;
    }

    // �u�c�v�L�[�������ꂽ��...
    if (Input::IsKey(DIK_D)) {

        // ��ʍ����ɐi��
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);
        transform_.rotate_.y = (angle_.y - 25) + 90;

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
    // ��]�̒��S�ʒu��ݒ�@
    XMFLOAT3 center = transform_.position_;
    center.y += 4.f;

    // �J�����̏œ_�E�ʒu���i�[����ϐ���p��
    XMFLOAT3 camTarget{};
    XMFLOAT3 camPosition{};

    // ��]�̂��߂̏����擾
    static float sensitivity = 0.3f;    // ���x
    {
        // �}�E�X���x��ݒ�E�擾
        ImGui::SliderFloat("sensitivity", &sensitivity, 0, 1);

        // �}�E�X�̈ړ��ʂ����]�ʂ�ݒ�E�擾
        XMFLOAT3 mouseMove = Input::GetMouseMove();
        angle_.x += mouseMove.y * sensitivity;
        angle_.y += mouseMove.x * sensitivity;

        // ������]�̏���E������ݒ肵��]�𐧌�
        {
            const float upperlimit = -80.f;
            if (angle_.x < upperlimit)angle_.x -= mouseMove.y * sensitivity;

            const float lowerlimit = 80.f;
            if (angle_.x > lowerlimit)angle_.x -= mouseMove.y * sensitivity;
        }
    }

     // �����̉�]���s��
    {
        // ��]�s����쐬
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));

        // ���̍s������ɉ�]
        XMVECTOR center_To_camTarget = { 0,0,1,0 };
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);

        // ������������
        float center_To_camTargetDistance = 6.f;
        if (Input::IsMouseButton(1))center_To_camTargetDistance = 3.f;

        center_To_camTarget *= center_To_camTargetDistance;

        // ���_����̈ʒu�����߂āA�J�����̏œ_��ݒ�
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);

        // center_To_camTarget�̋t�x�N�g����p��
        XMVECTOR center_To_camPosition = -center_To_camTarget;

        // ������Ɖ�]������
        float littleAngle = -45.f;
        if (Input::IsMouseButton(1))littleAngle = -60.f;
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(littleAngle)));

        // ���_����̈ʒu�����߂āA�J�����̈ʒu��ݒ�
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&camPosition,origin_To_camPosition);
    }
    
    // �����̉�]���s��
    {
        // ���S���ړ�
        XMVECTOR newCenter = (XMLoadFloat3(&camPosition) + XMLoadFloat3(&camTarget)) * 0.5f;
        
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);
        newCenter_ = center;
        
        // �c��]�̎����쐬
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);

        //// ��]�s����쐬
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis,XMConvertToRadians(angle_.x));

        //�@�œ_��ݒ� 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&camTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);

        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);

        // �ʒu��ݒ�
        XMVECTOR newCenter_To_camPosition = -newCenter_To_camTarget;
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + newCenter_To_camPosition;
        XMStoreFloat3(&camPosition, origin_To_camPosition);

    }

    Camera::SetTarget(camTarget);
    Camera::SetPosition(camPosition);
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

                // Effect���o��
                {
                    EmitterData data;

                    //��
                    data.textureFileName = "Images/cloudA.png";
                    data.position = ore->GetPosition();
                    data.delay = 0;
                    data.number = 80;
                    data.lifeTime = 30;
                    data.direction = XMFLOAT3(0, 1, 0);
                    data.directionRnd = XMFLOAT3(90, 90, 90);
                    data.speed = 0.1f;
                    data.speedRnd = 0.8;
                    data.size = XMFLOAT2(1.2, 1.2);
                    data.sizeRnd = XMFLOAT2(0.4, 0.4);
                    data.scale = XMFLOAT2(1.05, 1.05);
                    data.color = XMFLOAT4(0.5, 0.5, 0.1, 1);
                    data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
                    VFX::Start(data);

                    //�΂̕�
                    data.delay = 0;
                    data.number = 80;
                    data.lifeTime = 100;
                    data.positionRnd = XMFLOAT3(0.5, 0, 0.5);
                    data.direction = XMFLOAT3(0, 1, 0);
                    data.directionRnd = XMFLOAT3(90, 90, 90);
                    data.speed = 0.25f;
                    data.speedRnd = 1;
                    data.accel = 0.93;
                    data.size = XMFLOAT2(0.1, 0.1);
                    data.sizeRnd = XMFLOAT2(0.4, 0.4);
                    data.scale = XMFLOAT2(0.99, 0.99);
                    data.color = XMFLOAT4(0.4, 0.2, 0.0, 1);
                    data.deltaColor = XMFLOAT4(0, 0, 0, 0);
                    data.gravity = 0.003f;
                    VFX::Start(data);
                }


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
