#include "core/Component.h"

#include "core/GameObject.h"

Component::Component(GameObject& associated) : associated(associated){}

Component::~Component(){}
