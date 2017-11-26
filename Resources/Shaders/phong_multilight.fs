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

#define NUM_LIGHTS 5

uniform Material material;
uniform Light lights[NUM_LIGHTS];

layout (location=0) out vec4 outFragmentColor;

void phong(in int lightIndex, in vec4 position, in vec3 normal, out vec3 diffuse, out vec3 specular)
{
	

	vec3 positionToLight;
	if (lights[lightIndex].position.w == 0)
	{
		positionToLight = normalize(vec3(lights[lightIndex].position));
	}
	else
	{
		positionToLight = vec3(normalize(lights[lightIndex].position - outFragmentPosition));
	}

	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);

	diffuse = lights[lightIndex].diffuse * material.diffuse * diffuseIntensity;

	vec3 positionToView = normalize(-outFragmentPosition.xyz); 
	vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
	float specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);

	specularIntensity = pow(specularIntensity, material.shininess);

	specular = lights[lightIndex].specular * material.specular * specularIntensity;
}

void main()
{
	vec3 ambient = material.ambient;
	vec3 outColor = ambient;

	vec3 diffuse;
	vec3 specular;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		phong(i, outFragmentPosition, outFragmentNormal, diffuse, specular);
		outColor += (diffuse + specular);
	}

	outFragmentColor = vec4(outColor, 1.0);
}