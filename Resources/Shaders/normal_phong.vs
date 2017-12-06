#version 430

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexTangent;

uniform mat4 mxModelView;
uniform mat4 mxMVP;
uniform mat3 mxNormal;

uniform vec4 lightPosition;

out vec4 outTangentPosition;
out vec3 outTangentLightPosition;
out vec2 outFragmentTexCoord;

void main()
{
	vec3 normal = normalize(mxNormal * vertexNormal);
	vec3 tangent = normalize(mxNormal * vertexTangent);

	vec3 bitangent = cross(normal, tangent);

	mat3 tangentSpace = transpose(mat3(tangent, bitangent, normal));

	outTangentLightPosition = tangentSpace * lightPosition.xyz;

	vec3 position = vec3(mxModelView * vec4(vertexPosition, 1.0));
	outTangentPosition = vec4(tangentSpace * position, 1.0);


	outFragmentTexCoord = vertexTexCoord;

   gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}