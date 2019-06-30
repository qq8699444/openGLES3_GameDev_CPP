#version 300 es
uniform mat4 uMVPMatrix;
uniform mat4 uMMatrix;
uniform vec3 uLightLocation;

in vec3  aPosition;
in vec3  aNormal;

out vec3 vPosition;
out vec4 vDiffuse;

void setLight(
	in vec3 normal,
	inout vec4 diffuse,
	in vec3 lightLocation,
	in vec4 lightDiffuse
)
{
	vec3 NormalTarget = aPosition + aNormal;
	vec3 newNormal = (uMMatrix *vec4(NormalTarget,1.0)).xyz - (uMMatrix *vec4(aPosition,1.0)).xyz;
	newNormal=normalize(newNormal);
	
	vec3 vp = lightLocation - (uMMatrix *vec4(aPosition,1.0)).xyz;
	vp=normalize(vp);
	float nDotViewPosition=max(0.0,dot(newNormal,vp));
	diffuse=lightDiffuse*nDotViewPosition;
}
void main() {
	gl_Position = uMVPMatrix * vec4(aPosition,1.0f);
	
	vec4 diffuseTemp=vec4(0.0,0.0,0.0,0.0);
	setLight(normalize(aNormal), diffuseTemp, uLightLocation, vec4(0.8,0.8,0.8,1.0));
	
	vPosition = aPosition;
	vDiffuse = diffuseTemp;
}