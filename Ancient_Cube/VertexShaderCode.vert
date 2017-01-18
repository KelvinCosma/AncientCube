#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 ourColor;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

void main()
{
    gl_Position = MVP * vec4(position, 1.0f);

    Position_worldspace = (M * vec4(position, 1.0f)).xyz;

    vec3 VertexPosition_worldspace = (V * M * vec4(position, 1.0f)).xyz;
    EyeDirection_cameraspace = vec3(0.0f, 0.0f, 0.0f) - VertexPosition_worldspace;

    vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1.0f)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	Normal_cameraspace = ( V * M * vec4(normal, 0.0f)).xyz;

	ourColor = color;
}
