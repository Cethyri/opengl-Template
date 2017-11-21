#pragma once
#include "renderable.h"
#include "shader.h"
#include "mesh.h"

class Light: public Renderable
{
public:
	Light(const std::string& name, Scene* scene);
	~Light();

	virtual void Update() override;
	virtual void Render() override;
public:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

protected:
	Shader m_shader;
	Mesh m_mesh;

};

