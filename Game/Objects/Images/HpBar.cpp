#include "HpBar.h"
#include "../../../Engine/Image.h"

HpBar::HpBar(GameObject* parent)
	:GameObject(parent,"HpBar")
{
}

void HpBar::Initialize()
{
	hPict_gauge_ = Image::Load("Images/hp_bar.png");
	hPict_gauge_ = Image::Load("Images/hp_frame.png");

}

void HpBar::Update()
{
}

void HpBar::Draw()
{
}

void HpBar::Release()
{
}
