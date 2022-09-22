#type vertex
#version 430

layout (location = 0) in vec2 aPosition;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

layout (std140, binding = 4) uniform TransformBlock
{
	mat4 worldMatrix;
} uTransform;

uniform mat4 uLocalMatrix;
uniform float uScaleY;
uniform int uLod;
uniform vec2 uIndex;
uniform float uGap;
uniform vec2 uLocation;
uniform int uLodMorphArea[8];
uniform sampler2D uHeightMap;

out vec2 vTexCoord;

vec2 Morph(vec2 localPosition, float height, int morphArea);
float MorphLatitude(vec2 position);
float MorphLongitude(vec2 position);

void main()
{
	vec2 localPosition = (uLocalMatrix * vec4(aPosition.x, 0.0f, aPosition.y, 1.0f)).xz;

	float height = texture(uHeightMap, localPosition).r;
	if (uLod > 0)
	{
		localPosition += Morph(localPosition, height, uLodMorphArea[uLod - 1]);
		height = texture(uHeightMap, localPosition).r;
	}

	vTexCoord = localPosition;
	gl_Position = uTransform.worldMatrix * vec4(localPosition.x, height, localPosition.y, 1.0f);
}

vec2 Morph(vec2 localPosition, float height, int morphArea)
{
	vec2 morphing = vec2(0.0f);

	vec2 fixPointLatitude = vec2(0.0f);
	vec2 fixPointLongitude = vec2(0.0f);
	if (uIndex == vec2(0.0f))
	{
		fixPointLatitude = uLocation + vec2(uGap, 0.0f);
		fixPointLongitude = uLocation + vec2(0.0f, uGap);
	}
	else if (uIndex == vec2(1.0f, 0.0f))
	{
		fixPointLatitude = uLocation;
		fixPointLongitude = uLocation + vec2(uGap, uGap);
	}
	else if (uIndex == vec2(0.0f, 1.0f))
	{
		fixPointLatitude = uLocation + vec2(uGap, uGap);
		fixPointLongitude = uLocation;
	}
	else if (uIndex == vec2(1.0f, 1.0f))
	{
		fixPointLatitude = uLocation + vec2(0.0f, uGap);
		fixPointLongitude = uLocation + vec2(uGap, 0.0f);
	}

	float distLatitude = length(uCamera.viewPosition - (uTransform.worldMatrix * vec4(fixPointLatitude.x, height, fixPointLatitude.y, 1.0f)).xyz);
	float distLongitude = length(uCamera.viewPosition - (uTransform.worldMatrix * vec4(fixPointLongitude.x, height, fixPointLongitude.y, 1.0f)).xyz);

	if (distLatitude > morphArea)
	{
		morphing.x = MorphLatitude(localPosition);
	}

	if (distLongitude > morphArea)
	{
		morphing.y = MorphLongitude(localPosition);
	}

	return morphing;
}

float MorphLatitude(vec2 position)
{
	vec2 frac = position - uLocation;

	if (uIndex == vec2(0.0f))
	{
		float morph = frac.x - frac.y;
		if (morph > 0.0f)
		{
			return morph;
		}
	}

	if (uIndex == vec2(1.0f, 0.0f))
	{
		float morph = uGap - frac.x - frac.y;
		if (morph > 0.0f)
		{
			return morph;
		}
	}

	if (uIndex == vec2(0.0f, 1.0f))
	{
		float morph = frac.x + frac.y - uGap;
		if (morph > 0.0f)
		{
			return -morph;
		}
	}

	if (uIndex == vec2(1.0f, 1.0f))
	{
		float morph = frac.y - frac.x;
		if (morph > 0.0f)
		{
			return -morph;
		}
	}

	return 0;
}

float MorphLongitude(vec2 position)
{
	vec2 frac = position - uLocation;

	if (uIndex == vec2(0.0f))
	{
		float morph = frac.y - frac.x;
		if (morph > 0.0f)
		{
			return -morph;
		}
	}

	if (uIndex == vec2(1.0f, 0.0f))
	{
		float morph = frac.y - (uGap  - frac.x);
		if (morph > 0.0f)
		{
			return morph;
		}
	}

	if (uIndex == vec2(0.0f, 1.0f))
	{
		float morph = uGap - frac.x - frac.y;
		if (morph > 0.0f)
		{
			return -morph;
		}
	}

	if (uIndex == vec2(1.0f, 1.0f))
	{
		float morph = frac.x - frac.y;
		if (morph > 0.0f)
		{
			return morph;
		}
	}

	return 0;
}

