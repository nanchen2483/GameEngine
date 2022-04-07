#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aMaterial;
layout(location = 5) in vec3 aTangent;
layout(location = 6) in vec3 aBitangent;
layout(location = 7) in vec4 aBone1;
layout(location = 8) in vec4 aBone2;
layout(location = 9) in ivec4 aBoneIds;
layout(location = 10) in vec4 aWeights;
layout(location = 11) in int aEntityId;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform bool uEnableAnimation;
uniform bool uUseModel;
uniform mat4 uModel;
uniform mat4 uViewProjection;
uniform mat4 uFinalBonesMatrices[MAX_BONES];

out Vertex
{
	vec3 fragPos;
	vec3 normal;
	vec4 color;
	vec2 texCoord;
	flat vec3 material;
	flat int entityId;
} vertex;

vec4 CalcPosition();

void main()
{
	vec4 position = CalcPosition();

	vertex.fragPos = vec3(position);
	vertex.normal = aNormal;
	vertex.color = aColor;
	vertex.texCoord = aTexCoord;
	vertex.material = aMaterial;
	vertex.entityId = aEntityId;
	
	gl_Position = uViewProjection * position;
}

vec4 CalcPosition()
{
	vec4 finalPosition = vec4(0.0f);
	if (uEnableAnimation)
	{
		for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
		{
			int boneId = aBoneIds[i];
			if (boneId == -1)
			{
				continue;
			}

			if (boneId >= MAX_BONES)
			{
				finalPosition = vec4(aPosition, 1.0f);
				break;
			}

			vec4 localPosition = uFinalBonesMatrices[aBoneIds[i]] * vec4(aPosition, 1.0f);
			finalPosition += localPosition * aWeights[i];
		}
	}
	else
	{
		finalPosition = vec4(aPosition, 1.0f);
	}

	if (uUseModel)
	{
		finalPosition = uModel * finalPosition;
	}
	
	return finalPosition;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityId;

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec4 color;
} material; 

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 uViewPos;
uniform DirLight uDirLight;
uniform sampler2D uTextures[32];

in Vertex
{
	vec3 fragPos;
	vec3 normal;
	vec4 color;
	vec2 texCoord;
	flat vec3 material;
	flat int entityId;
} vertex;

void SetupMaterial();
vec3 CalcDirectionalLight(Material material, DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	// Properties
	vec3 normal = normalize(vertex.normal);
	vec3 viewDir = normalize(uViewPos - vertex.fragPos);

	SetupMaterial();

	vec3 result = CalcDirectionalLight(material, uDirLight, normal, viewDir);
	FragColor = vec4(result, 1.0) * material.color;
	EntityId = vertex.entityId;
}

void SetupMaterial()
{
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

	if (vertex.material.x != -1)
	{
		material.specular = texture(uTextures[int(round(vertex.material.y))], vertex.texCoord).rgb;
		material.shininess = vertex.material.z;
	}
}

vec3 CalcDirectionalLight(Material material, DirLight light, vec3 normal, vec3 viewDir)
{
	// Ambient
	vec3 ambient = light.ambient * material.diffuse;
	
	// Diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;  

	// Specular
	if (vertex.material.y != -1)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * material.specular;

		return ambient + diffuse + specular;
	}

	return ambient + diffuse;
}
