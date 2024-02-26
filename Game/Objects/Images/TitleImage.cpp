#include "TitleImage.h"

#include "../../../Engine/Image.h"

TitleImage::TitleImage(GameObject* parent)
	:GameObject(parent,"TitleScene")
{
}

void TitleImage::Initialize()
{
	hPict_ = Image::Load("Images/title_color02.png");
	transform_.scale_ = { 0.6f,0.6f, 1 };
	transform_.position_.y += 0.3f;
}

void TitleImage::Update()
{
}

void TitleImage::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void TitleImage::Release()
{
}
