#version 300 es

precision mediump float;

layout (location = 0) out vec4 fragColor;

in vec2 position;
in vec2 texcoord;
uniform sampler2D sampler;
void main() {
	vec2 coord = vec2(texcoord.x, texcoord.y);
	fragColor = vec4(texture(sampler, coord).xyz * 2.0, 1.0);
}
