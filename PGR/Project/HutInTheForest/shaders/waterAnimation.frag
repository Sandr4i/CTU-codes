#version 140

smooth in vec3 position_v;
smooth in vec2 texCoord_v;

uniform float time; //For light simulation
uniform mat4 Vmatrix;  
uniform sampler2D texSampler;



out vec4 color_f;

uniform ivec2 pattern = ivec2(5, 2); //8 frames in row, 2 rows
uniform float frameDuration = 0.1f;

vec4 sampleTexture(int frame){
	vec2 offset = vec2(1.0) / vec2(pattern);
	vec2 texCoordBase = texCoord_v / vec2(pattern);
	vec2 texCoord = texCoordBase + vec2(frame % pattern.x, (frame / pattern.x)) * offset;
	
	return texture(texSampler, texCoord);
}

void main(){
	int frame = int(time/frameDuration);
	color_f = sampleTexture(frame);
}