#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

uniform mat4 viewTransform;

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

in vec3 position;
in vec3 lightPosition;
in vec3 normalVector;
in vec2 texCoords;

void main()
{
    vec3 lightVector = normalize(lightPosition - position);
    vec3 normalizeNormalVector = normalize(normalVector);
    vec3 reflectedLightVector = normalize(reflect(-lightVector, normalVector));
    // This is corrected due to the view (camera) transformation. The view vector is toward the camera.
    // The camera was in (0, 0, 0), which allowed to use -position.
    // Since now model and view transform are separate, the view vector is based on the view point (0, 0, 0) transformed with the view transform.
    vec3 viewPoint = vec3(viewTransform * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    float diffuseFactor = max(dot(lightVector, normalVector), 0.0f);
    float specularFactor = pow(max(dot(reflectedLightVector, normalize(viewPoint - position)), 0), material.specularExponent);
    vec3 texColor = texture2D(texImage, texCoords).xyz;

    vec3 resultColor
            = material.emitionFactor * texColor // Emition of light
            + material.ambientFactor * texColor // Ambient illumination
            + material.diffuseFactor * diffuseFactor * light.color * texColor // Diffuse illumination
            + material.specularFactor * specularFactor * light.color; // Specular illumination
    fColor = vec4(resultColor, 1.0);
    //fColor = vec4(0.0, material.emitionFactor, 0.0, 1.0);
}
