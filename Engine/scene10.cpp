#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene10.h"
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

Scene10::Scene10(Engine* engine) : Scene(engine)
{
}


Scene10::~Scene10()
{
}

//#define SPECULAR
//#define MULTITEXTURE

bool Scene10::Initialize()
{

	// model
	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\puma.obj");
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	GLuint modelIndex = glGetSubroutineIndex(model->m_shader.GetHandle(), GL_VERTEX_SHADER, "reflection");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &modelIndex);

	//model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	//model->m_material.m_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
	//model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//model->m_material.m_shininess = 100.0f;

	//model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	//model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	//model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	//model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	//model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	//model->m_shader.SetUniform("light.specular", light->m_specular);
	//model->m_shader.SetUniform("light.position", light->m_transform.position);

	AddObject(model);


	model = new Model("skybox", this);
	model->m_transform.scale = glm::vec3(80.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\cube.obj");
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);

	std::vector<std::string> suffixes = { "_posx", "_negx", "_posy", "_negy", "_posz", "_negz" };
	model->m_material.LoadTextureCube("..\\Resources\\Textures\\lancellotti", suffixes, "jpg", GL_TEXTURE0);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	GLuint skyboxIndex = glGetSubroutineIndex(model->m_shader.GetHandle(), GL_VERTEX_SHADER, "skybox");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &skyboxIndex);

	//model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	//model->m_material.m_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
	//model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//model->m_material.m_shininess = 100.0f;

	//model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	//model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	//model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	//model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	//model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	//model->m_shader.SetUniform("light.specular", light->m_specular);
	//model->m_shader.SetUniform("light.position", light->m_transform.position);

	AddObject(model);


	// camera
	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);

	return true;
}

void Scene10::Update()
{

	Model* model = GetObject<Model>("model");
	Camera* camera = GetObject<Camera>("camera");

	float dt = m_engine->Get<Timer>()->FrameTime();

	//glm::quat rotation = glm::angleAxis(dt, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::quat rotation = glm::angleAxis(dt, glm::normalize(camera->m_transform.position));
	//model->m_transform.rotation = rotation * model->m_transform.rotation;

	auto models = GetObjects<Model>();
	for (auto model : models)
	{
		glm::mat4 mxModel = model->m_transform.GetMatrix44();
		model->m_shader.Use();
		model->m_shader.SetUniform("mxModel", mxModel);
		model->m_shader.SetUniform("cameraWorldPosition", camera->m_transform.position);
	}


	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene10::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene10::Shutdown()
{
}