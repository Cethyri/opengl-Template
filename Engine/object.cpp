#include "stdafx.h"
#include "object.h"


Object::Object(std::string name, Scene* scene)
{
	m_name = name;
	m_scene = scene;
}


Object::~Object()
{
}
