#version 140

in vec3 texCoord_v;

uniform bool fogSkyboxChange;
uniform bool dayLight;
uniform samplerCube skyboxSampler;

out vec4 color_f;

vec4 addFog(vec4 outputColor){
	float dist = length(vec4(1.0f));
	float density = 100.0f;
	float blendFactor = 1.0 / exp(dist * dist * density * density);

	blendFactor = clamp(blendFactor, 0.0f, 1.0f);
	outputColor = outputColor * blendFactor + (1 - blendFactor) * vec4(0.7f); //possibly change

	return outputColor;
}

void main(){
	color_f = texture(skyboxSampler, texCoord_v);

	if(fogSkyboxChange) color_f = addFog(color_f);
	if(!dayLight) color_f = color_f * 0.2f;
}