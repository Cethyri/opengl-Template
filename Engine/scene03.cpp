#include "stdafx.h"
#include "scene01.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "scene03.h"
#include "timer.h"

#define PHONG

namespace
{
	float vertexData[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	enum vboID
	{
		POSITION,
		COLOR,
		VERTEX
	};

	const float PI = 3.14159265358f;
}

Scene03::Scene03(Engine* engine) : Scene(engine)
{
}


Scene03::~Scene03()
{
}

bool Scene03::Initialize()
{

#ifdef PHONG
	m_cube.shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\phong.vs", "..\\Resources\\Shaders\\phong.fs");
#else
	m_cube.shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\gouraud.vs", "..\\Resources\\Shaders\\basic.fs");
#endif

	glGenBuffers(1, m_vboHandle);

	glBindBuffer(GL_ARRAY_BUFFER, *m_vboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_cube.vaoHandle);
	glBindVertexArray(m_cube.vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(0, *m_vboHandle, 0, sizeof(glm::vec3) * 2);
	glBindVertexBuffer(1, *m_vboHandle, sizeof(glm::vec3), sizeof(glm::vec3) * 2);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	m_cube.mxModelViewUniform = glGetUniformLocation(m_cube.shaderProgram, "mxModelView");
	m_cube.mxMVPUniform = glGetUniformLocation(m_cube.shaderProgram, "mxMVP");
	m_cube.mxNormalUniform = glGetUniformLocation(m_cube.shaderProgram, "mxNormal");

	m_cube.ambientMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "ambientMaterial");
	m_cube.diffuseMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "diffuseMaterial");
	m_cube.specularMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "specularMaterial");


	m_light.positionUniform = glGetUniformLocation(m_cube.shaderProgram, "lightPosition");
	m_light.colorUniform = glGetUniformLocation(m_cube.shaderProgram, "lightColor");


	return 1;
}

void Scene03::Update()
{
	glm::vec3 ambientMaterial = glm::vec3(0.2f, 0.2f, 0.2f);
	glUniform3fv(m_cube.ambientMaterialUniform, 1, &ambientMaterial[0]);

	glm::vec3 diffuseMaterial = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(m_cube.diffuseMaterialUniform, 1, &diffuseMaterial[0]);

	glm::vec3 specularMaterial = glm::vec3(1.0f, 0.5f, 0.7f);
	glUniform3fv(m_cube.specularMaterialUniform, 1, &specularMaterial[0]);

	m_rotation = m_rotation + m_engine->Get<Timer>()->FrameTime();
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 mxModel = translate * rotate;

	glm::mat4 mxView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxProjection = glm::perspective<float>(90.0, (float) m_engine->Get<Renderer>()->m_width / (float) m_engine->Get<Renderer>()->m_height, 0.0001f, 10000.0f);

	glm::mat4 mxModelView = mxView * mxModel;
	glUniformMatrix4fv(m_cube.mxModelViewUniform, 1, GL_FALSE, &mxModelView[0][0]);

	glm::mat4 mxMVP = mxProjection * mxView * mxModel;
	glUniformMatrix4fv(m_cube.mxMVPUniform, 1, GL_FALSE, &mxMVP[0][0]);

	glm::mat3 mxNormal = glm::mat3(mxModelView);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	glUniformMatrix3fv(m_cube.mxNormalUniform, 1, GL_FALSE, &mxNormal[0][0]);

	glm::vec3 lightPosition = mxView * glm::vec4(2.0f, 10.0f, 10.0f, 1.0f);
	glUniform4fv(m_light.positionUniform, 1, &lightPosition[0]);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(m_light.colorUniform, 1, &lightColor[0]);
}

void Scene03::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_cube.vaoHandle); // will be used for different object groups
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Scene03::Shutdown()
{
}