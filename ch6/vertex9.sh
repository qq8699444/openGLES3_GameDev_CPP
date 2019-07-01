#version 300 es
uniform mat4 uMVPMatrix;
in vec3  aPosition;
in vec3  aNormal;
out vec3 vPosition;
out vec3 vNormal;


void main() {
	gl_Position = uMVPMatrix * vec4(aPosition,1.0f);

	vPosition = aPosition;
	vNormal = aNormal;
}