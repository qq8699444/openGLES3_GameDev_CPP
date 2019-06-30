#version 300 es
uniform mat4 uMVPMatrix;
in vec3  aPosition;
out vec3 vPosition;
out vec4 vAmbient;
void main() {
	gl_Position = uMVPMatrix * vec4(aPosition,1.0f);
	vPosition = aPosition;
	vAmbient = vec4(0.15,0.15,0.15,1.0);
}