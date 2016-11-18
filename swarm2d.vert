#version 330 core
attribute vec3 posAttr;
varying vec4 center;
uniform float radius;
uniform mat4 proj_Matrix;
uniform mat4 mv_Matrix;
void main(void)
{
    center = mv_Matrix*vec4(posAttr,1.0);
    gl_Position = proj_Matrix*mv_Matrix*vec4(posAttr,1.0);
}

