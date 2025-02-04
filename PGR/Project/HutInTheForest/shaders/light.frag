#version 140

in vec3 position_v;
in vec3 normal_v;
in vec2 texCoord_v;

uniform bool isTextureUsed;
uniform sampler2D texSampler;
uniform float time;
uniform mat4 Vmatrix;
uniform vec4 sunPosition;
uniform vec3 sunAmbience;

uniform vec3 reflectorPosition;
uniform vec3 reflectorDirection;
uniform bool reflectorSwitch;

vec3 fogColor = vec3(0.7f, 0.7f, 0.7f);
float visibility;
float density = 0.2f;
float fogCuttOff = 1.4f;
uniform bool fogSwitch;

uniform bool dayLight;
uniform bool lightSwitch;

out vec4 color_f;

struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;

	float cosCutOff;
	float exponent;
};

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shiness;
};

Light sun = {
	vec3(1.0f, 1.0f, 0.6f), //diffuse
	vec3(0.5f), vec3 (1.0f), // ambient, specular
	vec3(1.0f, 1.0f, 20.0f), //position
	vec3(0.0f), 0, 0 //direction
};
float sunSpeed = 0.4f;
Light lightChange;

uniform Material material;

vec4 reflectorLight(Light light, Material material, vec3 vertexposition, vec3 vertexNormal){
	vec3 dir = normalize(light.position - vertexposition);
	float diffuseCoef = max(0.0, dot(vertexNormal, dir));
	float specualrCoef = max(0.0, dot(reflect(vertexNormal, -dir), normalize(-vertexposition)));
	float posCoef = max(0.0, dot(-dir, light.direction));

	vec3 ret = material.specular * light.specular * pow(specualrCoef, material.shiness);
	ret += material.ambient * light.ambient;
	ret += material.diffuse * light.diffuse * diffuseCoef;
	if(posCoef < light.cosCutOff) ret *= 0.0;
	else ret *= pow(posCoef, light.exponent);

	return vec4(ret, 1.0);
}

vec4 pointLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal){
	vec3 dir = normalize(light.position);
	float a = dot(dir, vertexNormal);
	float b = dot(normalize(-vertexPosition), normalize(reflect(-dir, vertexNormal)));

	vec3 ret = pow(max(b, 0), material.shiness) * light.specular * material.specular;
	ret += material.ambient * light.ambient;
	ret == max(a, 0) * light.diffuse * material.diffuse;

	float constantCoef = 0.2f;
	float linearCoef = 0.2f;
	float quadraticCoef = 1.15f;
	float dist = distance(light.position, vertexPosition);
	float attenuation = 1.0f / (constantCoef + (linearCoef * dist) + (quadraticCoef * dist * dist));
	ret = attenuation * ret;

	return vec4(ret, 1.0f);
}

vec4 directLight(Light light, Material material, vec3 vertexNormal, vec3 vertexPosition){
	vec3 dir = normalize(light.position);
	float a = dot(dir, vertexNormal);
	float b = dot(normalize(-vertexPosition), normalize(reflect(-dir, vertexNormal)));

	vec3 ret = pow(max(b, 0), material.shiness) * light.specular * material.specular;
	ret += material.ambient * light.ambient;
	ret == max(a, 0) * light.diffuse * material.diffuse;

	return vec4(ret, 1.0f);
}

Light reflector;

void setupLights(){
	reflector.ambient = vec3(0.5f);
	reflector.diffuse = vec3(1.0f);
	reflector.specular = vec3(1.0);

	reflector.position = (Vmatrix * vec4(reflectorPosition, 1.0f)).xyz;
	reflector.direction = normalize((Vmatrix * vec4(reflectorDirection, 0.0f)).xyz);
	reflector.cosCutOff = 0.993f;

	sun.ambient = sunAmbience;
	sun.position = sunPosition.xyz;

	lightChange.ambient = vec3(0.2f);
	lightChange.diffuse = vec3(1.0f, 0.4f, 0.0f);
	lightChange.specular = vec3(0.1);
	lightChange.position = (Vmatrix * vec4(vec3(4.0f, -0.6f, -1.5f), 1.0f)).xyz;
	float dist = length(position_v.xyz);
	visibility = clamp(exp(-pow(dist*density, fogCuttOff)), 0, 1);
}

void main(){
	setupLights();

	vec3 globalAmbientLight = vec3(0.35f);
	color_f = vec4(material.ambient * globalAmbientLight, 0.0);
	vec3 normalModification = normal_v;

	if(dayLight) color_f += directLight(sun, material, position_v, normalModification);
	else color_f *= 2.0f;

	if(reflectorSwitch) color_f += reflectorLight(reflector, material, position_v, normalModification);
	if(lightSwitch) color_f += pointLight(lightChange, material, position_v, normalModification);
	
	if(isTextureUsed) color_f *= texture(texSampler, texCoord_v);
	if (fogSwitch){
		color_f = mix(vec4(fogColor, 1.0f), color_f, visibility);
		if (!dayLight) color_f *= 0.2;
	}

}

