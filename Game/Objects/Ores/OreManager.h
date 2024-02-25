#pragma once
#include <DirectXMath.h>



#include "../../../Engine/GameObject.h"
#include "../Ores/Ore.h"
#include <vector>

using namespace DirectX;

namespace OreManager
{
	extern std::vector<Ore*> ores_;

	void CreateOre(Ore_type _type, XMFLOAT3 _position,float _scale,GameObject* parent);
	void Destroy(Ore* _ore);
	void AllRelease();
};

