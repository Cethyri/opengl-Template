#pragma once
#include "glm\glm.hpp"

namespace MeshLoader
{
	bool LoadMesh(const std::string& filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs);
}

