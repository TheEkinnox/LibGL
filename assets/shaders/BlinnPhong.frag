struct Material
{
	vec4		tint;
	vec4		specularColor;
	vec2		uvOffset;
	vec2		uvScale;
	sampler2D	diffuse;
	sampler2D	specular;
	sampler2D	normal;
	float		shininess;
	int			usedMaps;
};

uniform Material	u_material;

vec3 g_normal;
vec3 g_viewDir;
vec4 g_diffColor;
vec4 g_specColor;

vec3 calculateBlinnPhong(vec3 lightDir, vec4 lightColor)
{
	float lambertian = max(dot(lightDir, g_normal), 0);
	float specularIntensity = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + g_viewDir);
		float specularAngle = max(dot(halfDir, g_normal), 0);

		specularIntensity = pow(specularAngle, u_material.shininess);
	}

	vec3 diffuseColor = lightColor.rgb * g_diffColor.rgb * lambertian;
	vec3 specColor = lightColor.rgb * g_specColor.rgb * specularIntensity;

	return (diffuseColor + specColor) * lightColor.a;
}