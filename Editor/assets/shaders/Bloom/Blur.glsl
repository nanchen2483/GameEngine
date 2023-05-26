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

uniform bool uHorizontal;
uniform float uWeight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 texOffset = 1.0 / textureSize(uScene, 0); // gets size of single texel
     vec3 result = texture(uScene, vTexCoord).rgb * uWeight[0];
     if(uHorizontal)
     {
         for (int i = 1; i < 5; ++i)
         {
            result += texture(uScene, vTexCoord + vec2(texOffset.x * i, 0.0)).rgb * uWeight[i];
            result += texture(uScene, vTexCoord - vec2(texOffset.x * i, 0.0)).rgb * uWeight[i];
         }
     }
     else
     {
         for (int i = 1; i < 5; ++i)
         {
             result += texture(uScene, vTexCoord + vec2(0.0, texOffset.y * i)).rgb * uWeight[i];
             result += texture(uScene, vTexCoord - vec2(0.0, texOffset.y * i)).rgb * uWeight[i];
         }
     }

     aFragColor = vec4(result, 1.0);
}
