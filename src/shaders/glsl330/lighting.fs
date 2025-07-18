#version 330

in vec3 fragPos;
in vec3 fragNormal;

out vec4 finalColor;

uniform vec3 viewPos;
uniform vec4 ambient;

#define MAX_LIGHTS 4
uniform int lightType[MAX_LIGHTS];
uniform vec3 lightPos[MAX_LIGHTS];
uniform vec4 lightColor[MAX_LIGHTS];

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = ambient.rgb;

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec3 lightDir = vec3(0.0);
        if (lightType[i] == 2) // Directional light
            lightDir = normalize(-lightPos[i]); // lightPos IS direction
        else
            lightDir = normalize(lightPos[i] - fragPos);

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor[i].rgb;

        result += diffuse;
    }

    finalColor = vec4(result, 1.0);
}

