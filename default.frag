#version 330 core

out vec4 FragColor; // Output color

in vec3 color; // Input color data
in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
    float ambient = 0.20f;
    
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    FragColor = vec4(color, 1.0) * lightColor * (diffuse + ambient + specular); 
}