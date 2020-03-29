#version 330

in vec4 fColor;

out vec4 fragColor;

void main() {
  if (gl_FrontFacing)
    fragColor=fColor;
  else
    fragColor=vec4(0,0,0,1);
}
