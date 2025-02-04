#version 140

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform float time;
uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 normalMatrix;

smooth out vec2 texCoord_v;
out vec3 position_v;
out vec3 normal_v;

const float rotationAmplitude = 20.0; // Rotation amplitude in degrees
const float rotationPeriod = 14.0; // Rotation period in seconds

void main()
{
    // Calculate the rotation angle based on time
    float rotationAngle = rotationAmplitude * sin(2.0 * 3.14159 * time / rotationPeriod);

    // Rotate the position around the x-axis
    float cosAngle = cos(radians(rotationAngle));
    float sinAngle = sin(radians(rotationAngle));
    mat3 rotationMatrix = mat3(
        1.0, 0.0, 0.0,
        0.0, cosAngle, -sinAngle,
        0.0, sinAngle, cosAngle
    );
    vec3 rotatedPosition = rotationMatrix * position;

    // Transform the rotated position
    position_v = (Vmatrix * Mmatrix * vec4(rotatedPosition, 1.0)).xyz;

    // Calculate the transformed position for rendering
    gl_Position = PVMmatrix * vec4(rotatedPosition, 1.0);
    texCoord_v = texCoord;

    // Calculate the transformed normal for lighting
    normal_v = normalize((Vmatrix * normalMatrix * vec4(normal, 0.0)).xyz);
}
