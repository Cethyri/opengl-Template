#include "stdafx.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene04.h"
#include "timer.h"
#include "image.h"
#include "light.h"
#include "input.h"

namespace
{
	float vertexData[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f

	};

	const float PI = 3.14159265358f;
}

Scene04::Scene04(Engine* engine) : Scene(engine)
{
}


Scene04::~Scene04()
{
}

//#define SPECULAR
//#define MULTITEXTURE

bool Scene04::Initialize()
{

#if defined (SPECULAR)

	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_specular.vs", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_specular.fs", GL_FRAGMENT_SHADER);

#elif defined (MULTITEXTURE)

	m_shader.CompileShader("..\\Resources\\Shaders\\multi_texture_phong.vs", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\multi_texture_phong.fs", GL_FRAGMENT_SHADER);


#else

	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vs", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.fs", GL_FRAGMENT_SHADER);

#endif // SPECULAR



	m_shader.Link();
	m_shader.Use();

	glGenBuffers(1, m_vboHandle);

	glBindBuffer(GL_ARRAY_BUFFER, *m_vboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexBuffer(0, *m_vboHandle, 0, sizeof(glm::vec3) * 2 + sizeof(glm::vec2));
	glBindVertexBuffer(1, *m_vboHandle, sizeof(glm::vec3), sizeof(glm::vec3) * 2 + sizeof(glm::vec2));
	glBindVertexBuffer(2, *m_vboHandle, sizeof(glm::vec3) * 2, sizeof(glm::vec3) * 2 + sizeof(glm::vec2));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	m_material.SetMaterial(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0f);
	m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", /*"crate",*/ GL_TEXTURE0);
	m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", /*"crate_specular",*/ GL_TEXTURE1);
	m_material.LoadTexture2D("..\\Resources\\Textures\\grass.bmp", /*"crate_specular",*/ GL_TEXTURE2);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	m_engine->Get<Input>()->AddButton("application quit", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);

	Camera* camera = new Camera("camera", this);
	camera->Initialize(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	AddObject(camera);

	Light* light = new Light("light", this);

	light->m_ambient = glm::vec3(0.75f, 0.85f, 1.0f);
	light->m_diffuse = glm::vec3(0.75f, 0.85f, 1.0f);
	light->m_specular = glm::vec3(0.75f, 0.85f, 1.0f);

	Transform lightPosition;

	lightPosition.position = glm::vec3(-10.0f, 10.0f, 10.0f);
	light->m_transform = lightPosition;

	AddObject(light);

	return 1;
}

/*void Scene04::addTexture(const char* fileName, int textureUnit)
{

	int width;
	int height;
	int bpp;

	const unsigned char* data = Image::LoadBMP(fileName, width, height, bpp);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle[textureUnit]);

	GLuint formatStorage = bpp == 32 ? GL_RGBA8 : GL_RGB8;
	GLuint formatImage = bpp == 32 ? GL_BGRA : GL_BGR;
	glTexStorage2D(GL_TEXTURE_2D, 1, formatStorage, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, formatImage, GL_UNSIGNED_BYTE, data);

	delete[] data;
}*/

void Scene04::Update()
{
	if (m_engine->Get<Input>()->GetButton("application quit") == Input::eButtonState::DOWN)
	{
		glfwSetWindowShouldClose(m_engine->Get<Renderer>()->m_window, GLFW_TRUE);
	}

	//auto objects = GetObjects<Object>();
	for (auto object : m_objects)
	{
		object->Update();
	}

	m_shader.SetUniform("material.ambient", m_material.m_ambient);
	m_shader.SetUniform("material.diffuse", m_material.m_diffuse);
	m_shader.SetUniform("material.specular", m_material.m_specular);
	m_shader.SetUniform("material.shininess", m_material.m_shininess);

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_rotation = m_rotation + m_engine->Get<Timer>()->FrameTime() / 8;
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(1.0f, 0.0f, 1.0f));
	glm::mat4 mxModel = translate * rotate;

	Camera* camera = GetObject<Camera>("camera");

	glm::mat4 mxView = camera->GetView();
	glm::mat4 mxProjection = camera->GetProjection();

	glm::mat4 mxModelView = mxView * mxModel;
	m_shader.SetUniform("mxModelView", mxModelView);

	glm::mat4 mxMVP = mxProjection * mxView * mxModel;
	m_shader.SetUniform("mxMVP", mxMVP);

	glm::mat3 mxNormal = glm::mat3(mxModelView);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader.SetUniform("mxNormal", mxNormal);

	Light* light = GetObject<Light>("light");

	m_shader.SetUniform("light.ambient", light->m_ambient);
	m_shader.SetUniform("light.diffuse", light->m_diffuse);
	m_shader.SetUniform("light.specular", light->m_specular);

	glm::vec3 lightPosition = mxView * glm::vec4(light->m_transform.position, 1.0f);
	m_shader.SetUniform("light.position", lightPosition);

}

void Scene04::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_vaoHandle); // will be used for different object groups
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Scene04::Shutdown()
{
}