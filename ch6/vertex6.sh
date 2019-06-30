#version 300 es
uniform mat4 uMVPMatrix;
uniform mat4 uMMatrix;
uniform vec3 uLightDirection;	//定向光方向
uniform vec3 uCamera;

in vec3  aPosition;
in vec3  aNormal;

out vec3 vPosition;
out vec4 vAmbient;			//用于传递给片元着色器的环境光最终强度
out vec4 vDiffuse;			//用于传递给片元着色器的散射光最终强度
out vec4 vSpecular;			//用于传递给片元着色器的镜面光最终强度

void setLight(
	in vec3 normal,
	inout vec4 ambient,			//环境光最终强度
	inout vec4 diffuse,				//散射光最终强度
	inout vec4 specular,			//镜面光最终强度
	in vec3 lightDirection,
	in vec4 lightAmbient,			//环境光强度
	in vec4 lightDiffuse,			//散射光强度
	in vec4 lightSpecular
)
{
	ambient = lightAmbient;
	
	vec3 NormalTarget = aPosition + aNormal;
	vec3 newNormal = (uMMatrix *vec4(NormalTarget,1.0)).xyz - (uMMatrix *vec4(aPosition,1.0)).xyz;
	newNormal=normalize(newNormal);
	
	vec3 veye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz);  
	//vec3 vp = lightLocation - (uMMatrix *vec4(aPosition,1.0)).xyz;
	//vp=normalize(vp);
	vec3 vp= normalize(lightDirection);  //规格化定向光方向向量
	
	vec3 halfVector=normalize(vp+veye);	//求视线与光线的半向量  
	float nDotViewHalfVector=dot(newNormal,halfVector);
	
	float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//求法向量与vp的点积与0的最大值
	diffuse=lightDiffuse*nDotViewPosition;				//计算散射光的最终强度

	float shininess=50.0;				//粗糙度，越小越光滑    
	float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess));
	specular=lightSpecular*powerFactor; 
}
void main() {
	gl_Position = uMVPMatrix * vec4(aPosition,1.0f);
	
	vec4 ambientTemp,diffuseTemp,specularTemp;	  //用来接收三个通道最终强度的变量 
	setLight(normalize(aNormal), 
					ambientTemp,diffuseTemp, specularTemp,
					uLightDirection, 
					vec4(0.15,0.15,0.15,1.0),vec4(0.8,0.8,0.8,1.0),vec4(0.7,0.7,0.7,1.0));
	
	vPosition = aPosition;
	vAmbient = ambientTemp;
	vDiffuse = diffuseTemp;
	vSpecular = specularTemp;
}