#pragma once
#include "object.h"

class Light: public Object
{
public:
	Light(const std::string& name, Scene* scene);
	~Light();

	virtual void Update() override;
public:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

