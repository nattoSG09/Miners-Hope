#include "OreManager.h"

namespace OreManager {
	std::vector<Ore*> ores_;
}

void OreManager::CreateOre(Ore_type _type, XMFLOAT3 _position, float _scale,GameObject* parent)
{
	Ore* ore = Instantiate<Ore>(parent);
	ore->SetPosition(_position);
	ore->SetType(_type);
	ore->SetScale(_scale, _scale, _scale);
	ore->SetDurability(_type * _scale);
	ores_.push_back(ore);
}

void OreManager::Destroy(Ore* _ore)
{
	auto it = std::find(ores_.begin(), ores_.end(), _ore);
	_ore->KillMe();
	ores_.erase(it);
}

void OreManager::AllRelease()
{
	ores_.clear(); // ores_‚ð‹ó‚É‚·‚é
}
