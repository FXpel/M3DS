#version 330
/**
  <c> F.Aubert Univ-Lille
  **/

layout(location=0) in vec3 position;

layout(location=2) in vec4 texCoord;
out vec4 fTexCoord;

uniform mat4 mvp;
uniform mat4 texTransform;

void main(void) {


  fTexCoord=texTransform*vec4(position,1);
  gl_Position=mvp*vec4(position,1);
}
