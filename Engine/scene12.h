#pragma once
#include "scene.h"
#include "material.h"
#include "camera.h"

class Scene12 :
	public Scene
{
public:
	Scene12(Engine* engine);
	~Scene12();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;

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

private:
	GLuint m_depthBuffer = 0;
	Shader* m_depthShader = nullptr;

};

