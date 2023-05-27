#type vertex
#version 460 core

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
#version 460 core

layout (location = 0) out vec4 aFragColor;
layout (location = 1) out vec4 aBrightColor;

uniform vec3 uLightColor;

void main()
{           
    aFragColor = vec4(uLightColor, 1.0);
    float brightness = dot(aFragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
	{
        aBrightColor = vec4(aFragColor.rgb, 1.0);
	}
	else
	{
		aBrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
