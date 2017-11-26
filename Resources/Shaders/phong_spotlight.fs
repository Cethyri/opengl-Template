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
	vec4 position;
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
	float exponent;
	float cutoff;

};

//struct Fog
//{
//	float distanceMin;
//	float distanceMax;
//	vec3 color;
//};
//
//uniform Fog fog;

uniform Material material;
uniform Light light;

layout (location=0) out vec4 outFragmentColor;

void phong(in vec4 position, in vec3 normal, out vec3 ambientDiffuse, out vec3 specular) 
{

	vec3 ambient = material.ambient;
	
	vec3 directionToLight  = vec3(normalize(light.position - position));
	vec3 spotDirection = normalize(light.direction);

	float angle = acos(dot(-directionToLight, spotDirection));

	if (angle < light.cutoff) 
	{
		float spotFactor = pow(dot(-directionToLight, spotDirection), light.exponent);


		float diffuseIntensity = max(dot(directionToLight, normal), 0.0);

		vec3 diffuse = (light.diffuse * material.diffuse * diffuseIntensity) * spotFactor;


		ambientDiffuse = ambient + diffuse;

		vec3 positionToView = normalize(-position.xyz); 
		vec3 reflectLightVector = reflect(-directionToLight, normal);
		float specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);

		specularIntensity = pow(specularIntensity, material.shininess);

		specular = light.specular * material.specular * specularIntensity * spotFactor;
	}
	else
	{
		ambientDiffuse = ambient;
		specular = vec3(0.0f, 0.0f, 0.0f);
	}
}

void main()
{
	vec3 ambientDiffuse;
	vec3 specular;
	phong(outFragmentPosition, outFragmentNormal, ambientDiffuse, specular);

	vec3 outColor = ambientDiffuse + specular;


	//float distance = abs(outFragmentPosition.z);
	//
	//float fogIntensity = clamp((distance - fog.distanceMin) / (fog.distanceMax - fog.distanceMin), 0.0, 1.0);
	//
	//vec3 outColorWithFog = mix(outColor, fog.color, fogIntensity);

	outFragmentColor = vec4(outColor, 1.0);
}