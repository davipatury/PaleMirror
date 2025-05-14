#include "Component.h"

#include "GameObject.h"

Component::Component(GameObject& associated) : associated(associated){}

Component::~Component(){}
