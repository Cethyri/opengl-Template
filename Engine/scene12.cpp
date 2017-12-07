#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene12.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"
#include "meshLoader.h"
#include "model.h"
#include <glm\gtc\random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

#define SHADOW_BUFFER_WIDTH	1024
#define SHADOW_BUFFER_HEIGHT 1024

namespace
{
	const float PI = 3.14159265358f;
}

Scene12::Scene12(Engine* engine) : Scene(engine)
{
}


Scene12::~Scene12()
{
}

bool Scene12::Initialize()
{
	// light
	Light* light = new Light("light", this);
	light->m_transform.position = glm::vec3(2.0f, 2.0f, 3.0f);
	//light->m_diffuse = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 0.8f));
	light->m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	AddObject(light);


	m_depthShader = new Shader();
	m_depthShader->CompileShader("..\\Resources\\Shaders\\shadow_depth.vs", GL_VERTEX_SHADER);
	m_depthShader->CompileShader("..\\Resources\\Shaders\\shadow_depth.fs", GL_FRAGMENT_SHADER);
	m_depthShader->Link();

	GLuint depthTexture = Material::CreateDepthTexture(SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);

	// model

	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.25f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	model->m_material.m_specular = glm::vec3(1.0f);
	model->m_material.m_shininess = 100.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\brick.png", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	AddObject(model);


	model = new Model("floor", this);
	model->m_transform.scale = glm::vec3(8.0f);
	model->m_transform.position = glm::vec3(0.0f, -4.0f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\plane.obj");
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	model->m_material.m_specular = glm::vec3(1.0f);
	model->m_material.m_shininess = 100.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\brick.png", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	AddObject(model);


	model = new Model("debug", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\quad.obj");
	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::TEXCOORD);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\shadow_depth_debug.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\shadow_depth_debug.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	model->m_material.m_specular = glm::vec3(1.0f);
	model->m_material.m_shininess = 100.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	model->m_material.AddTexture(GL_TEXTURE_2D, GL_TEXTURE0, depthTexture);

	AddObject(model);


	m_depthBuffer = Material::CreateDepthbuffer(depthTexture, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);


	// camera
	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);


	return true;
}

void Scene12::Update()
{
	if (m_engine->Get<Input>()->GetButton("quit") == Input::eButtonState::DOWN)
	{
		glfwSetWindowShouldClose(m_engine->Get<Renderer>()->m_window, GLFW_TRUE);
	}


	Light* light = GetObject<Light>("light");
	Camera* camera = GetObject<Camera>("camera");


	glm::mat4 mxLightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f); glm::mat4 mxLightView = glm::lookAt(light->m_transform.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mxVP = mxLightProjection * mxLightView;

	m_depthShader->Use();
	m_depthShader->SetUniform("mxLVP", mxVP);


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

	}

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene12::Render()
{
	glViewport(0, 0, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_depthShader->Use();

	auto model = GetObject<Model>("model");
	m_depthShader->SetUniform("mxModel", model->m_transform.GetMatrix44());
	model->m_mesh.Render();

	model = GetObject<Model>("floor");
	m_depthShader->SetUniform("mxModel", model->m_transform.GetMatrix44());
	model->m_mesh.Render();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->m_width, m_engine->Get<Renderer>()->m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model = GetObject<Model>("debug");
	model->Render();

	auto renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
	renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);
}

void Scene12::Shutdown()
{
}