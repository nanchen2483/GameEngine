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

uniform sampler2DArray uDepthMap;
uniform int uLayer;

void main()
{
    float depthValue = texture(uDepthMap, vec3(vTexCoord, uLayer)).r;
    aFragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
