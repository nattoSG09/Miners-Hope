#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"
#include "../../Others/Rect.h"

#include "../Enemy/Enemy.h"
#include "../Stage/Stage.h"
#include "Pickaxe.h"

#include <fstream>
#include <map>
#include <iostream>


namespace {

    // 二つのベクトルから角度を求める関数(ラジアン)
    float AngleBetweenVectors(XMVECTOR& _vec1, XMVECTOR& _vec2) {
        // ベクトルを正規化する
        XMVECTOR normVec1 = XMVector3Normalize(_vec1);
        XMVECTOR normVec2 = XMVector3Normalize(_vec2);

        // 内積を計算する
        float dotProduct = XMVectorGetX(XMVector3Dot(normVec1, normVec2));

        // 外積を計算する
        XMVECTOR crossProduct = XMVector3Cross(normVec1, normVec2);
        float crossLength = XMVectorGetX(XMVector3Length(crossProduct));

        // atan2を使用して角度を計算し、返す
        return atan2(crossLength, dotProduct);
    }

    // 線形イージング関数
    XMFLOAT3 linearEasing(const XMFLOAT3& start, const XMFLOAT3& end, float duration, float currentTime) {
        float t = currentTime / duration;
        return XMFLOAT3(
            start.x + (end.x - start.x) * t,
            start.y + (end.y - start.y) * t,
            start.z + (end.z - start.z) * t
        );
    }

    // イージング関数
    XMFLOAT3 easing(const XMFLOAT3& start, const XMFLOAT3& end, float duration, float currentTime) {
        return linearEasing(start, end, duration, currentTime); // ここで線形イージングを使用
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
    // カメラの位置を操作
    CalcCameraMove();

    // キャラクターの位置を操作
    Move();
   
    // 採掘操作
    Mining();


    if (Input::IsMouseButtonDown(0)) {
        EnemyAttack();
    }

    if(pEnemy_ != nullptr)
    ImGui::Text("Enemy HP = %d", pEnemy_->GetHP());

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
    // アニメーションがされているかどうか
    bool isAnim = false;

    // 方向ベクトルを設定
    XMVECTOR direction{}; {
       // 視線ベクトルを取得
       XMVECTOR sightline = Camera::GetSightline(); {
           sightline = XMVectorSetY(sightline, 0);
           sightline = XMVector3Normalize(sightline);
       }

       if (Input::IsKey(DIK_W)) {
           direction += sightline;
           isAnim = true;
           transform_.rotate_.y = angle_.y - 25;
       }

       if (Input::IsKey(DIK_A)) {
           direction += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90)));
           isAnim = true;
           transform_.rotate_.y = (angle_.y - 25) - 90;
       }

       if (Input::IsKey(DIK_S)) {
           direction += -sightline;
           isAnim = true;
           transform_.rotate_.y = (angle_.y - 25) + 180;
       }

       if (Input::IsKey(DIK_D)) {
           direction += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90)));
           isAnim = true;
           transform_.rotate_.y = (angle_.y - 25) + 90;
       }

       // 正規化
       direction = XMVector3Normalize(direction);
    }

   // 移動速度を設定
   float speed = 0.1f; 
   if (Input::IsKey(DIK_LSHIFT)) {
       speed *= 1.5f;
   }
   
   // 移動
   XMVECTOR move = direction * speed;
   XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + move);

   // アニメーションを行う
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
    // 回転の中心位置を設定①
    XMFLOAT3 center = transform_.position_;
    center.y += 4.f;

    // カメラの焦点・位置を格納する変数を用意
    XMFLOAT3 camTarget{};
    XMFLOAT3 camPosition{};

    // 回転のための情報を取得
    static float sensitivity = 0.3f;    // 感度
    {
        // マウス感度を設定・取得
        ImGui::SliderFloat("sensitivity", &sensitivity, 0, 1);

        // マウスの移動量から回転量を設定・取得
        XMFLOAT3 mouseMove = Input::GetMouseMove();
        angle_.x += mouseMove.y * sensitivity;
        angle_.y += mouseMove.x * sensitivity;

        // ｘ軸回転の上限・下限を設定し回転を制限
        {
            const float upperlimit = -80.f;
            if (angle_.x < upperlimit)angle_.x -= mouseMove.y * sensitivity;

            const float lowerlimit = 80.f;
            if (angle_.x > lowerlimit)angle_.x -= mouseMove.y * sensitivity;
        }
    }

     // ｙ軸の回転を行う
    {
        // 回転行列を作成
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));

        // ↑の行列を元に回転
        XMVECTOR center_To_camTarget = { 0,0,1,0 };
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);

        // 長さを加える
        float center_To_camTargetDistance = 7.f;
        if (Input::IsMouseButton(1))center_To_camTargetDistance = 3.f;

        center_To_camTarget *= center_To_camTargetDistance;

        // 原点からの位置を求めて、カメラの焦点を設定
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);

        // center_To_camTargetの逆ベクトルを用意
        XMVECTOR center_To_camPosition = -center_To_camTarget;

        // ちょっと回転させる
        float littleAngle = -30.f;
        if (Input::IsMouseButton(1))littleAngle = -60.f;
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(littleAngle)));

        // 原点からの位置を求めて、カメラの位置を設定
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&camPosition,origin_To_camPosition);
    }
    
    // ｘ軸の回転を行う
    {
        // 中心を移動
        XMVECTOR newCenter = (XMLoadFloat3(&camPosition) + XMLoadFloat3(&camTarget)) * 0.5f;
        
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);
        newCenter_ = center;
        
        // 縦回転の軸を作成
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);

        //// 回転行列を作成
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis,XMConvertToRadians(angle_.x));

        //　焦点を設定 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&camTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);

        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&camTarget, origin_To_camTarget);

        // 位置を設定
        XMVECTOR newCenter_To_camPosition = -newCenter_To_camTarget;
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + newCenter_To_camPosition;
        XMStoreFloat3(&camPosition, origin_To_camPosition);

    }

    Camera::SetTarget(camTarget);
    Camera::SetPosition(camPosition);
}

