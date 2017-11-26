#version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;

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

	vec3 positionToLight;
	if (light.position.w == 0)
	{
		positionToLight = normalize(vec3(light.position));
	}
	else
	{
		positionToLight = vec3(normalize(light.position - outFragmentPosition));
	}

	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);

	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;

	ambientDiffuse = ambient + diffuse;

	vec3 positionToView = normalize(-outFragmentPosition.xyz); 
	vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
	float specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);

	specularIntensity = pow(specularIntensity, material.shininess);
	specular = light.specular * material.specular * specularIntensity;
}

void main()
{
	vec3 ambientDiffuse;
	vec3 specular;
	phong(outFragmentPosition, outFragmentNormal, ambientDiffuse, specular);

	vec3 outColor = ambientDiffuse + specular;
	outFragmentColor = vec4(outColor, 1.0);
}