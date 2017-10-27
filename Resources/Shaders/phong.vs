#version 410

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;

uniform mat4 mxModelView;
uniform mat4 mxMVP;
uniform mat3 mxNormal;

out vec4 outFragmentPosition;
out vec3 outFragmentNormal;

void main()
{
	outFragmentPosition = mxModelView * vec4(vertexPosition, 1.0);
	outFragmentNormal = mxNormal * vertexNormal;

   gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}