#type vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aEntityId;

out vec2 vTexCoord;
out flat int vEntityId;

void main()
{
	gl_Position = vec4(aPosition, 1.0);
	vTexCoord = aTexCoord;
	vEntityId = aEntityId;
}

#type control
#version 460 core

layout (vertices = 4) out;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform mat4 uModel;

in vec2 vTexCoord[];
in flat int vEntityId[];
out vec2 cTexCoord[];
out flat int cEntityId[];

void main()
{
	if (gl_InvocationID == 0)
	{
		const int MIN_TESS_LEVEL = 4;
		const int MAX_TESS_LEVEL = 64;
		const float MIN_DISTANCE = 20;
		const float MAX_DISTANCE = 800;

		vec4 eyeSpacePos00 = uCamera.view * uModel * gl_in[0].gl_Position;
		vec4 eyeSpacePos01 = uCamera.view * uModel * gl_in[1].gl_Position;
		vec4 eyeSpacePos10 = uCamera.view * uModel * gl_in[2].gl_Position;
		vec4 eyeSpacePos11 = uCamera.view * uModel * gl_in[3].gl_Position;

		// "distance" from camera scaled between 0 and 1
		float distance00 = clamp( (abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
		float distance01 = clamp( (abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
		float distance10 = clamp( (abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
		float distance11 = clamp( (abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );

		float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
		float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
		float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
		float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

		gl_TessLevelOuter[0] = tessLevel0;
		gl_TessLevelOuter[1] = tessLevel1;
		gl_TessLevelOuter[2] = tessLevel2;
		gl_TessLevelOuter[3] = tessLevel3;

		gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
		gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
	}

	cTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
	cEntityId[gl_InvocationID] = vEntityId[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

#type evaluation
#version 460 core

layout (quads, fractional_odd_spacing, cw) in;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform sampler2D uHeightMap;
uniform mat4 uModel;

in vec2 cTexCoord[];
in flat int cEntityId[];
out float eHeight;
out flat int eEntityId;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 position =
	(
		(1 - u) * (1 - v) * gl_in[2].gl_Position +
			 u  * (1 - v) * gl_in[0].gl_Position +
			 u  *	   v  * gl_in[1].gl_Position +
		(1 - u) *	   v  * gl_in[3].gl_Position
	);

	vec2 texCoord =
	(
		(1 - u) * (1 - v) * cTexCoord[2] +
			 u  * (1 - v) * cTexCoord[0] +
			 u  *	   v  * cTexCoord[1] +
		(1 - u) *	   v  * cTexCoord[3]
	);

	eHeight = texture(uHeightMap, texCoord).y * 64.0f - 16.0f;

	position.y = eHeight;

	eEntityId = cEntityId[0];
	gl_Position = uCamera.projection * uCamera.view * uModel * position;
}

#type fragment
#version 460 core

layout (location = 0) out vec4 aFragColor;
layout (location = 1) out int aEntityId;

in float eHeight;
in flat int eEntityId;

void main()
{
	float h = (eHeight + 16.0f) / 64.0f;
	aFragColor = vec4(h, h, h, 1.0);
	aEntityId = eEntityId;
}
