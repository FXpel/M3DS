#version 330
/**
  <c> F.Aubert Univ-Lille1
  **/

layout(location=0) in vec3 position;
layout(location=2) in vec2 texCoord;

out vec2 fTexCoord;

uniform mat4 mvp;

void main(void) {
  fTexCoord=texCoord;
  gl_Position=mvp*vec4(position,1.0);
}
