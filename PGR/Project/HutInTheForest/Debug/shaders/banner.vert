#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform float time;

smooth out vec2 texCoord_v;

float decay = 0.07f;

void main(){
	gl_Position = PVMmatrix * vec4(position, 1.0f);
	float currentTime = time * decay;
	vec2 offset = vec2(0.0f, (floor(currentTime) - currentTime) * 3 + 1.0f);
	texCoord_v = texCoord - offset;
}