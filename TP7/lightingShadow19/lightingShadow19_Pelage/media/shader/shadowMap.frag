#version 330

uniform sampler2D depthTexture;

in vec4 fTexCoord;

out vec4 fragColor;

void main() {
  vec4 texCoord=fTexCoord;

  texCoord.xyz/=texCoord.w; // clip coordinates => ndc

  texCoord+=1;
  texCoord/=2;

  fragColor=texture(depthTexture,texCoord.xy);

  //Q8 & 9
  if(texCoord.x <= 0 || texCoord.x >= 1 || texCoord.y <= 0 || texCoord.y >= 1 || texCoord.z <= 0 || texCoord.z >= 1)
      fragColor = vec4(0.,0.,0.,0.6);
    else if(texCoord.z-0.001 < fragColor.r)
      fragColor = vec4(0.,0.,0.,0.6);
    else
      fragColor = vec4(0.,0.,0.,0.);
}
