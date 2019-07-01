#version 300 es
precision mediump float;
uniform float uR;
in vec3 vPosition;
in vec3 vNormal;

uniform mat4 uMMatrix;
uniform vec3 uLightLocation;	//定向光方向
uniform vec3 uCamera;

out vec4 fragColor;

void setLight(
	in vec3 normal,
	inout vec4 ambient,			//环境光最终强度
	inout vec4 diffuse,				//散射光最终强度
	inout vec4 specular,			//镜面光最终强度
	in vec3 lightLocation,
	in vec4 lightAmbient,			//环境光强度
	in vec4 lightDiffuse,			//散射光强度
	in vec4 lightSpecular
)
{
	ambient = lightAmbient;
	
	vec3 NormalTarget = vPosition + normal;
	vec3 newNormal = (uMMatrix *vec4(NormalTarget,1.0)).xyz - (uMMatrix *vec4(vPosition,1.0)).xyz;
	newNormal=normalize(newNormal);
	
	vec3 veye= normalize(uCamera-(uMMatrix*vec4(vPosition,1)).xyz);  
	vec3 vp = lightLocation - (uMMatrix *vec4(vPosition,1.0)).xyz;
	vp=normalize(vp);
	//vec3 vp= normalize(lightDirection);  //规格化定向光方向向量
	
	vec3 halfVector=normalize(vp+veye);	//求视线与光线的半向量  
	float nDotViewHalfVector=dot(newNormal,halfVector);
	
	float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//求法向量与vp的点积与0的最大值
	diffuse=lightDiffuse*nDotViewPosition;				//计算散射光的最终强度

	float shininess=50.0;				//粗糙度，越小越光滑    
	float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess));
	specular=lightSpecular*powerFactor; 
}

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
	
	vec4 ambient,diffuse,specular;	  //用来接收三个通道最终强度的变量 
	setLight(normalize(vNormal), 
					ambient,diffuse, specular,
					uLightLocation, 
					vec4(0.15,0.15,0.15,1.0),vec4(0.8,0.8,0.8,1.0),vec4(0.7,0.7,0.7,1.0));
					
			
	fragColor = finalColor * ambient + finalColor*diffuse + finalColor * specular;
}
