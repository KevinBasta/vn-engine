#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTextureCoord;

out vec2 passedTextureCoord;

//uniform mat4 inModel;
//uniform mat4 inView;
//uniform mat4 inProjection;

void main() {
	gl_Position = vec4(inPos.x, -1 * inPos.y, inPos.z, 1.0);
	passedTextureCoord = inTextureCoord;
	//gl_Position = inProjection * inView * inModel * vec4(inPos, 1.0);
}
