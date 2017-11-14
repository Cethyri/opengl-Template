#pragma once
#include "scene.h"
#include "material.h"
#include "camera.h"

class Scene04 :
	public Scene
{
public:
	Scene04(Engine* engine);
	~Scene04();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;
private:
	//void addTexture(const char* fileName, int textureUnit);

private:
	//GLuint m_shaderProgram;
	GLuint m_vboHandle[1];
	//GLuint m_vaoHandle;

	Material m_material;

	GLuint m_vaoHandle;

	GLuint m_textureHandle[2];

	float m_rotation;
};

