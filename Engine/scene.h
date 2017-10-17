#pragma once
#include "engine.h"

class Scene
{
public:
	Scene(Engine* engine) : m_engine(engine) {};
	//~Scene();

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

protected:
	Engine* m_engine;
};

