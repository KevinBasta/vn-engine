#version 330 core
out vec4 fragmentTexture;

in vec2 passedTextureCoord;

uniform sampler2D inTexture;

void main() {
	fragmentTexture = texture(inTexture, passedTextureCoord);
}
