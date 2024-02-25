#include "Circle.h"

Circle::Circle()
	:center_(0.f, 0.f), radius_(1.f) 
{
}

Circle::Circle(float _cX, float _cY, float _radius)
	:center_{ _cX,_cY }, radius_(_radius)
{
}

bool Circle::ContainsPoint(XMFLOAT2 _point)
{
    float a = _point.x - center_.x;
    float b = _point.y - center_.y;
    float c = sqrt(a * a + b * b);

    return c <= radius_;
}

bool Circle::ContainsPoint(float _px, float _py)
{
    return ContainsPoint(XMFLOAT2(_px,_py));
}

bool Circle::OverlapCircle(Circle _circle)
{
    float a = _circle.center_.x - this->center_.x;
    float b = _circle.center_.y - this->center_.y;
    float c = sqrt(a * a + b * b);

    return c <= _circle.radius_ + this->radius_;
}
