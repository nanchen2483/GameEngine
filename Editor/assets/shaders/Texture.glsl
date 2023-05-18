#type vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uModel;

out Vertex
{
	vec3 position;
	vec2 texCoord;
} vertex;

void main()
{
	vertex.position = aPosition;
	vertex.texCoord = aTexCoord;
	gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 460 core

layout (location = 0) out vec4 aFragColor;

in Vertex
{
	vec3 position;
	vec2 texCoord;
} vertex;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main()
{
	aFragColor = texture(uTexture, vertex.texCoord);
}
