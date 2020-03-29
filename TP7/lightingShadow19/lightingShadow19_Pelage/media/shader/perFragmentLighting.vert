#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

uniform mat4 modelviewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvp;

uniform vec3 lightPosition;

out vec3 fN,fL,fV,fR;
out vec4 fColor;
uniform vec3 materialDiffuse,materialSpecular;
uniform vec4 materialAmbient;
void main() {
  vec4 positionEye;
  float intensity=0;


  positionEye=modelviewMatrix*vec4(position,1);

  fV=-positionEye.xyz/positionEye.w;
  fL=lightPosition+fV;
  fN=normalMatrix*normal;

//  fL=normalize(fL);
//  fV=normalize(fV);
//  fN=normalize(fN);
//  fR = 2 * dot(fL,fN) * fN - fL;
//  fR = normalize(fR);

//  intensity = pow(dot(fV,fR), 50.0);

  //fColor.rgb = fColor.rgb + materialSpecular * intensity;
  gl_Position=mvp*vec4(position,1);
}