#type control
#version 430

layout (vertices = 16) out;

const int LEFT = 0;
const int BOTTOM = 1;
const int RIGHT = 2;
const int TOP = 3;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform int uTessellationFactor;
uniform float uTessellationSlope;
uniform float uTessellationShift;

in vec2 vTexCoord[];
out vec2 cTexCoord[];

float LodFactor(float distance)
{
	float tessellationLevel = max(0.0f, uTessellationFactor / pow(distance, uTessellationSlope) + uTessellationShift);

	return tessellationLevel;
}

void main()
{
	if (gl_InvocationID == 0)
	{
		vec3 midLeft = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position) / 2.0f;
		vec3 midBottom = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position) / 2.0f;
		vec3 midRight = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position) / 2.0f;	
		vec3 midTop = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position) / 2.0f;

		float distanceLeft = distance(midLeft, uCamera.viewPosition);
		float distanceBottom = distance(midBottom, uCamera.viewPosition);
		float distanceRight = distance(midRight, uCamera.viewPosition);
		float distanceTop = distance(midTop, uCamera.viewPosition);

		gl_TessLevelOuter[LEFT] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceLeft));
		gl_TessLevelOuter[BOTTOM] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceBottom));
		gl_TessLevelOuter[RIGHT] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceRight));
		gl_TessLevelOuter[TOP] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceTop));

		gl_TessLevelInner[0] = (gl_TessLevelOuter[TOP] + gl_TessLevelOuter[BOTTOM]) / 4.0f;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[RIGHT] + gl_TessLevelOuter[LEFT]) / 4.0f;
	}

	cTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

#type evaluation
#version 430

layout (quads, fractional_odd_spacing, cw) in;

uniform sampler2D uHeightMap;
uniform float uScaleY;

in vec2 cTexCoord[];
out vec2 eTexCoord;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	// world position
	vec4 position =
	(
		(1 - u) * (1 - v) * gl_in[12].gl_Position +
			 u  * (1 - v) * gl_in[0].gl_Position +
			 u  *	   v  * gl_in[3].gl_Position +
		(1 - u) *	   v  * gl_in[15].gl_Position
	);

	vec2 texCoord =
	(
		(1 - u) * (1 - v) * cTexCoord[12] +
			 u  * (1 - v) * cTexCoord[0] +
			 u  *	   v  * cTexCoord[3] +
		(1 - u) *	   v  * cTexCoord[15]
	);

	float height = texture(uHeightMap, texCoord).r;
	height *= uScaleY;

	position.y = height;

	eTexCoord = texCoord;
	gl_Position = position;
}

#type geometry
#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

struct Material {
	sampler2D diffuseMap;
	sampler2D normalMap;
	sampler2D heightMap;
	float heightScaling;
	float horizontalScaling;
};

uniform Material uMaterials[3];
uniform int uTBNRange;
uniform sampler2D uSplatMap;

in vec2 eTexCoord[];
out Geometry
{
	vec3 fragPos;
	vec2 texCoord;
	vec3 tangent;
} geometry;

