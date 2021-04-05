#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	color = vColor;
}
