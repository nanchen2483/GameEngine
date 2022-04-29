#type vertex
#version 450 core
			
layout(location = 0) in vec3 aPosition;

layout (std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPosition;
} uCamera;

out vec3 vTexCoords;

void main()
{
	vTexCoords = aPosition;
	vec4 pos = uCamera.projection * mat4(mat3(uCamera.view))  * vec4(aPosition, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube uSkybox;
			
void main()
{
	FragColor = texture(uSkybox, vTexCoords);
}
