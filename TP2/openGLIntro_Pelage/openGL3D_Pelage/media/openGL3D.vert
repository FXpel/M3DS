#version 330

layout(location=0) in vec3 position;
//layout(location=1) in vec3 color;
layout(location=1) in vec3 normal;
out vec3 fColor;
uniform vec3 lightposition;
uniform mat4 projection;
uniform mat4 transform;
uniform vec3 diffuseColor;
void main() {
    vec3 N0 = normal;
    vec3 L0 = lightposition - position ;
    L0 = normalize(L0);
    N0 = normalize(N0);
    vec4 N = transform * vec4(N0,0);
    vec4 L = vec4(L0,0);

  vec4 eyePosition = vec4(position,1);
  //vec4 clipPosition=vec4(position,1);
  vec4 clipPosition=projection*eyePosition;
  float intensity = max(dot(N,L),0);
  //fColor = vec3(intensity,intensity,intensity);
  fColor=intensity* diffuseColor;
  clipPosition = projection*transform*eyePosition;
  gl_Position=clipPosition;
}
