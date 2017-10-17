#include "stdafx.h"
#include "scene02.h"
#include "renderer.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "input.h"

namespace
{
	glm::vec3 vPositionData[] =
	{
		glm::vec3(-0.8f, -0.8f, 0.0f),
		//glm::vec3(0.0f,  -0.6f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		glm::vec3(0.0f,  0.8f, 0.0f),
	};

	glm::vec3 vColorData[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		//glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	enum vboID
	{
		POSITION,
		COLOR,
		VERTEX
	};
}

Scene02::Scene02(Engine* engine) : Scene(engine)
{
}


Scene02::~Scene02()
{
}

bool Scene02::Initialize()
{
	glfwSetInputMode(m_engine->Get<Renderer>()->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	m_shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\transform.vs", "..\\Resources\\Shaders\\basic.fs");

	glGenBuffers(3, m_vboHandles);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositionData), vPositionData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vColorData), vColorData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(0, m_vboHandles[POSITION], 0, sizeof(glm::vec3));
	glBindVertexBuffer(1, m_vboHandles[COLOR], 0, sizeof(glm::vec3));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	m_mxTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.0f));

	m_engine->Get<Input>()->AddButton("click", Input::eButtonType::MOUSE, 0);
	m_engine->Get<Input>()->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
	m_engine->Get<Input>()->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);

	m_cursorScale = glm::vec3(0.05f, 0.1f, 0.0f);

	return 1;
}

void Scene02::Update()
{
	Input::eButtonState state = m_engine->Get<Input>()->GetButton("click");

	float x = m_engine->Get<Input>()->GetAnalogAbsolute("x-axis");
	float y = m_engine->Get<Input>()->GetAnalogAbsolute("y-axis");

	m_engine->Get<Renderer>()->m_width;
	m_engine->Get<Renderer>()->m_height;

	m_x = ((x / m_engine->Get<Renderer>()->m_width) * 2);
	m_y = ((y / m_engine->Get<Renderer>()->m_height) * 2);

	m_x--;
	m_y--;

	m_mxTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, -m_y, 0.0f));
	m_scale = glm::scale(glm::mat4(1.0f), m_cursorScale);
}

void Scene02::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(m_vaoHandle); // will be used for different object groups

	GLint modelMatrixLocation = glGetUniformLocation(m_shaderProgram, "modelMatrix");

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &(m_mxTranslate[0][0]));

	GLint scaleLocation = glGetUniformLocation(m_shaderProgram, "scale");

	glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &(m_scale[0][0]));

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Scene02::Shutdown()
{
}
