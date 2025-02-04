#version 140

in vec3 position_v;
smooth in vec2 texCoord_v;

uniform float alpha;
uniform float time;
uniform sampler2D texSampler;
uniform bool fogChange;

vec3 fogColor = vec3(0.7f, 0.7f, 0.7f);
float density = 0.3f;
float fogCutOff = 1.2f;

out vec4 color_f;

void main()
{
    color_f = texture(texSampler, texCoord_v);
    if(fogChange){
        float dist = length(position_v.xyz);
        float visible = clamp(exp(-pow(dist * density, fogCutOff)), 0, 1);

        color_f = mix(vec4(fogColor, 1.0f), color_f, visible);
    }
}
