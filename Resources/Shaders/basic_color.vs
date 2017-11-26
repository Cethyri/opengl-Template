#version 430

layout (location=0) in vec3 vertexPosition;
	
uniform vec3 color;
uniform mat4 mxMVP;

out vec3 outVertexColor;

void main()
{
    outVertexColor = color;
    gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}
