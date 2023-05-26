#type vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPosition, 1.0);
}

#type fragment
#version 460 core

layout (location = 0) out vec4 aFragColor;

in vec2 vTexCoord;

uniform sampler2D uScene;
uniform sampler2D uBloomBlur;

void main()
{        
	// Sample the original scene texture
    vec3 sceneColor  = texture(uScene, vTexCoord).rgb;
	
	// Sample the blurred bright texture
	vec3 bloomColor = texture(uBloomBlur, vTexCoord).rgb;

	// Combine the original scene and blurred bright textures
    sceneColor += bloomColor;

    aFragColor = vec4(sceneColor, 1.0);
}
