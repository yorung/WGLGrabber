#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
out vec2 texcoord;
out vec2 position;
out vec3 normal;
out vec4 color;

layout (std430) buffer WaterSSBO {
	mat4 matW;
	mat4 matV;
	mat4 matP;
	float time;
};

const float airToWater = 1.0 / 1.33333;
const vec3 camDir = vec3(0, 0, -1);
const float waterDepth = 0.2;

void main() {
	mat4 matWV = matV * matW;
	gl_Position = matWV * vec4(vPosition.xyz, 1) * matP;
	normal = normalize(vNormal) * mat3(matW);

	vec3 rayDir = refract(camDir, normal, airToWater);

	vec3 bottom = rayDir * waterDepth / rayDir.z;

	position = vPosition.xz;
	texcoord = (vPosition.xz + bottom.xy) * vec2(0.5, -0.5) + vec2(0.5, 0.5);

	float mask = dot(normal, vec3(0, 0, 1));
	color = vec4(1, 1, 1, 1.0 - mask);
}
