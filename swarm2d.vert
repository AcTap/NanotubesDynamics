#version 330 core
in vec3 posAttr;
in vec4 colAttr;
out vec4 center;
out vec4 color;
uniform float radius;
uniform mat4 proj_Matrix;
uniform mat4 mv_Matrix;
void main(void)
{
    color= colAttr;
    center = mv_Matrix*vec4(posAttr,1.0);
    gl_Position = proj_Matrix*mv_Matrix*vec4(posAttr,1.0);
}

