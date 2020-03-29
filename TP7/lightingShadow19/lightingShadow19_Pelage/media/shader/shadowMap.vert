#version 330

layout(location=0) in vec3 position;

uniform mat4 modelviewMatrix;
uniform mat4 mvp; // = projection*modelview
uniform mat4 textureEyeMatrix; // passage Texture->Eye

out vec4 fTexCoord; // coordonnées de texture passées au fragment shader

void main() {

  vec4 positionEye=modelviewMatrix*vec4(position,1); // sera exploité lors des questions du TP

  fTexCoord=vec4(0,0,0,1); // les coordonnées de texture de chaque sommet sont calculées (ici, initialisation à 0)

  gl_Position=mvp*vec4(position,1);
  //fTexCoord=gl_Position;
  //fTexCoord=vec4(position,1);
  //fTexCoord=positionEye;
   fTexCoord=textureEyeMatrix*positionEye;
}
