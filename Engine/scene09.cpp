#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene09.h"
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

Scene09::Scene09(Engine* engine) : Scene(engine)
{
}


Scene09::~Scene09()
{
}

bool Scene09::Initialize()
{
	m_engine->Get<Input>()->AddButton("mode", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);
	srand((unsigned int) time(0));

	// light
	Light* light = new Light("light", this);
	light->m_transform.position = glm::vec3(3.0f, 2.0f, 1.0f);
	light->m_diffuse = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 0.8f));
	light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	AddObject(light);

	// model
	auto model = new Model("model", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");

	model->m_cameraID = "camera_rtt";

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 100.0f;

	model->m_shader.SetUniform("ambientMaterial", model->m_material.m_ambient);
	model->m_shader.SetUniform("diffuseMaterial", model->m_material.m_diffuse);
	model->m_shader.SetUniform("specularMaterial", model->m_material.m_specular);

	model->m_shader.SetUniform("lightColor", light->m_diffuse);
	model->m_shader.SetUniform("lightPosition", light->m_transform.position);

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);


	model = new Model("cube", this);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	model->m_transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	model->m_mesh.Load("..\\Resources\\Meshes\\cube.obj");

	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.fs", GL_FRAGMENT_SHADER);
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

	model->m_shader.SetUniform("light.diffuse", light->m_diffuse);
	model->m_shader.SetUniform("light.specular", light->m_specular);
	model->m_shader.SetUniform("light.position", light->m_transform.position);

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);


	GLuint texture = Material::CreateTexture(512, 512);
	model->m_material.AddTexture(GL_TEXTURE_2D, texture, GL_TEXTURE0);

	AddObject(model);

	// camera
	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::ORBIT;
	camera->Initialize(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);

	camera = new Camera("camera_rtt", this);
	data.type = Camera::eType::LOOK_AT;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);


	m_rotation = 0.0f;


	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	return true;
}

void Scene09::Update()
{
	Light* light = GetObject<Light>("light");

	float dt = m_engine->Get<Timer>()->FrameTime();


	Camera* camera = GetObject<Camera>("camera_rtt");
	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.position, 1.0f);

	Model* model = GetObject<Model>("model");
	m_rotation = m_rotation + 1.0f * dt;
	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	model->m_transform.rotation = rotation;

	model->m_shader.Use();
	model->m_shader.SetUniform("lightPosition", position);


	camera = GetObject<Camera>("camera");
	position = camera->GetView() * glm::vec4(light->m_transform.position, 1.0f);

	model = GetObject<Model>("cube");
	model->m_shader.Use();
	model->m_shader.SetUniform("light.position", position);


	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}
}

void Scene09::Render()
{
	glViewport(0, 0, 512, 512);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderable = GetObject<Renderable>("model");
	renderable->Render();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->m_width, m_engine->Get<Renderer>()->m_height);
	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderable = GetObject<Renderable>("cube");
	renderable->Render();
	renderable = GetObject<Renderable>("light");
	renderable->Render();

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene09::Shutdown()
{
}