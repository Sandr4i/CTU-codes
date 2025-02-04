#version 140

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 normalMatrix;

out vec3 normal_v;
out vec3 position_v;
smooth out vec2 texCoord_v;

void main(){
	position_v = (Vmatrix * Mmatrix * vec4(position, 1.0f)).xyz;
	gl_Position = PVMmatrix * vec4(position, 1);
	normal_v = normalize((Vmatrix * normalMatrix * vec4(normal, 0.0)).xyz);

	texCoord_v = texCoord;
}