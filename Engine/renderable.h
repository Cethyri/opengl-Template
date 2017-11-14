#pragma once
#include "object.h"
#include "material.h"
#include "shader.h"

class Renderable :
	public Object
{
public:
	Renderable(const std::string& name, Scene* scene);
	~Renderable();

	virtual void Update() override;

protected:
	Shader m_shader;
	Material m_material;
};

