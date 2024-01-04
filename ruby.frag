#version 330 core

out vec4 FragColor; // Output color

in vec3 color; // Input color data, no need for texCoord
in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

// Define your material properties here
Material material = Material(
    vec4(0.1745, 0.01175, 0.01175, 1.0),
    vec4(0.61424, 0.04136, 0.04136, 1),
    vec4(0.727811, 0.626959, 0.626959, 1.0),
    76.8
);

void main()
{
    float ambientStrength = 0.30f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    float specular = specAmount * specularLight;

    // Calculate the final color with material properties
    vec4 ambientColor = material.ambient * lightColor;
    vec4 diffuseColor = material.diffuse * lightColor * diffuse;
    vec4 specularColor = material.specular * lightColor * specular;

    FragColor = vec4(color, 1.0) * (ambientColor * ambientStrength + diffuseColor + specularColor);
}
