#version 300 es
uniform mat4 uMVPMatrix;
uniform mat4 uMMatrix;
uniform vec3 uLightLocation;
uniform vec3 uCamera;

in vec3  aPosition;
in vec3  aNormal;

out vec3 vPosition;
out vec4 vSpecular;

void setLight(
	in vec3 normal,
	inout vec4 specular,
	in vec3 lightLocation,
	in vec4 lightSpecular
)
{
	vec3 NormalTarget = aPosition + aNormal;
	vec3 newNormal = (uMMatrix *vec4(NormalTarget,1.0)).xyz - (uMMatrix *vec4(aPosition,1.0)).xyz;
	newNormal=normalize(newNormal);
	
	vec3 veye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz);  
	vec3 vp = lightLocation - (uMMatrix *vec4(aPosition,1.0)).xyz;
	vp=normalize(vp);
	
	vec3 halfVector=normalize(vp+veye);	//求视线与光线的半向量  
	float nDotViewHalfVector=dot(newNormal,halfVector);
	
	float shininess=50.0;				//粗糙度，越小越光滑    
	float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess));
	specular=lightSpecular*powerFactor; 
}
void main() {
	gl_Position = uMVPMatrix * vec4(aPosition,1.0f);
	
	vec4 diffuseTemp=vec4(0.0,0.0,0.0,0.0);
	setLight(normalize(aNormal), diffuseTemp, uLightLocation, vec4(0.7,0.7,0.7,1.0));
	
	vPosition = aPosition;
	vSpecular = diffuseTemp;
}