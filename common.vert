#version 330 core
in vec3 vertex;
in vec4 color;
out vec4 outColor;
uniform mat4 matrix;
void main(void)
{
    outColor = color;
    gl_Position = matrix*vec4(vertex,1.0);
}

