#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // Use aColor for color data
layout (location = 2) in vec3 aNormal;

out vec3 color; // Output color to fragment shader
out vec3 Normal;
out vec3 crntPos;
out vec3 fragPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0f));
    fragPos = crntPos; // Pass the fragment position to the fragment shader

    gl_Position = camMatrix * vec4(crntPos, 1.0);

    color = aColor; // Use aColor as the color output
    Normal = aNormal;
}