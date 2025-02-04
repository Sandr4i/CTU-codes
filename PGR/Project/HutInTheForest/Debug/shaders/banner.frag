#version 140

smooth in vec2 texCoord_v;
uniform sampler2D texSampler;
out vec4 color_f;

void main(){
	color_f = texture(texSampler, texCoord_v);
}