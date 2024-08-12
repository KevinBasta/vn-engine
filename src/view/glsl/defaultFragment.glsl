#version 330 core
out vec4 fragmentTexture;

in vec2 passedTextureCoord;

uniform sampler2D inTexture;
uniform float inOpacity;

void main() {
	fragmentTexture = vec4(1.0f, 1.0f, 1.0f, inOpacity) * texture(inTexture, passedTextureCoord);
}
