#version 140

smooth in vec2 texCoord_v;
smooth in vec2 position_v;

uniform float time;
uniform sampler2D texSampler;
uniform ivec2 pattern = ivec2(4, 4); //4 frames in 4 rows
uniform float frameDuration = 0.03f;

out vec4 color_f;

vec4 sampleTexture(int frame){
	vec2 texCoordStart = texCoord_v / vec2(pattern);
	vec2 texCoordEnd = texCoordStart + vec2(frame % pattern.x, frame/pattern.x) /vec2(pattern);

	return texture(texSampler, texCoordEnd);
}

void main(){
	int frame = int(time / frameDuration);
	color_f = sampleTexture(frame);
}