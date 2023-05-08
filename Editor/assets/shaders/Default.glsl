#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec3 aMaterial;
layout (location = 5) in vec3 aTangent;
layout (location = 6) in vec3 aBitangent;
layout (location = 7) in ivec4 aBoneIds;
layout (location = 8) in vec4 aWeights;
layout (location = 9) in int aIsWorldPos;
layout (location = 10) in int aEntityId;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform mat4 uModel;
uniform mat3 uInverseModel;
uniform bool uHasAnimation;
uniform mat4 uBoneTransforms[MAX_BONES];
uniform int uEntityId;

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
	vertex.entityId = aEntityId != -1 ? aEntityId : uEntityId;
	
	gl_Position = uCamera.projection * uCamera.view * worldPosition;
}

vec4 CalcWorldPosition()
{
	vec4 position = vec4(0.0f);
	if (uHasAnimation == true)
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
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
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

layout (location = 0) out vec4 aFragColor;
layout (location = 1) out int aEntityId;

const int MAX_TEXTURES = 32;
const int MAX_SHADOW_CASCADES = 16;

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

layout (std140, binding = 3) uniform LightSpaceBlock
{
	mat4 lightSpaceMatrices[MAX_SHADOW_CASCADES];
} uLightSpace;

uniform sampler2DArray uShadowMap;
uniform sampler2D uTextures[MAX_TEXTURES];
uniform bool uHasPointLight;

// Shadow
uniform float uCascadePlaneDistances[MAX_SHADOW_CASCADES];
uniform int uCascadeCount;

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec4 color;
	float shadow;
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
float CalcShadow();
int GetShadowLayer();
float CalcShadowBias(int layer);

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

	material.shadow = CalcShadow();

	return material;
}

vec3 CalcDirectionalLight(Material material, vec3 normal, vec3 viewDir)
{
	// Ambient
	vec3 ambient = uDirLight.ambient * material.diffuse;
	
	// Diffuse
	vec3 lightDir = uDirLight.direction;
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = uDirLight.diffuse * diff * material.diffuse;

	// Specular
	if (vertex.material.y != -1)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
		vec3 specular = uDirLight.specular * spec * material.specular;

		return ambient + (1.0 - material.shadow) * (diffuse + specular);
	}

	return ambient + (1.0 - material.shadow) * diffuse;
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
		
		return ambient + (1.0 - material.shadow) * (diffuse + specular);
	}

	return ambient + (1.0 - material.shadow) * diffuse;
}

// Caculate shadow using Percentage-Closer Filtering (PCF)
float CalcShadow()
{
	// Select cascade layer
	const int layer = GetShadowLayer();
	if (layer == -1)
	{
		return 0.0;
	}

	vec4 fragPosLightSpace = uLightSpace.lightSpaceMatrices[layer] * vec4(vertex.fragPos, 1.0);
	
	// Perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	
	// Get depth of current fragment from light's perspective
	const float fragmentDepth = projCoords.z;

	// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (fragmentDepth > 1.0)
	{
		return 0.0;
	}

	// Calculate bias (based on depth map resolution and slope)
	const float bias = CalcShadowBias(layer);

	// Percentage-Closer Filtering with a 3x3 filter
	float shadow = 0.0;
	int sampleCount = 0;
	const int sampleSize = 1;
	vec2 texelSize = 1.0 / vec2(textureSize(uShadowMap, 0));
	for (int x = -sampleSize; x <= sampleSize; ++x)
	{
		for (int y = -sampleSize; y <= sampleSize; ++y)
		{
			vec2 offset = vec2(x, y) * texelSize;
			vec3 texCoord = vec3(projCoords.xy + offset, layer);
			float depth = texture(uShadowMap, texCoord).r;
			shadow += depth < (fragmentDepth - bias) ? 1.0 : 0.0;
			++sampleCount;
		}
	}

	shadow /= (sampleCount * 2.0);

	return shadow;
}

// Get cascade shadow layer for a given fragment, -1 ifthe  depth is greater than cascade plane distances
int GetShadowLayer()
{
	vec4 fragPosViewSpace = uCamera.view * vec4(vertex.fragPos, 1.0);
	float depthValue = abs(fragPosViewSpace.z);

	int layer = -1;
	for (int i = 0; i < uCascadeCount; ++i)
	{
		if (depthValue < uCascadePlaneDistances[i])
		{
			layer = i;
			break;
		}
	}

	return layer;
}

// Calculate bias (based on depth map resolution and slope)
float CalcShadowBias(int layer)
{
	vec3 normal = normalize(vertex.normal);
	float bias = max(0.1 * (1.0 - dot(normal, uDirLight.direction)), 0.01);
	const float biasModifier = 0.5f;
	bias *= 1.0 / (uCascadePlaneDistances[layer] * biasModifier);

	return bias;
}
