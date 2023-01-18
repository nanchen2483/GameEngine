#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

uniform mat4 uModel;

void main()
{
	gl_Position = uCamera.projection * uCamera.view * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 aFragColor;

void main()
{
	aFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
