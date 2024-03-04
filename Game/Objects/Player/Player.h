#pragma once
#include "../../../Engine/GameObject.h"
#include "../../../Engine/VFX.h"

#include "../Ores/OreManager.h"

class Stage;
class Pickaxe;
class Enemy;

struct Inventory
{
	int num_Iron;
	int num_Gold;
	int num_Diamond;
	int num_Emerald;
	int num_Ruby;

	void Load(std::string _filePath);
	void Save(std::string _filePath);
};

class Player : public GameObject
{
	int hModel_;
	Stage* pStage_;
	Enemy* pEnemy_;

	Inventory myInventory_;
	Pickaxe* myPickaxe_;
	XMFLOAT2 angle_;
	int hPoint_;

	// debug 
	XMFLOAT3 center_;
	XMFLOAT3 newCenter_;
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
	void EnemyAttack();
};

