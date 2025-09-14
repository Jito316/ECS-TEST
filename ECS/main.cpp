#include <iostream>
#include "ECSManager.h"
#include "Component.h"
#include "ClassInfo.h"
#include "Math.h"

COMPONENT_REFLECTION(GameObject);
COMPONENT_REFLECTION(Transfrom);
COMPONENT_REFLECTION(Rigidbody);

struct Player : IComponent
{
	int hp = 0;
};
COMPONENT_REFLECTION(Player);


int main()
{
	
}
