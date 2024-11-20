#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

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

uniform sampler2D texImage;

in float diffuseFactor;
in float specularFactor;
in vec2 texCoords;

void main()
{
    vec3 texColor = texture2D(texImage, texCoords).xyz;
    vec3 finalColor
            = material.emitionFactor * texColor
            + material.ambientFactor * texColor
            + material.diffuseFactor * diffuseFactor * light.color * texColor
            + material.specularFactor * specularFactor * light.color;
    fColor = vec4(finalColor, 1.0);
}
