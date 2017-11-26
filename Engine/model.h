#pragma once
#include "renderable.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"

class Model :
	public Renderable
{
public:
	Model(const std::string& name, Scene* scene);
	~Model();

	virtual void Update() override;

	virtual void Render() override;

public:
	Mesh m_mesh;
	Shader m_shader;
	Material m_material;
	std::string m_cameraID;
};

