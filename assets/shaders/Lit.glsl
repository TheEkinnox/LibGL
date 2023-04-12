#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoords;

out VS_OUT
{
	vec3	FragPos;
	vec3	Normal;
	vec2	TexCoords;
} vs_out;

uniform mat4 u_mvp;
uniform mat4 u_modelMat;
uniform mat4 u_normalMat;

void main()
{
	gl_Position = u_mvp * vec4(_pos, 1.0);
	
	vs_out.FragPos = (u_modelMat * vec4(_pos, 1.0)).xyz;
	vs_out.Normal = (u_normalMat * vec4(_normal, 0)).xyz;
	vs_out.TexCoords = _texCoords;
}

#shader fragment
#version 330 core

#define NB_POINT_LIGHTS 4

struct DirLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	direction;
};

struct PointLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	position;

	float	constant;
	float	linear;
	float	quadratic;
};

struct SpotLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	position;
	vec3	direction;

	float	constant;
	float	linear;
	float	quadratic;

	float	cutOff;
	float	outerCutOff;
};

struct Material
{
	sampler2D	diffuse;
	sampler2D	specular;
	float		shininess;
};

in VS_OUT
{
	vec3	FragPos;
	vec3	Normal;
	vec2	TexCoords;
} fs_in;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NB_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material _material;

uniform vec3 u_viewPos;

uniform Material u_material;

vec3 g_normal;
vec3 g_viewDir;
vec4 g_diffColor;
vec4 g_specColor;

vec3 calculateBlinnPhong(vec3 lightDir, vec4 ambient, vec4 diffuse, vec4 specular);
vec3 calculateDirLight(DirLight light);
vec3 calculatePointLight(PointLight light);
vec3 calculateSpotLight(SpotLight light);

void main()
{
	g_normal = normalize(fs_in.Normal);
	g_viewDir = normalize(u_viewPos - fs_in.FragPos);
	g_diffColor = texture(u_material.diffuse, fs_in.TexCoords);
	g_specColor = texture(u_material.specular, fs_in.TexCoords);

	vec3 litColor = vec3(0);

	litColor += calculateDirLight(dirLight);

	for (int i = 0; i < NB_POINT_LIGHTS; i++)
		litColor += calculatePointLight(pointLights[i]);

	litColor += calculateSpotLight(spotLight);

	FragColor = vec4(litColor, g_diffColor.w);
}

vec3 calculateBlinnPhong(vec3 lightDir, vec4 ambient, vec4 diffuse, vec4 specular)
{
	float lambertian = max(dot(lightDir, g_normal), 0);
	float specularIntensity = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + g_viewDir);
		float specularAngle = max(dot(halfDir, g_normal), 0);

		specularIntensity = pow(specularAngle, u_material.shininess);
	}

	vec3 ambientColor = ambient.xyz * g_diffColor.xyz;
	vec3 diffuseColor = diffuse.xyz * g_diffColor.xyz;
	vec3 specularColor = specular.xyz * g_specColor.xyz;

	return (ambientColor + diffuseColor * lambertian + specularColor * specularIntensity);
}

vec3 calculateDirLight(DirLight light)
{
	vec3 lightDir = normalize(-light.direction);

	return calculateBlinnPhong(lightDir, light.ambient, light.diffuse, light.specular);
}

vec3 calculatePointLight(PointLight light)
	{
	vec3 lightDir = normalize(light.position - fs_in.FragPos);

	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				 light.quadratic * (distance * distance));

	return calculateBlinnPhong(lightDir, light.ambient, light.diffuse, light.specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light)
{
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	vec3 spotDir = normalize(-light.direction);

	float spotAngle = dot(lightDir, spotDir);

	if (spotAngle < light.outerCutOff)
		return vec3(0);

	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((spotAngle - light.outerCutOff) / epsilon, 0.0, 1.0);

	if (epsilon == 0.0)
		return vec3(0);

	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				light.quadratic * (distance * distance));

	return calculateBlinnPhong(lightDir, light.ambient, light.diffuse, light.specular) * attenuation * intensity;
}