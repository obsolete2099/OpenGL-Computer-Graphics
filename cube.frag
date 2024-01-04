#version 330 core

out vec4 FragColor; // Output color

uniform vec4 lightColor;

void main()
{
    FragColor = lightColor; // Output the color directly
}
