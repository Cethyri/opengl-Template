#include "stdafx.h"
#include "light.h"
#include "camera.h"
#include "scene.h"

Light::Light(const std::string& name, Scene* scene) : Renderable(name, scene)
{
	m_mesh.Load("..\\Resources\\Meshes\\cube.obj");
	m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	m_transform.scale = glm::vec3(0.2f);

	m_shader.CompileShader("..\\Resources\\Shaders\\basic_color.vs", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\basic.fs", GL_FRAGMENT_SHADER);
	m_shader.Link();
	m_shader.Use();

}

Light::~Light()
{
}

void Light::Update()
{
	m_shader.Use();

	Camera* camera = m_scene->GetObject<Camera>("camera");
	glm::mat4 mxMVP = camera->GetProjection() * camera->GetView() * m_transform.GetMatrix44();
	m_shader.SetUniform("mxMVP", mxMVP);
	m_shader.SetUniform("color", m_diffuse);
}

void Light::Render()
{
	m_shader.Use();
	m_mesh.Render();
}
