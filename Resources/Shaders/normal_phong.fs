#version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;
in vec2 outFragmentTexCoord;

layout (binding=0) uniform sampler2D textureSample;
layout (binding=1) uniform sampler2D normalSample;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

uniform Material material;
uniform Light light;

layout (location=0) out vec4 outFragmentColor;

void phong(in vec4 position, in vec3 normal, out vec3 ambientDiffuse, out vec3 specular) 
{

	vec3 ambient = material.ambient;

	vec3 positionToLight = vec3(normalize(light.position - position));

	float diffuseIntensity = max(dot(positionToLight, normal), 0.0);

	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;

	ambientDiffuse = ambient + diffuse;

	vec3 positionToView = normalize(-position.xyz); 
	vec3 reflectLightVector = reflect(-positionToLight, normal);
	float specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);

	specularIntensity = pow(specularIntensity, material.shininess);
	specular = light.specular * material.specular * specularIntensity;
}

void main()
{
	vec3 ambientDiffuse;
	vec3 specular;

	vec3 normal = texture(normalSample, outFragmentTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	phong(outFragmentPosition, normal, ambientDiffuse, specular);

	vec3 textureColor = vec3(texture(textureSample, outFragmentTexCoord));

	vec3 outColor = textureColor * ambientDiffuse + specular;

	outFragmentColor = vec4(outColor, 1.0);
}