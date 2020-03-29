#version 330

in vec3 fN,fL,fV;

out vec4 fragColor;

uniform vec3 materialDiffuse,materialSpecular;
uniform vec4 materialAmbient;
void main() {
  vec3 N,L,V,R;
  vec4 color=vec4(0,0,0,0);
  float intensity=0;


  N=normalize(fN);
   L=normalize(fL);
   V=normalize(fV);
   R=normalize(2.0*dot(fL,fN)*fN-fL);

   intensity = pow(dot(V,R), 50.0);
color.rgb = color.rgb + materialSpecular * intensity;
  fragColor=color;
}
