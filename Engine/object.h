#pragma once
#include "transform.h"

class Scene;
class Object
{
public:
	Object(std::string name, Scene* scene);
	~Object();

	virtual void Update() = 0;

public:
	std::string m_name;
	Transform m_transform;

protected:
	Scene* m_scene;

};