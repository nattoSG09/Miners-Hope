#include "LivingObject.h"

LivingObject::LivingObject(GameObject* parent, std::string name)
	:GameObject(parent,name),hModel_(-1)
{
}
