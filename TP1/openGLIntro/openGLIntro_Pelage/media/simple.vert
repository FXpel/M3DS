#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
vec3 newPosition = position;
uniform float coeff;
layout(location=2)in vec2 texCoord;


out vec4 fcolor;
out vec4 fragColor;
out vec2 fTexCoord;
void main() {
    fcolor = color;
    fTexCoord = texCoord;
//    newPosition = position * coeff;
//   gl_Position=vec4(newPosition,1.0);

    gl_Position=vec4(position,1.0);
}
