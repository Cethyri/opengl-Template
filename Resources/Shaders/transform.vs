#version 410

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexColor;

uniform mat4 modelMatrix;
uniform mat4 scale;

out vec3 outVertexColor;

void main()
{
   outVertexColor = vertexColor;
   gl_Position = modelMatrix * (scale * vec4(vertexPosition, 1.0));
}