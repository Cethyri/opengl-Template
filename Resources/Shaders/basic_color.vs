#version 430

layout (location=0) in vec3 positionVertex;
	
uniform vec3 color;
uniform mat4 mxMVP;

out vec3 outVertexColor;

void main()
{
    outVertexColor = color;
    gl_Position = mxMVP * vec4(positionVertex, 1.0);
}
