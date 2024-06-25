#version 330 core
layout (location = 0) in vec4 inVertex; // <vec2 pos, vec2 tex>

out vec2 passedTextureCoord;

uniform mat4 inModel;
uniform mat4 inOrtho;

void main()
{
    gl_Position = inOrtho * inModel * vec4(inVertex.xy, 0.0, 1.0);
    passedTextureCoord = inVertex.zw;
}