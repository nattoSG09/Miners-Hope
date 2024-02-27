#include "CrossHair.h"
#include "../../../Engine/Image.h"

CrossHair::CrossHair(GameObject* parent)
	:GameObject(parent,"CrossHair")
{
}

void CrossHair::Initialize()
{
	hPict_ = Image::Load("Images/crossHair.png");
	transform_.scale_ = { 0.8f,0.8f,1.f };
}

void CrossHair::Update()
{
}

void CrossHair::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void CrossHair::Release()
{
}
