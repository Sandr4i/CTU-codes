#version 140

in vec3 position_v;
in vec3 normal_v;
smooth in vec2 texCoord_v;

uniform float time;
uniform float alpha;
uniform bool crowFogChange;
uniform sampler2D texSampler;

vec3 fogColor = vec3(0.7f, 0.7f, 0.7f);
float density = 0.25f;
float visibility;
float fogCutOff = 1.4f;

out vec4 color_f;

void main(){
	vec3 ambientLight = vec3(0.4f);
	//color_f = vec4(abs(sin(position_v.x) * cos(position_v.z)),
	//abs(sin(position_v.x) * sin(position_v.z)),
	//abs(cos(time)), alpha);
	vec3 finalColor = vec3(0.2f, 0.2f, 0.2f);
	finalColor *= texture(texSampler, texCoord_v);
	color_f = vec4(finalColor, 1.0f);

	if(crowFogChange){
		float dist = length(position_v.xyz);
		visibility = clamp(exp(-pow(dist * density, fogCutOff)), 0, 1);
		color_f = mix(vec4(fogColor, 1.0f), color_f, visibility);
	}
}