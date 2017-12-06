#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene11.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"
#include "meshLoader.h"
#include "model.h"
#include <glm\gtc\random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

namespace
{
	const float PI = 3.14159265358f;
}

Scene11::Scene11(Engine* engine) : Scene(engine)
{
}


Scene11::~Scene11()
{
}

bool Scene11::Initialize()
{

	// light
	Light* light = new Light("light", this);
	light->m_transform.position = glm::vec3(0.0f, 2.0f, 2.0f);
	//light->m_diffuse = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 0.8f));
	light->m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	AddObject(light);

	// model

	auto model = new Model("ogre", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\ogre.obj", true);
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);
	model->m_mesh.BindVertexAttrib(3, Mesh::eVertexType::TANGENT);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\normal_phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\normal_phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	model->m_material.m_specular = glm::vec3(1.0f);
	model->m_material.m_shininess = 100.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\ogre_diffuse.bmp", GL_TEXTURE0);
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\ogre_normal.bmp", GL_TEXTURE1);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	AddObject(model);

	// camera
	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);

	return true;
}

void Scene11::Update()
{
	Light* light = GetObject<Light>("light");
	Camera* camera = GetObject<Camera>("camera");

	float dt = m_engine->Get<Timer>()->FrameTime();
	m_rotation = dt;

	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f)/* glm::normalize(camera->m_transform.position)*/);
	light->m_transform.position = rotation * light->m_transform.position;

	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.position, 1.0f);


	auto models = GetObjects<Model>();
	for (auto model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", position);
		model->m_shader.SetUniform("lightPosition", position);

	}

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene11::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene11::Shutdown()
{
}