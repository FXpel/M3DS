#version 330
/**
  <c> F.Aubert Univ-Lille1
  **/

layout(location=0) in vec3 position;
layout(location=2) in vec2 texCoord;

out vec4 fTexCoord;

uniform mat4 mvp;
uniform mat4 textureMatrix;

void main(void) {
  fTexCoord=textureMatrix*vec4(texCoord.st,1,1);
  gl_Position=mvp*vec4(position,1.0);
}
