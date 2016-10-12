#version 330 core

out vec4 color;

in vec2 TexCoord0;
in vec3 FragPos0;
in vec3 Tangent0;
in vec3 Normal0;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float shininessStrength;
};

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	float intensity;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
    float constant;
    float linear;
    float quadratic;
	vec3 color;
	bool activated;
};
struct PointLight {
	vec3 position;
    float constant;
    float linear;
    float quadratic;
	vec3 color;
	bool activated;
};

uniform Material material;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_depth;
uniform bool hasNormalTexture;

#define NUM_POINT_LIGHTS 7
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

/* Lighting Calculation uniforms */
uniform vec3 viewPosition;
uniform float ambientStrength;
uniform vec3 shadowTexelSize;

/* Light Calculation */
// DirectionalLight
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;
	vec3 lightDir = normalize(-light.direction);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Calculate result
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	float shadowAmount = 1;/*calcShadowAmount(textureDepth, ShadowCoord0, normal, lightDir);*/
	
	return (ambient+diffuse+specular*shadowAmount)*(0.5+0.5*shadowAmount)*light.intensity;
}

// Spotlight
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;

	vec3 lightDir = normalize(light.position-fragPos);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position-fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}

// PointLight
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;

	vec3 lightDir = normalize(light.position-fragPos);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position-fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}



/* Bump Map Calculation */
vec3 calcBumpMap(sampler2D textureNormal, vec2 texCoord)
{
	vec3 result;
	
	vec3 normal = normalize(Normal0);
	vec3 tangent = normalize(Tangent0);
	tangent = normalize(tangent - dot(tangent,normal)*normal);
	vec3 bitangent = cross(tangent,normal);
	
	vec3 bumpmapnormal = texture(textureNormal, texCoord).xyz;
	bumpmapnormal = 2 * bumpmapnormal - vec3(1);
	mat3 TBN = mat3(tangent, bitangent, normal);
	result = TBN * bumpmapnormal;
	result = normalize(result);
	
	return result;	
}

void main()
{
	vec3 normal = Normal0;
	if(hasNormalTexture)
	{
		normal = calcBumpMap(texture_normal, TexCoord0);
	}
	
	vec4 diffuseColor = texture2D(texture_diffuse, TexCoord0);
	vec3 viewDir = normalize(viewPosition - FragPos0);
	vec3 dirLight = calcDirLight(directionalLight, normal, viewDir, material);
	vec4 result = vec4(dirLight, 1);
	result += vec4(1,1,1,1)*vec4(ambientStrength);
	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		if(pointLights[i].activated)
			result += max(vec4(0),vec4(calcPointLight(pointLights[i], normal, FragPos0, viewDir, material),1.0));
	}
	color = result * diffuseColor;
}
