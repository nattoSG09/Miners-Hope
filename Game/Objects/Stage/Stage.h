#pragma once
#include "../../../Engine/GameObject.h"
#include "../../Others/Circle.h"
#include <vector>


enum Cave_type {
	Cave01, Cave02, Cave03, Cave04,Cave05, CaveMax
};

class Stage : public GameObject
{
	std::vector<int> hModels_;
	Cave_type type_;

	Circle circle_;
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release();

	Cave_type GetCaveType() { return type_; }
	void SetCaveType(Cave_type _type) { type_ = _type; }
	Circle GetCaveCircle() { return circle_; }
private:
};

template<class T>T* CreateStage(Cave_type _type, GameObject* parent) {
	T* stage = Instantiate<T>(parent);
	stage->SetCaveType(_type);

	return stage;
}

