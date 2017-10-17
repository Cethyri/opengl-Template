#pragma once
#include "scene.h"
#include "glm\glm.hpp"

class Scene02 :
	public Scene
{
public:
	Scene02(Engine* engine);
	~Scene02();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;

private:
	GLuint m_shaderProgram;
	GLuint m_vboHandles[3];
	GLuint m_vaoHandle;

	glm::mat4 m_mxTranslate;

	float m_x;
	float m_y;
	
	glm::vec3 m_cursorScale;
	glm::mat4 m_scale;

};

