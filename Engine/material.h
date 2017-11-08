#pragma once
#include "glm\glm.hpp"

class Material
{
public:
	Material();
	~Material();

	void SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

	bool LoadTexture2D(const std::string& filename, GLuint activeTexture);
	void SetTextures();


public:

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;

private:
	struct TextureInfo
	{
		GLuint activeTexture;
		GLuint texture;
	};

	std::vector<TextureInfo> m_textures;

};

