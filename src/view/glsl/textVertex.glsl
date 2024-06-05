#version 330 core
layout (location = 0) in vec4 inVertex; // <vec2 pos, vec2 tex>

out vec2 passedTextureCoord;

uniform mat4 inModel;

void main()
{
    gl_Position = inModel * vec4(inVertex.xy, 0.0, 1.0);
    passedTextureCoord = inVertex.zw;
}