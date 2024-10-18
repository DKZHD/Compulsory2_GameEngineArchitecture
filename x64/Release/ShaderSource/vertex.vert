#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 ProjModel;

void main(){
    gl_Position = ProjModel * vec4(aPos,0,1);
    Color = aColor;
}