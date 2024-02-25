#pragma once
#include <DirectXMath.h>
using namespace DirectX;


class Rect
{
public:
	XMFLOAT2 center_;
	float width_;
	float height_;

	Rect();
	Rect(float _cx, float _cy, float _width, float _height);
	Rect(XMFLOAT2 _center, float _width, float _height);

	bool ContainsPoint(float _px, float _py);
	bool ContainsPoint(XMFLOAT2 _point);
};

