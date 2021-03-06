#version 300 es

layout (location = 0) in vec2 vPosition;
out vec2 texcoord;
out vec2 position;

void main() {
	gl_Position = vec4(vPosition.xy, 0, 1);
	texcoord.x = vPosition.x * 0.5 + 0.5;
	texcoord.y = vPosition.y * 0.5 + 0.5;
	position = vPosition;
}