void Player::Mining()
{
    /*ImGui::Begin("Inventory"); {
        ImGui::Text("num_Iron = %d", myInventory_.num_Iron);
        ImGui::Text("num_Gold = %d", myInventory_.num_Gold);
        ImGui::Text("num_Diamond = %d", myInventory_.num_Diamond);
        ImGui::Text("num_Emerald = %d", myInventory_.num_Emerald);
        ImGui::Text("num_Ruby = %d", myInventory_.num_Ruby);
    }
    ImGui::End();*/

    int i = 0;
    for (auto& ore : OreManager::ores_) {
        // 視線ベクトルを取得
        XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

        // レイキャストを発射
        RayCastData sightRay; {
            XMStoreFloat3(&sightRay.dir, sightline);
            sightRay.start = Camera::GetPosition();
            Model::RayCast(ore->GetModelHandle(), &sightRay);

        }

        // 採掘可能かどうか
        bool isMining = false;
        if (ore->GetCircle().ContainsPoint(transform_.position_.x, transform_.position_.z)
            && sightRay.hit) {
            isMining = true;
            if (Input::IsMouseButtonDown(0)) {

                // Effectを出す
                {
                    EmitterData data;

                    //炎
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

                    //火の粉
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

void Player::EnemyAttack()
{
    pEnemy_ = (Enemy*)FindObject("Enemy");
    if (pEnemy_ == nullptr)return;
    // 視線ベクトルを取得
    XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

    // レイキャストを発射
    RayCastData sightRay; {
        XMStoreFloat3(&sightRay.dir, sightline);
        sightRay.start = Camera::GetPosition();
        Model::RayCast(pEnemy_->GatModelHandle(), &sightRay);
    }

    if (sightRay.hit && sightRay.dist <= 10.0f) {
        // Effectを出す
        {
            EmitterData data;

            //炎
            data.textureFileName = "Images/cloudA.png";

            XMStoreFloat3(&data.position, XMLoadFloat3(&sightRay.start) + (XMLoadFloat3(&sightRay.dir) * sightRay.dist));
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

            //火の粉
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

        // 攻撃する
        pEnemy_->SetHP(pEnemy_->GetHP() - 20);
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
