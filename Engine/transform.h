#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

class Transform
{
public:
	Transform();
	~Transform();

	glm::mat4 GetMatrix44();

public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

