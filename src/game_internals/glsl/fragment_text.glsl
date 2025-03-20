#version 330 core
in vec2 passedTextureCoord;

out vec4 fragmentColor;

uniform sampler2D inText;
uniform vec3 inTextColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(inText, passedTextureCoord).r);
    fragmentColor = vec4(inTextColor, 1.0) * sampled;
}