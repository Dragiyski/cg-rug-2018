#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;
layout (location = 2) in vec2 texCoordinates_in;

// Specify the Uniforms of the vertex shader
 uniform mat4 modelTransform;
 uniform mat4 viewTransform;
 uniform mat4 projectionTransform;
 uniform mat3 normalTransform;

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
    float emitionFactor;
    float ambientFactor;
    float diffuseFactor;
    float specularFactor;
    float specularExponent;
};

uniform Light light;
uniform Material material;

// Specify the output of the vertex stage
out float diffuseFactor;
out float specularFactor;
out vec2 texCoords;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vertCoordinates_in, 1.0);

    vec3 position = vec3(modelTransform * vec4(vertCoordinates_in, 1.0));
    vec3 normalVector = normalize(normalTransform * vertNormal_in);
    vec3 lightVector = normalize(light.position - position);
    vec3 reflectedLightVector = normalize(reflect(-lightVector, normalVector));
    vec3 viewPoint = vec3(viewTransform * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    diffuseFactor = max(dot(lightVector, normalVector), 0.0f);
    specularFactor = pow(max(dot(reflectedLightVector, normalize(viewPoint - position)), 0), material.specularExponent);

    texCoords = texCoordinates_in;
}
