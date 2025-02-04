#version 140

in vec3 position_v;
in vec2 texCoord_v;

uniform mat4 Vmatrix;
uniform bool frogFogChange;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shiness;
uniform sampler2D texSampler;
uniform bool useTexture;
uniform vec3 frogColor;

vec3 fogColor = vec3(0.7f, 0.7f, 0.7f);
float density = 0.25f;
float visibility;
float fogCutOff = 0.7f;

out vec4 color_f;

void main(){
	vec3 ambientLight = vec3(0.4f);
    vec3 finalColor = frogColor; // Green color for frog
    if (useTexture) {
       finalColor *= texture(texSampler, texCoord_v);
    }
    color_f = vec4(finalColor, 1.0f);
	//color_f = texture(texSampler, texCoord_v);

	if(frogFogChange){
		float dist = length(position_v.xyz);
		visibility = clamp(exp(-pow(dist * density, fogCutOff)), 0, 1);
		//color_f = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		color_f = mix(vec4(fogColor, 1.0f), color_f, visibility);
	}
}