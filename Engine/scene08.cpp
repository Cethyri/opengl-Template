#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene08.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"
#include "meshLoader.h"
#include "model.h"
#include <glm\gtc\random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

//#define SPOTLIGHT
#define NUM_LIGHTS 5

namespace
{
	const float PI = 3.14159265358f;
}

Scene08::Scene08(Engine* engine) : Scene(engine)
{
}


Scene08::~Scene08()
{
}

//#define SPECULAR
//#define MULTITEXTURE

bool Scene08::Initialize()
{
	m_engine->Get<Input>()->AddButton("mode", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);
	srand((unsigned int)time(0));

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		// light
		Light* light = new Light("light", this);
		light->m_transform.position = glm::sphericalRand(4.0f);
		light->m_diffuse = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 0.5f));
		light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		AddObject(light);
	}
	std::vector<Light*> lights = GetObjects<Light>();

	// model
	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multilight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 100.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	//model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	//model->m_shader.SetUniform("light.specular", light->m_specular);
	//model->m_shader.SetUniform("light.position", light->m_transform.position);
	for (size_t i = 0; i < lights.size(); i++)
	{
		char uniformName[32];
		sprintf_s(uniformName, "lights[%d].diffuse", i);
		model->m_shader.SetUniform(uniformName, lights[i]->m_diffuse);

		sprintf_s(uniformName, "lights[%d].specular", i);
		model->m_shader.SetUniform(uniformName, lights[i]->m_specular);
	}

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
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multilight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 100.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	//model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	//model->m_shader.SetUniform("light.specular", light->m_specular);
	//model->m_shader.SetUniform("light.position", light->m_transform.position);
	for (size_t i = 0; i < lights.size(); i++)
	{
		char uniformName[32];
		sprintf_s(uniformName, "lights[%d].diffuse", i);
		model->m_shader.SetUniform(uniformName, lights[i]->m_diffuse);

		sprintf_s(uniformName, "lights[%d].specular", i);
		model->m_shader.SetUniform(uniformName, lights[i]->m_specular);
	}
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

void Scene08::Update()
{
	if (m_engine->Get<Input>()->GetButton("mode") == Input::eButtonState::DOWN)
	{
		m_pointLightMode = !m_pointLightMode;
	}

	float w = (m_pointLightMode) ? 1.0f : 0.0f;

	Model* model = GetObject<Model>("model");
	Camera* camera = GetObject<Camera>("camera");

	float dt = m_engine->Get<Timer>()->FrameTime();
	m_rotation = 1.0f * dt;

	auto lights = GetObjects<Light>();
	for (size_t i = 0; i < lights.size(); i++)
	{

		glm::quat rotation = glm::angleAxis(m_rotation, /*glm::vec3(0.0f, 1.0f, 0.0f)*/ glm::normalize(camera->m_transform.position));
		lights[i]->m_transform.position = rotation * lights[i]->m_transform.position;

		glm::vec4 position = camera->GetView() * glm::vec4(lights[i]->m_transform.position, w);
		auto models = GetObjects<Model>();
		for (auto model : models)
		{
			model->m_shader.Use();
			char uniformName[32];
			sprintf_s(uniformName, "lights[%d].position", i);
			model->m_shader.SetUniform(uniformName, position);
		}
	}


	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene08::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene08::Shutdown()
{
}