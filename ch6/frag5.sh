#version 300 es
precision mediump float;
uniform float uR;
in vec3 vPosition;
out vec4 fragColor;
in vec4 vAmbient;//接收从顶点着色器过来的环境光最终强度
in vec4 vDiffuse;//接收从顶点着色器过来的散射光最终强度
in vec4 vSpecular;//接收从顶点着色器过来的镜面反射光最终强度
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
	
	vec4 finalColor=vec4(color,1) ;
	fragColor = finalColor * vAmbient + finalColor*vDiffuse + finalColor * vSpecular;
}
