#include "stdafx.h"
#include "scene.h"

void Scene::AddObject(Object * object)
{
	m_objects.push_back(object);
}

Scene::~Scene()
{
	for (auto object : m_objects)
	{
		delete object;
	}
}
