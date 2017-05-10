#version 330 core
in vec4 center;
in vec4 color;
uniform float radius;
uniform vec4 viewport;
uniform vec2 win;
out vec4 FragColor;
void main(void)
{
    vec2 ndc_center = center.xy+win/2;
    vec2 diff = gl_FragCoord.xy - ndc_center;
    float d2 = dot(diff,diff);
    float r2 = radius*radius;
    float a = dot(center,center);
    a = center.z ==0 ? a/5000: a/1000;
    if (d2>r2) {
//        gl_FragDepth = center.z;
        FragColor = vec4(0.0,1.0,0,1.0);
//        discard;
    } else {
        FragColor = color;
//        gl_FragColor = vec4(1.0,0.0,0,1.0);
    }


}

