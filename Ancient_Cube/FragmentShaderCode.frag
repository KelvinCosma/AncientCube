#version 330 core

in vec3 ourColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 color;

uniform vec3 LightPosition_worldspace;

void main()
{
    vec3 LightColor = vec3(0.5f, 1.0f, 1.0f);
    float LightPower = 100.0f;

    vec3 MaterialDiffuseColor = ourColor;
    vec3 MaterialAmbientColor = vec3(0.25f, 0.25f, 0.25f) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(0.25f, 0.5, 0.5f);

    float distance = length(LightPosition_worldspace - Position_worldspace);

    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);

    float cosTheta = clamp(dot(n, l), 0, 1);

    vec3 E = normalize(EyeDirection_cameraspace);
    vec3 R = reflect(-l,n);

    float cosAlpha = clamp(dot(E, R), 0, 1);

    MaterialDiffuseColor = MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance);
    MaterialSpecularColor = MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5.0f) / (distance*distance);

    vec3 MaterialColor = MaterialAmbientColor + MaterialDiffuseColor + MaterialSpecularColor;

    color = vec4 (MaterialColor, 1.0f);

}

