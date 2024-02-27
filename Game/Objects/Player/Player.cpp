#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"

#include "../../Others/Rect.h"
#include "../Stage/Stage.h"
#include "../Ores/OreManager.h"

namespace {
    const float SENSITIVITY = 0.5f;// マウス感度
    const float PLAYER_TO_CAMERA_DISTANCE = 10.f;

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
	hModel_ = Model::Load("Models/Player/Walking.fbx");
	assert(hModel_ >= 0);

	pStage_ = (Stage*)FindObject("Stage");
    
}

void Player::Update()
{
    // カメラの位置を操作
    CalcCameraMove();

    // キャラクターの位置を操作
    Move();
   
    // 採掘操作
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
    // アニメーションがされているかどうか
    bool isAnim = false;

    // プレイヤーの速度
    float speed = 0.1f;

    // 正規化した視線ベクトルを取得
    XMVECTOR moveDir = XMVector3Normalize(Camera::GetSightline());

    // Y方向への移動を制限したいので、Y要素を０にする
    moveDir = XMVectorSetY(moveDir, 0);

    // スピードを乗算
    moveDir *= speed;

    // 移動方向ベクトルを用意
    XMVECTOR move{ 0,0,0,0 };

    // 「Ｗ」キーが押されたら...
    if (Input::IsKey(DIK_W)) {

        // 画面前方に進む
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // アニメーションを動作させる
        isAnim = true;
    }

    // 「Ａ」キーが押されたら...
    if (Input::IsKey(DIK_A)) {

        // 画面右方に進む
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // アニメーションを動作させる
        isAnim = true;
    }

    // 「Ｓ」キーが押されたら...
    if (Input::IsKey(DIK_S)) {

        // 画面後方に進む
        move = XMLoadFloat3(&transform_.position_) - moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // アニメーションを動作させる
        isAnim = true;
    }

    // 「Ｄ」キーが押されたら...
    if (Input::IsKey(DIK_D)) {

        // 画面左方に進む
        moveDir = XMVector3Transform(moveDir, XMMatrixRotationY(XMConvertToRadians(90)));
        move = XMLoadFloat3(&transform_.position_) + moveDir;
        XMStoreFloat3(&transform_.position_, move);

        // アニメーションを動作させる
        isAnim = true;
    }

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
    // マウスの移動量から回転量を取得
    XMFLOAT3 mouseMove = Input::GetMouseMove();
    cameraRotate_.x += mouseMove.x * SENSITIVITY;
    cameraRotate_.y += mouseMove.y * SENSITIVITY;

    // 初期値を示すベクトルを用意（ｚ＋方向ベクトル）
    XMVECTOR sightDirection = { 0.f,0.f,1.f,0.f };

    XMFLOAT3 tmpCameraRotate = {cameera}
    
}

void Player::Mining()
{
    ImGui::Begin("miningData");

    int i = 0;
    for (auto& ore : OreManager::ores_) {
        // 視線ベクトルを取得
        XMVECTOR sightline = XMVector3Normalize(Camera::GetSightline());

        // レイキャストを発射
        RayCastData sightRay; {
            XMStoreFloat3(&sightRay.dir, sightline);
            sightRay.start = Camera::GetPosition();
            Model::RayCast(ore->GetModelHandle(), &sightRay);

            // debug
            ImGui::Text("Ore[%d].sightRay.hit = %s",i,sightRay.hit ? "true" : "false");
        }

        // 採掘可能かどうか
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
