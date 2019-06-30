#version 300 es
precision mediump float;
uniform float uR;
in vec3 vPosition;
out vec4 fragColor;

void main() {
	vec3 color;
	float n = 8.0;
	float span = 2.0*uR/n;
	//int i = int(abs(vPosition.x)/span);
	//int j = int(abs(vPosition.y)/span);
	//int k = int(abs(vPosition.z)/span);
	int i = int((vPosition.x + uR)/span);
	int j = int((vPosition.y + uR)/span);
	int k = int((vPosition.z + uR)/span);
	
	int whichColor = (i+j+k) % 2;
	if(whichColor == 1) { 
		color = vec3(0.678,0.231,0.129); //红色
	}else { 
		color = vec3(1.0,1.0,1.0); //白色
	}
	fragColor = vec4(color,1);
}
