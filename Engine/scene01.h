#pragma once
#include "scene.h"

class Scene01 :
	public Scene
{
public:
	Scene01(Engine* engine);
	~Scene01();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;

private:
	GLuint program;
	GLuint vboHandles[3];
	GLuint vaoHandle;
};

