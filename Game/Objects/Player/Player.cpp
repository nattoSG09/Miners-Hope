#include "Player.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Model.h"
#include "../../../Engine/Camera.h"
#include "../../../Engine/SceneManager.h"

#include "../../Others/Rect.h"
#include "../Stage/Stage.h"
#include "../Ores/OreManager.h"

namespace {
    const float sensitivity = 0.5f;// マウス感度

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
}

void Player::Update()
{
    // カメラの位置を操作
    CalcCameraMove();

    // キャラクターの位置を操作
    Move();

    // 採掘操作
    Mining();

    // キャラクターの向きを設定
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
    // プレイヤーの頭部の位置を設定
    XMFLOAT3 playerHead_position = transform_.position_;
    playerHead_position.y += 4.f;

    // マウスの情報を取得
    XMFLOAT3 mouseMove = Input::GetMouseMove();

    // カメラの位置の回転
    XMFLOAT3 camera_position = Camera::GetPosition();
    {

        // 正規化済みの向きベクトルを用意
        XMVECTOR player_To_camPos = XMLoadFloat3(&camera_position) - XMLoadFloat3(&playerHead_position);
        player_To_camPos = XMVector3Normalize(player_To_camPos);

        // 回転行列をマウスの移動量を基に作成
        XMMATRIX matRotate =
            XMMatrixRotationX(XMConvertToRadians(mouseMove.y * sensitivity)) * XMMatrixRotationY(XMConvertToRadians(mouseMove.x * sensitivity));

        // 回転行列を掛けて、向きベクトルを回転
        player_To_camPos = XMVector3Transform(player_To_camPos, matRotate);

        // 長さを変更
        player_To_camPos *= 10.f;

        // 原点０，０から回転後のカメラの位置に伸びるベクトルを作成し、位置に代入
        XMVECTOR origin_To_camPos = player_To_camPos + XMLoadFloat3(&playerHead_position);
        XMStoreFloat3(&camera_position, origin_To_camPos);

    }

    // カメラの焦点の回転
    XMFLOAT3 camera_target = Camera::GetTarget();
    {

        // 正規化済みの向きベクトルを用意
        XMVECTOR player_To_camPos = XMLoadFloat3(&camera_position) - XMLoadFloat3(&playerHead_position);
        player_To_camPos = XMVector3Normalize(player_To_camPos);

        // 回転行列をマウスの移動量を基に作成
        XMMATRIX matRotate =
            XMMatrixRotationX(XMConvertToRadians(mouseMove.y * sensitivity) + 45.f) * XMMatrixRotationY(XMConvertToRadians(mouseMove.x * sensitivity) + 45.f);

        // 回転行列を掛けて、向きベクトルを回転
        XMVECTOR player_To_camTgt = XMVector3Transform(player_To_camPos, matRotate);

        // 長さを変更
        player_To_camTgt *= 10.f;

        // 原点０，０から回転後のカメラの位置に伸びるベクトルを作成し、位置に代入
        XMVECTOR origin_To_camTgt = XMLoadFloat3(&playerHead_position) - player_To_camTgt;
        XMStoreFloat3(&camera_target, origin_To_camTgt);

    }

    Camera::SetPosition(camera_position);

    // 焦点が頭部の時はうまくいく。
    Camera::SetTarget(playerHead_position);
    //Camera::SetTarget(camera_target);

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
