#version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;
in vec2 outFragmentTexCoord;

layout (binding=0) uniform sampler2D textureSample;
layout (binding=1) uniform sampler2D specularSample;
layout (binding=2) uniform sampler2D normalSample;

uniform mat3 mxNormal;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

uniform vec3 lightPosition;
uniform vec3 lightColor;

layout (location=0) out vec4 outFragmentColor;

void main()
{

	//vec3 normalMapVector = normalize((vec3(texture(normalSample, outFragmentTexCoord)) / 2) - vec3(1));
	//vec3 normalMapVector = normalize(texture( normalSample, outFragmentTexCoord ).rgb*2.0 - 1.0);
	//vec3 normalMapVector = vec3(0, 0, 1);



	//mat3 toFaces = mat3(outFragmentNormal.zxy, outFragmentNormal.yzx, outFragmentNormal.xyz);

	vec3 fragNormals = mxNormal * outFragmentNormal;

	vec3 ambient = lightColor * material.ambient;

	vec3 positionToLight = normalize(lightPosition - vec3(outFragmentPosition));

	float diffuseIntensity = max(dot(positionToLight, fragNormals), 0.0);

	vec3 diffuse = lightColor * material.diffuse * diffuseIntensity;

	vec3 specular = vec3(0.0);

	vec3 positionToView = normalize(-outFragmentPosition.xyz); 
	vec3 reflectLightVector = reflect(-positionToLight, fragNormals);
	float specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);

	specularIntensity = pow(specularIntensity, material.shininess);
	specular = lightColor * material.specular * specularIntensity;

	
	vec3 textureColor = vec3(texture(textureSample, outFragmentTexCoord));

	vec3 specularMapColor = vec3(texture(specularSample, outFragmentTexCoord));

	float specularMapValue = (specularMapColor.x + specularMapColor.y + specularMapColor.z) / 3;

	

	//vec3 outVertexColor = mix(textureColor, specularMapColor, specularIntensity);// * (ambient + diffuse) + specular;
	vec3 outVertexColor = textureColor * (ambient + diffuse) + (specular * specularMapValue);
	//vec3 outVertexColor = textureColor * (ambient + diffuse) + (specular);

	outFragmentColor = vec4(outVertexColor, 1.0f);
}