#include "stdafx.h"
#include "scene01.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene04.h"
#include "timer.h"
#include "image.h"

#define PHONG

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

bool Scene04::Initialize()
{
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vs", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.fs", GL_FRAGMENT_SHADER);
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


	glGenTextures(3, m_textureHandle);
	addTexture();
	addTexture();
	addTexture();
	/*data = Image::LoadBMP("C:\\Users\\Christopher\\Desktop\\OneDrive - Neumont University\\Code\\C++\\Gat350\\Engine\\Resources\\Textures\\crate_specular.bmp", width, height, bpp);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle[1]);

	formatStorage = bpp == 32 ? GL_RGBA8 : GL_RGB8;
	formatImage = bpp == 32 ? GL_BGRA : GL_BGR;
	glTexStorage2D(GL_TEXTURE_2D, 1, formatStorage, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, formatImage, GL_UNSIGNED_BYTE, data);

	delete[] data;
	data = Image::LoadBMP("C:\\Users\\Christopher\\Desktop\\OneDrive - Neumont University\\Code\\C++\\Gat350\\Engine\\Resources\\Textures\\TestNormals.bmp", width, height, bpp);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle[2]);

	formatStorage = bpp == 32 ? GL_RGBA8 : GL_RGB8;
	formatImage = bpp == 32 ? GL_BGRA : GL_BGR;
	glTexStorage2D(GL_TEXTURE_2D, 1, formatStorage, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, formatImage, GL_UNSIGNED_BYTE, data);

	delete[] data;*/

	return 1;
}

void Scene04::addTexture(const char* fileName, int textureUnit)
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
}

void Scene04::Update()
{
	glm::vec3 ambientMaterial = glm::vec3(0.2f, 0.2f, 0.2f);
	m_shader.SetUniform("ambientMaterial", ambientMaterial);

	glm::vec3 diffuseMaterial = glm::vec3(0.8f, 0.8f, 0.8f);
	m_shader.SetUniform("diffuseMaterial", diffuseMaterial);

	glm::vec3 specularMaterial = glm::vec3(1.0f, 1.0f, 1.0f);
	m_shader.SetUniform("specularMaterial", specularMaterial);

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_rotation = m_rotation + m_engine->Get<Timer>()->FrameTime() / 8;
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(1.0f, 0.0f, 1.0f));
	glm::mat4 mxModel = translate * rotate;



	glm::mat4 mxView = glm::lookAt(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxProjection = glm::perspective<float>(90.0, (float) m_engine->Get<Renderer>()->m_width / (float) m_engine->Get<Renderer>()->m_height, 0.0001f, 10000.0f);

	glm::mat4 mxModelView = mxView * mxModel;
	m_shader.SetUniform("mxModelView", mxModelView);

	glm::mat4 mxMVP = mxProjection * mxView * mxModel;
	m_shader.SetUniform("mxMVP", mxMVP);

	glm::mat3 mxNormal = glm::mat3(mxModelView);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader.SetUniform("mxNormal", mxNormal);

	glm::vec3 lightPosition = mxView * glm::vec4(-10.0f, 10.0f, 10.0f, 1.0f);
	m_shader.SetUniform("lightPosition", lightPosition);

	glm::vec3 lightColor = glm::vec3(0.8f, 0.9f, 1.0f);
	m_shader.SetUniform("lightColor", lightColor);

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