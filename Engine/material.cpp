#include "stdafx.h"
#include "material.h"
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Material::Material()
{
	m_ambient = glm::vec3(0.2f);
	m_diffuse = glm::vec3(0.8f);
	m_specular = glm::vec3(1.0f);
	m_shininess = 10.0f;
}


Material::~Material()
{
}

void Material::SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
}

GLuint Material::CreateTexture(GLuint width, GLuint height)
{
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void Material::AddTexture(GLenum type, GLuint texture, GLuint activeTexture)
{
	TextureInfo textureInfo = { type, activeTexture, texture };
	m_textures.push_back(textureInfo);
}

bool Material::LoadTexture2D(const std::string & filename, GLuint activeTexture)
{

	int width;
	int height;
	int n;

	unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);

	GLuint texture;

	glGenTextures(1, &texture);

	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, texture);

	AddTexture(GL_TEXTURE_2D, activeTexture, texture);

	GLuint formatStorage = n == 4 ? GL_RGBA8 : GL_RGB8;
	GLuint formatImage = n == 4 ? GL_RGBA : GL_RGB;
	glTexStorage2D(GL_TEXTURE_2D, 1, formatStorage, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, formatImage, GL_UNSIGNED_BYTE, image);

	delete[] image;

	return true;
}

bool Material::LoadTextureCube(const std::string& basename, const std::vector<std::string>& suffixes, const std::string& type, GLuint activeTexture)
{
	GLuint targets[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < 6; i++)
	{
		int width;
		int height;
		int n;

		std::string filename = basename + suffixes[i] + "." + type;

		unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);
		assert(image);

		GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(targets[i], 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);

		delete image;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	AddTexture(GL_TEXTURE_CUBE_MAP, activeTexture, texture);

	return true;
}

void Material::SetTextures()
{
	for (TextureInfo textureInfo : m_textures)
	{
		glActiveTexture(textureInfo.activeTexture);
		glBindTexture(textureInfo.type, textureInfo.texture);
	}
}


