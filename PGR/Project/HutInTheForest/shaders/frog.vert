#version 140

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 normalMatrix; //inverse transposed model matrix

out vec3 position_v;
smooth out vec2 texCoord_v;


void main()
{
    position_v = (Vmatrix * Mmatrix * vec4(position, 1.0f)).xyz;
    gl_Position = PVMmatrix * vec4(position, 1);

	texCoord_v = texCoord;
}