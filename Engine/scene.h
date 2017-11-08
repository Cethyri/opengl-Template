#pragma once
#include "engine.h"
#include "shader.h"

class Object;
class Scene
{
public:
	Scene(Engine* engine) : m_engine(engine) {};
	//~Scene();

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	Engine* m_engine;

	void AddObject(Object* object);

	template<typename T>
	T* GetObject(const std::string& name);

	template<typename T>
	std::vector<T*> GetObjects();

protected:
	std::vector<Object*> m_objects;
	Shader m_shader;
};

template<typename T>
inline T * Scene::GetObject(const std::string & name)
{
	for (auto object : m_objects)
	{
		if (object->m_name._Equal(name))
		{
			return dynamic_cast<T*>(object);
		}
	}
}

template<typename T>
inline std::vector<T*> Scene::GetObjects()
{
	std::vector<T*> objects;

	for (auto object : m_objects)
	{
		//if (typeof(object) == T)
		if (dynamic_cast<T*>(object) != nullptr)
		{
			objects.push_back(object);
		}
	}
	return objects;
}
