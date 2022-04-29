#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aMaterial;
layout(location = 5) in vec3 aTangent;
layout(location = 6) in vec3 aBitangent;
layout(location = 7) in ivec4 aBoneIds;
layout(location = 8) in vec4 aWeights;
layout(location = 9) in int aIsWorldPos;
layout(location = 10) in int aHasAnimations;
layout(location = 11) in int aEntityId;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform mat4 uModel;
uniform mat3 uInverseModel;
uniform mat4 uBoneTransforms[MAX_BONES];

out Vertex
{
	vec3 fragPos;
	vec3 normal;
	vec4 color;
	vec2 texCoord;
	flat vec3 material;
	flat int entityId;
} vertex;

vec4 CalcWorldPosition();
vec4 CalcBonePosition();
vec3 CalcWorldNormal();

void main()
{
	vec4 worldPosition = CalcWorldPosition();
	vec3 worldNormal = CalcWorldNormal();

	vertex.fragPos = vec3(worldPosition);
	vertex.normal = worldNormal;
	vertex.color = aColor;
	vertex.texCoord = aTexCoord;
	vertex.material = aMaterial;
	vertex.entityId = aEntityId;
	
	gl_Position = uCamera.projection * uCamera.view * worldPosition;
}

vec4 CalcWorldPosition()
{
	vec4 position = vec4(0.0f);
	if (aHasAnimations == 1)
	{
		position = CalcBonePosition();
	}
	else
	{
		position = vec4(aPosition, 1.0f);
	}

	if (aIsWorldPos == 0)
	{
		position = uModel * position;
	}
	
	return position;
}

vec4 CalcBonePosition()
{
	vec4 bonePosition = vec4(0.0f);
	for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
	{
		int boneId = aBoneIds[i];
		if (boneId == -1)
		{
			continue;
		}

		if (boneId >= MAX_BONES)
		{
			return vec4(aPosition, 1.0f);
		}

		bonePosition += uBoneTransforms[boneId] * vec4(aPosition, 1.0f) * aWeights[i];
	}

	return bonePosition;
}

vec3 CalcWorldNormal()
{
	if (aIsWorldPos == 0)
	{
		return uInverseModel * aNormal;
	}
	else
	{
		return aNormal;
	}
}

#type fragment
#version 450 core

layout(location = 0) out vec4 aFragColor;
layout(location = 1) out int aEntityId;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

layout (std140, binding = 1) uniform DirLightBlock
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} uDirLight;

layout (std140, binding = 2) uniform PointLightBlock
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} uPointLight;

uniform sampler2D uTextures[32];
uniform bool uHasPointLight;

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec4 color;
};

in Vertex
{
	vec3 fragPos;
	vec3 normal;
	vec4 color;
	vec2 texCoord;
	flat vec3 material;
	flat int entityId;
} vertex;

Material SetupMaterial();
vec3 CalcDirectionalLight(Material material, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Material material, vec3 normal, vec3 viewDir);

void main()
{
	// Properties
	vec3 normal = normalize(vertex.normal);
	vec3 viewDir = normalize(uCamera.viewPosition - vertex.fragPos);
	Material material = SetupMaterial();

	vec3 result = CalcDirectionalLight(material, normal, viewDir);
	if (uHasPointLight)
	{
		result += CalcPointLight(material, normal, viewDir);
	}

	aFragColor = vec4(result, 1.0f) * material.color;
	aEntityId = vertex.entityId;
}

Material SetupMaterial()
{
	Material material;
	if (vertex.material.x != -1)
	{
		material.diffuse = texture(uTextures[int(round(vertex.material.x))], vertex.texCoord).rgb;
		material.color = vertex.color;
	}
	else
	{
		material.diffuse = vertex.color.rgb;
		material.color = vec4(vec3(1.0f), vertex.color.a);;
	}

	if (vertex.material.y != -1)
	{
		material.specular = texture(uTextures[int(round(vertex.material.y))], vertex.texCoord).rgb;
		material.shininess = vertex.material.z;
	}

	return material;
}

vec3 CalcDirectionalLight(Material material, vec3 normal, vec3 viewDir)
{
	// Ambient
	vec3 ambient = uDirLight.ambient * material.diffuse;
	
	// Diffuse
	vec3 lightDir = normalize(-uDirLight.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = uDirLight.diffuse * diff * material.diffuse;  

	// Specular
	if (vertex.material.y != -1)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
		vec3 specular = uDirLight.specular * spec * material.specular;

		return ambient + diffuse + specular;
	}

	return ambient + diffuse;
}

vec3 CalcPointLight(Material material, vec3 normal, vec3 viewDir)
{
	// Attenuation
	float distance = length(uPointLight.position - vertex.fragPos);
	float attenuation = 1.0 / (uPointLight.constant + uPointLight.linear * distance + uPointLight.quadratic * (distance * distance));    

	// Ambient
	vec3 ambient = uPointLight.ambient * material.diffuse * attenuation;

	// Diffuse
	vec3 lightDir = normalize(uPointLight.position - vertex.fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = uPointLight.diffuse * diff * material.diffuse * attenuation;
	
	// Specular
	if (vertex.material.y != -1)
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = uPointLight.specular * spec * material.specular * attenuation;
		
		return ambient + diffuse + specular;
	}

	return ambient + diffuse;
}
