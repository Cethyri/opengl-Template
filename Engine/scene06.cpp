#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene06.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"
#include "meshLoader.h"
#include "model.h"

namespace
{
	const float PI = 3.14159265358f;
}

Scene06::Scene06(Engine* engine) : Scene(engine)
{
}


Scene06::~Scene06()
{
}

//#define SPECULAR
//#define MULTITEXTURE

bool Scene06::Initialize()
{
	// light
	Light* light = new Light("light", this);
	light->m_transform.position = glm::vec3(2.0f, 2.0f, 10.0f);
	light->m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	AddObject(light);

	// model
	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_fog.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_fog.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.75f, 0.75f, 0.75f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 0.4f * 128.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	model->m_shader.SetUniform("fog.distanceMin", 1.0f);
	model->m_shader.SetUniform("fog.distanceMax", 50.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));

	model->m_mesh.Load("..\\Resources\\Meshes\\cube.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);


	AddObject(model);

	// camera
	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::ORBIT;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);

	return true;
}

void Scene06::Update()
{

	Model* model = GetObject<Model>("model");
	Camera* camera = GetObject<Camera>("camera");
	Light* light = GetObject<Light>("light");

	glm::vec3 position = camera->GetView() * glm::vec4(light->m_transform.position, 1.0f);
	model->m_shader.Use();
	model->m_shader.SetUniform("light.position", position);

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene06::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene06::Shutdown()
{
}