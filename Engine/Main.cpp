#include <iostream>
#include "stdafx.h"
#include "engine.h"
#include "renderer.h"
#include "GLM\glm\glm.hpp"

int main() {

	std::shared_ptr<Engine> engine(new Engine);
	if (!engine->Initialize())
	{
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(engine->Get<Renderer>()->m_window))
	{
		engine->Update();

		// scene render

		// render code

		glfwSwapBuffers(engine->Get<Renderer>()->m_window);
	}
	engine->Shutdown();
}