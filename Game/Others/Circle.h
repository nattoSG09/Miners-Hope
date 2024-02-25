#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Circle
{
public:
    XMFLOAT2 center_;
    float radius_;

public:
    Circle();
    Circle(float _cX, float _cY, float _radius);

    bool ContainsPoint(XMFLOAT2 _point);
    bool ContainsPoint(float _px,float _py);
    bool OverlapCircle(Circle _circle);
};

