#pragma once
#include "scene.h"

class Scene03 :
	public Scene
{
public:
	Scene03(Engine* engine);
	~Scene03();

	// Inherited via Scene
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Shutdown() override;

private:
	//GLuint m_shaderProgram;
	GLuint m_vboHandle[1];
	//GLuint m_vaoHandle;

	struct object
	{
		// id / handles
		GLuint shaderProgram;
		GLuint vaoHandle;

		// transforms
		GLint mxModelViewUniform;
		GLint mxMVPUniform;
		GLint mxNormalUniform;

		// material
		GLint ambientMaterialUniform;
		GLint diffuseMaterialUniform;
		GLint specularMaterialUniform;
	};

	struct light
	{
		GLint positionUniform;
		GLint colorUniform;
	};


	object m_cube;
	light m_light;

	float m_rotation;

};
