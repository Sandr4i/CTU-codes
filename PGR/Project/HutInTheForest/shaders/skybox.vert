#version 140

in vec2 screenCoord;

uniform mat4 inversePVmatrix;

out vec3 texCoord_v;

void main(){
	vec4 farplaneCoord = vec4(screenCoord, 0.9999f, 1.0f);  //possibly change
	vec4 worldViewCoord = inversePVmatrix * farplaneCoord;
	texCoord_v = worldViewCoord.xyz / worldViewCoord.w;
	gl_Position = farplaneCoord;
}