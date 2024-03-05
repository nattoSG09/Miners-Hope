#include "HPBar.h"
#include "../../../Engine/Image.h"
#include "../../../Engine/Input.h"
HPBar::HPBar(GameObject* parent)
{
}

void HPBar::Initialize()
{
	hpFrame_ = Image::Load("Images/hp_frame.png");
	hpGauge_ = Image::Load("Images/hp_gauge.png");
	transform_.position_ = { -0.8f,0.8f,0.f };
	transform_.scale_ = { 2.0f,0.05f,0.2f };
}

void HPBar::Update()
{
	if (pObject_ != nullptr)transform_.scale_ = { (float)pObject_->GetHP()*0.01f,0.05f ,1.f };
}

void HPBar::Draw()
{
	Image::SetTransform(hpGauge_, transform_);
	Image::Draw(hpGauge_);

	Transform tf;
	tf.position_= { -0.8f,0.8f,0.f };
	tf.scale_ = { 2.0f,0.05f,0.2f };
	Image::SetTransform(hpFrame_, tf);
	Image::Draw(hpFrame_);


}

void HPBar::Release()
{
}
