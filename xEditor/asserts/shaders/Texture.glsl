#type vertex
#version 330 core
			
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
			
uniform mat4 uViewProjection;
uniform mat4 uModel;

out vec3 vPosition;
out vec2 vTexCoord;
			
void main()
{
	vPosition = aPosition;
	vTexCoord = aTexCoord;
	gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core
			
layout (location = 0) out vec4 color;
			
in vec3 vPosition;
in vec2 vTexCoord;
			
uniform vec3 uColor;
uniform sampler2D uTexture;
			
void main()
{
	color = texture(uTexture, vTexCoord);
}