vec3 CalcTangent()
{
	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;

	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;

	vec2 uv0 = eTexCoord[0];
	vec2 uv1 = eTexCoord[1];
	vec2 uv2 = eTexCoord[2];

	vec2 deltaUV1 = uv1 - uv0;
	vec2 deltaUV2 = uv2 - uv0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main()
{
	vec3 displacement[3];
	for (int i = 0; i < 3; i++)
	{
		displacement[i] = vec3(0.0f);
	}

	float distanceToCamera = (distance(gl_in[0].gl_Position.xyz, uCamera.viewPosition)
				+ distance(gl_in[1].gl_Position.xyz, uCamera.viewPosition)
				+ distance(gl_in[2].gl_Position.xyz, uCamera.viewPosition)) / 3;

	vec3 tangent;
	if (distanceToCamera < uTBNRange)
	{
		tangent = CalcTangent();
		for (int k = 0; k < gl_in.length(); k++)
		{
			displacement[k] = vec3(0.0f, 1.0f, 0.0f);

			vec4 blendValues = texture(uSplatMap, eTexCoord[k]).rgba;
			float[4] blendValuesArray = float[] (blendValues.r, blendValues.g, blendValues.b, blendValues.a);

			float scale = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				scale += texture(uMaterials[i].heightMap, eTexCoord[k] * uMaterials[i].horizontalScaling).r
								* uMaterials[i].heightScaling * blendValuesArray[i];
			}

			float attenuation = clamp(-distance(gl_in[k].gl_Position.xyz, uCamera.viewPosition) / (uTBNRange - 50) + 1, 0.0f, 1.0f);
			scale *= attenuation;

			displacement[k] *= scale;
		}
	}

	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 worldPosition = gl_in[i].gl_Position + vec4(displacement[i], 0.0f);
		gl_Position = uCamera.projection * uCamera.view * worldPosition;
		geometry.fragPos = worldPosition.xyz;
		geometry.texCoord = eTexCoord[i];
		geometry.tangent = tangent;
		EmitVertex();
	}
	
	EndPrimitive();
}

#type fragment
#version 430

layout (location = 0) out vec4 aFragColor;

const vec3 LIGHT_DIRECTION = vec3(0.1f, -1.0f, 0.1f);
const float INTENSITY = 0.8f;
const float Z_FAR = 1000.0f;
const float SIGHT_RANGE = 1.6f;
const vec3 FOG_COLOR = vec3(0.65f, 0.85f, 0.9f);

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

struct Material {
	sampler2D diffuseMap;
	sampler2D normalMap;
	sampler2D heightMap;
	float heightScaling;
	float horizontalScaling;
};

uniform Material uMaterials[3];
uniform sampler2D uNormalMap;
uniform sampler2D uSplatMap;
uniform int uTBNRange;

in Geometry
{
	vec3 fragPos;
	vec2 texCoord;
	vec3 tangent;
} geometry;

float Diffuse(vec3 normal)
{
	return max(0.04f, dot(normal, -LIGHT_DIRECTION) * INTENSITY);
}

float GetFogFactor(float distance)
{
	return -0.0002f / SIGHT_RANGE * (distance - Z_FAR / 10.0f * SIGHT_RANGE) + 1;
}

void main()
{
	float distanceToCamera = length(uCamera.viewPosition - geometry.fragPos);
	float height = geometry.fragPos.y;

	vec3 normal = normalize(texture(uNormalMap, geometry.texCoord).rbg);
	vec4 blendValues = texture(uSplatMap, geometry.texCoord).rgba;

	float[4] blendValuesArray = float[] (blendValues.r, blendValues.g, blendValues.b, blendValues.a);

	if (distanceToCamera < uTBNRange - 50)
	{
		float attenuation = clamp(-distanceToCamera / (uTBNRange - 50) + 1, 0.0f, 1.0f);

		vec3 bitangent = normalize(cross(geometry.tangent, normal));
		mat3 TBN = mat3(geometry.tangent, normal, bitangent);

		vec3 bumpNormal;
		for (int i = 0; i < 3; i++)
		{
			bumpNormal += (2 * (texture(uMaterials[i].normalMap, geometry.texCoord * uMaterials[i].horizontalScaling).rbg) - 1) * blendValuesArray[i];
		}

		bumpNormal = normalize(bumpNormal);
		bumpNormal.xz *= attenuation;
		normal = normalize(TBN * bumpNormal);
	}

	vec3 fragColor = vec3(0.0f);
	for (int i = 0; i < 3; i++)
	{
		fragColor += texture(uMaterials[i].diffuseMap, geometry.texCoord * uMaterials[i].horizontalScaling).rgb * blendValuesArray[i];
	}

	float diffuse = Diffuse(normal);
	fragColor *= diffuse;

	float fogFactor = GetFogFactor(distanceToCamera);
	fragColor = mix(FOG_COLOR, fragColor, clamp(fogFactor, 0, 1));

	aFragColor = vec4(fragColor, 1.0);
}
