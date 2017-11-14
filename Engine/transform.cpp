#include "stdafx.h"
#include "transform.h"
#include "glm\gtc\matrix_transform.hpp"

Transform::Transform()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::quat(glm::vec3(0.0f));
	scale = glm::vec3(1.0f);

}

Transform::~Transform()
{
}

glm::mat4 Transform::GetMatrix44()
{
	glm::mat4 mxt = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 mxr = glm::mat4_cast(rotation);
	glm::mat4 mxs = glm::scale(glm::mat4(1.0f), scale);

	return mxt * (mxr * mxs);
}