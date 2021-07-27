#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in int aEntityId;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out flat int vEntityId;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
	vEntityId = aEntityId;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in flat int vEntityId;

uniform sampler2D uTextures[32];

void main()
{
	color = texture(uTextures[int(round(vTexIndex))], vTexCoord * 2.0f) * vColor;
	color2 = vEntityId;
}
