#pragma once
#include "scene.h"
#include "material.h"
#include "camera.h"

class Scene05 :
	public Scene
{
public:
	Scene05(Engine* engine);
	~Scene05();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;
private:
	//void addTexture(const char* fileName, int textureUnit);

private:
	GLuint m_vboHandle[3];

	GLuint m_numVertices;

	GLuint m_vaoHandle;

	std::vector<glm::vec3> m_vertices;

	std::vector<glm::vec3> m_normals;

	std::vector<glm::vec2> m_uvs;

	Material m_material;

	GLuint m_textureHandle[2];

	float m_rotation;
};

