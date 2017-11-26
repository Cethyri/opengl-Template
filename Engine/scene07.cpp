#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene07.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"
#include "meshLoader.h"
#include "model.h"

#define SPOTLIGHT

namespace
{
	const float PI = 3.14159265358f;
}

Scene07::Scene07(Engine* engine) : Scene(engine)
{
}


Scene07::~Scene07()
{
}

//#define SPECULAR
//#define MULTITEXTURE

bool Scene07::Initialize()
{
	m_engine->Get<Input>()->AddButton("mode", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);

	// light
	Light* light = new Light("light", this);
	light->m_transform.position = glm::vec3(0.0f, 0.0f, 2.0f);
	light->m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	AddObject(light);

	// model
	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.75f, 0.75f, 0.75f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 12.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);
#ifdef SPOTLIGHT
	model->m_shader.SetUniform("light.cutoff", glm::radians(45.0f));
	model->m_shader.SetUniform("light.exponent", 30.0f);
#endif 

	//model->m_shader.SetUniform("fog.distanceMin", 5.0f);
	//model->m_shader.SetUniform("fog.distanceMax", 20.0f);
	//model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);


	model = new Model("plane", this);
	model->m_transform.scale = glm::vec3(20.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, -4.0f, 0.0f);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.15f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.7f, 0.75f, 0.75f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 12.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);
#ifdef SPOTLIGHT
	model->m_shader.SetUniform("light.cutoff", glm::radians(45.0f));
	model->m_shader.SetUniform("light.exponent", 30.0f);
#endif 

	//model->m_shader.SetUniform("fog.distanceMin", 5.0f);
	//model->m_shader.SetUniform("fog.distanceMax", 50.0f);
	//model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));

	model->m_mesh.Load("..\\Resources\\Meshes\\plane.obj");

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

	m_rotation = 0.0f;

	return true;
}

void Scene07::Update()
{
	if (m_engine->Get<Input>()->GetButton("mode") == Input::eButtonState::DOWN)
	{
		m_pointLightMode = !m_pointLightMode;
	}

	float w = (m_pointLightMode) ? 1.0f : 0.0f;

	Model* model = GetObject<Model>("model");
	Camera* camera = GetObject<Camera>("camera");
	Light* light = GetObject<Light>("light");

	float dt = m_engine->Get<Timer>()->FrameTime();

	m_rotation = m_rotation + 1.0f * dt;
	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	light->m_transform.position = rotation * glm::vec3(0.0f, 5.0f, 1.5f);

	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.position, w);

	glm::mat3 viewDirectionMatrix = glm::mat3(camera->GetView());
	//glm::vec3 direction = glm::angleAxis(cos(m_rotation * 2) / 3, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::angleAxis(sin(m_rotation * 3) / 2, glm::vec3(1.0f, 0.0f,0.0f)) * glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 direction = viewDirectionMatrix * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	auto models = GetObjects<Model>();
	for (auto model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", position);
		model->m_shader.SetUniform("light.direction", direction);
	}

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene07::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene07::Shutdown()
{
}