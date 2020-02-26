#version 330
in vec4 fcolor;
out vec4 fragColor;
uniform sampler2D textureUnit;
uniform float coeff;

in vec2 fTexCoord;

void main() {
    //fragColor=fcolor;
//     fragColor=texture(textureUnit,fTexCoord);
     fragColor=texture(textureUnit,fTexCoord)*fcolor.b;
      // coeff = uniform passée par l'application (questions précédentes).
       fragColor.g*=(1.0-coeff);

}
