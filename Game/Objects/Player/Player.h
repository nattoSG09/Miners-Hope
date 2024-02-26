#pragma once
#include "../../../Engine/GameObject.h"

class Stage;

class Player : public GameObject
{
	int hModel_;
	Stage* pStage_;

	XMFLOAT3 cameraRotate_;

public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsExitCave();
private:
	void Move();
	void CalcCameraMove();
	void Mining();
};

